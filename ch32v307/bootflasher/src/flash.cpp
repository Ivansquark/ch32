#include "flash.h"

Flash* Flash::pThis = nullptr;

Flash::Flash() {
    pThis = this;
    bootFlag = 0xFF;
    lock();
    unlock();
    lock();
    readBootFlag();
}

void Flash::writeBootFlag() {
    uint32_t flag = 0x01;
    programPage(FLASH_BOOT_FLAGS_ADDR, (uint8_t*)&flag, 4);
}

void Flash::clearBootFlag(void) { erasePage(FLASH_BOOT_FLAGS_ADDR); }

void Flash::readBootFlag(void) {
    bootFlag = (*(uint32_t*)(FLASH_BOOT_FLAGS_ADDR));
}

void Flash::readIPs() { memcpy((void*)IPs, (void*)FLASH_IP_ADDR, 4); }
void Flash::writeIPs(uint8_t ip0, uint8_t ip1, uint8_t ip2, uint8_t ip3) {
    IPs[0] = ip0;
    IPs[1] = ip1;
    IPs[2] = ip2;
    IPs[3] = ip3;
    programPage(FLASH_IP_ADDR, IPs, 4);
}

void Flash::WritePagesFromBank2ToBank1() {
    for (volatile int i = 0; i < 500; i++) {
        readPage(FLASH_BANK2_FIRMWARE + i * PAGE_SIZE);
        programPage(FLASH_BANK1_FIRMWARE + i * PAGE_SIZE, ramPage, PAGE_SIZE);
    }
}

void Flash::erasePage(uint32_t pageAddr) {
    lock();
    unlock();
    pageAddr &= 0xFFFFFF00;
    // no flash memory operation is ongoing (BSY -busy)
    while (FLASH->STATR & FLASH_STATR_BSY) {}
    // reset bit end off operation
    if (FLASH->STATR & FLASH_STATR_EOP) { FLASH->STATR = FLASH_STATR_EOP; }
    // single page erase chosen
    FLASH->CTLR |= FLASH_CTLR_PAGE_ER;
    // choose page address
    FLASH->ADDR |= pageAddr;
    // triggers an ER/PR operation when set.
    FLASH->CTLR |= FLASH_CTLR_STRT;
    while (FLASH->STATR & FLASH_STATR_BSY) {}
    // while not end of operation FLASH->SR = FLASH_SR_EOP;   // reset bit
    FLASH->CTLR &= ~FLASH_CTLR_PAGE_ER;
    lock();
}

void Flash::programPage(uint32_t pageAddr, uint8_t* buf, uint16_t len) {
    uint16_t tempLen;
    uint8_t remainder = len % PAGE_SIZE;
    if (remainder) {
        tempLen = len / PAGE_SIZE;
    } else {
        tempLen = 1 + (len / PAGE_SIZE);
    }
    for (int i = 0; i < tempLen; i++) { erasePage(pageAddr + PAGE_SIZE * i); }

    lock();
    unlock();
    while (FLASH->STATR & FLASH_STATR_BSY) {}
    // reset bit end off operation
    if (FLASH->STATR & FLASH_STATR_EOP) { FLASH->STATR = FLASH_STATR_EOP; }
    // choose page address
    FLASH->ADDR |= FLASH_BOOT_FLAGS_ADDR;
    // single page program chosen
    FLASH->CTLR |= FLASH_CTLR_PAGE_PG;
    //!< 1 byte writing>
    for (volatile int i = 0; i < len / 4; i++) {
        *((uint32_t*)(pageAddr) + i) = *((uint32_t*)(buf) + i);
        while (FLASH->STATR & FLASH_STATR_WR_BSY) {}
    }
    FLASH->CTLR |= FLASH_CTLR_PG_STRT;
    while (FLASH->STATR & FLASH_STATR_BSY) {}
    // finish programming
    FLASH->CTLR &= ~(FLASH_CTLR_PAGE_PG);
    lock();
}
void Flash::lock(void) { FLASH->CTLR |= FLASH_CTLR_LOCK; }
void Flash::unlock(void) {
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;

    FLASH->MODEKEYR = FLASH_KEY1;
    FLASH->MODEKEYR = FLASH_KEY2;
}
void Flash::readPage(uint32_t pageAddr) {
    memcpy(ramPage, (const void*)pageAddr, PAGE_SIZE);
}
