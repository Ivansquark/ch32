#include "ch32v30x.h"
#include "flash.h"

#define FLASH_KEY1 ((uint32_t)0x45670123)
#define FLASH_KEY2 ((uint32_t)0xCDEF89AB)
#define FLASH_START 0x08000000
#define FLASH_BOOT_FLAGS_ADDR 0x08000800
#define FLASH_IP_ADDR 0x08000900
#define FLASH_BANK1_FIRMWARE 0x08000A00
#define FLASH_BANK2_FIRMWARE 0x08020000
#define PAGE_SIZE 0x100
#define BOOT_FLAG 0x01

extern "C" __attribute__((section("init_boot"))) int main_boot(void) {
    uint8_t ramPage[PAGE_SIZE] = {0};
    // unlock
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
    FLASH->MODEKEYR = FLASH_KEY1;
    FLASH->MODEKEYR = FLASH_KEY2;
    uint32_t bootFlag = (*(uint32_t*)(FLASH_BOOT_FLAGS_ADDR));

    // uint8_t buf[256] = {0};
    // flash.writeBootFlagAndSectorNum(3);
    // flash.programPage(flash.FLASH_BANK2_FIRMWARE, buf, 256);
    // flash.writeBootFlag();
    // flash.readBootFlag();
    // flash.clearBootFlag();
    // flash.readBootFlag();
    if (bootFlag == BOOT_FLAG) {
        // start flash new firmware
        // flash.WritePagesFromBank2ToBank1();
        for (volatile int i = 0; i < 500; i++) {
            while (FLASH->STATR & FLASH_STATR_BSY) {}
            // reset bit end off operation
            if (FLASH->STATR & FLASH_STATR_EOP) {
                FLASH->STATR = FLASH_STATR_EOP;
            }
            // choose page address
            FLASH->ADDR |= FLASH_BOOT_FLAGS_ADDR;
            // single page program chosen
            FLASH->CTLR |= FLASH_CTLR_PAGE_PG;
            //!< 1 byte writing>
            for (volatile int i = 0; i < PAGE_SIZE / 4; i++) {
                *((uint32_t*)(FLASH_BANK1_FIRMWARE) + i) =
                    *((uint32_t*)(FLASH_BANK2_FIRMWARE) + i);
                while (FLASH->STATR & FLASH_STATR_WR_BSY) {}
            }
            FLASH->CTLR |= FLASH_CTLR_PG_STRT;
            while (FLASH->STATR & FLASH_STATR_BSY) {}
            // finish programming
            FLASH->CTLR &= ~(FLASH_CTLR_PAGE_PG);
        }
        // erase page boot flag
        // no flash memory operation is ongoing (BSY -busy)
        while (FLASH->STATR & FLASH_STATR_BSY) {}
        // reset bit end off operation
        if (FLASH->STATR & FLASH_STATR_EOP) { FLASH->STATR = FLASH_STATR_EOP; }
        // single page erase chosen
        FLASH->CTLR |= FLASH_CTLR_PAGE_ER;
        // choose page address
        FLASH->ADDR |= FLASH_BOOT_FLAGS_ADDR;
        // triggers an ER/PR operation when set.
        FLASH->CTLR |= FLASH_CTLR_STRT;
        while (FLASH->STATR & FLASH_STATR_BSY) {}
        // while not end of operation FLASH->SR = FLASH_SR_EOP;   // reset bit
        FLASH->CTLR &= ~FLASH_CTLR_PAGE_ER;
        // lock
        FLASH->CTLR |= FLASH_CTLR_LOCK;
        //
        //___________________ Software RESET __________________________________
        NVIC->CFGR = NVIC_KEY3 | (1 << 7);
    } else {
        // goto firmware
        //! ___	here proceed transition to new programm _______________________
        // memset((void*)&stackEndPtr, 2, 0x10000);
        uint32_t mainAppAddr = FLASH_BANK1_FIRMWARE;
        // check first 4 bytes (where must be stack ptr)
        uint32_t mainAppStack = *((uint32_t*)(long)mainAppAddr);
        void (*mainApplication)() = (void (*)())(long)(mainAppAddr);
        // Corrected!!!
        // void (*mainApplication)() =
        //    (void (*)())(long)*(uint32_t*)(long)(mainAppAddr + 4);
        //__set_MTVEC((uint32_t)flash.FLASH_BANK1_FIRMWARE);
        //__asm volatile("la sp, mainAppStack");
        //__asm volatile("la sp, _eusrstack");
        mainApplication(); // go to start main programm
        /*!< in linker main programm need to set PROGRAMM START ADDRESS to
         * MAIN_PROGRAMM_ADDRESS (0x08020000 - sector 1)>*/
    }
    while (1) {}
}
