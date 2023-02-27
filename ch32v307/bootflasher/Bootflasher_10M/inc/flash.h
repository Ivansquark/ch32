#ifndef FLASH_H
#define FLASH_H

#include "main.h"
#include <stdint.h>
#include <string.h>
// CODE-256KB + RAM-64KB
// Kbytes:
// Page 0   0x08000000 - 0x080000FF 256 bytes ...
// Page 7   0x08000700 - 0x080007FF 256 bytes ... bootflasher
// Page 8   0x08000800 - 0x080000FF 256 bytes ... boot flag
// Page 9   0x08000900 - 0x080000FF 256 bytes ... IP + boot flag + align
//
// Page 10   0x08000A00 - 0x08000AFF 256 bytes ... main prog ..

// 128 kB 0x20000
// Page 512   0x08020000 - 0x080200FF 256 bytes ... new prog ..
class Flash {
  public:
    static constexpr uint32_t FLASH_KEY1 = ((uint32_t)0x45670123);
    static constexpr uint32_t FLASH_KEY2 = ((uint32_t)0xCDEF89AB);

    static constexpr uint32_t FLASH_START = 0x08000000;
    static constexpr uint32_t FLASH_BOOT_FLAGS_ADDR = 0x08000800;
    static constexpr uint32_t FLASH_IP_ADDR = 0x08000900;
    static constexpr uint32_t FLASH_BANK1_FIRMWARE = 0x08000A00;
    static constexpr uint32_t FLASH_BANK2_FIRMWARE = 0x08020000;
    static constexpr uint32_t PAGE_SIZE = 0x100;
    static constexpr uint32_t BOOT_FLAG = 0x01;

    Flash();
    static Flash* pThis;
    //-------- BOOT --------------
    void writeBootFlag();
    void clearBootFlag(void);
    void readBootFlag(void);
    //-------- IPs ---------------
    uint8_t IPs[4] = {0};
    void readIPs();
    void writeIPs(uint8_t ip0, uint8_t ip1, uint8_t ip2, uint8_t ip3);

    void readFromBank2ToRAM();
    bool WriteSectorsFromBank2ToBank1();
    bool writeBufBank1(uint32_t len);
    bool writeBufBank2(uint32_t len);

    // first half of 128 kB RAM (second in CCMRAM)
    //uint8_t RamBuff[HALF_SECTOR_SIZE];
    
    uint8_t sectors = 0;
    // if not 0x00000001 => start main program
    // else start bootflasher
    uint32_t bootFlag = 0xFFFFFFFF;
    uint8_t ramPage[PAGE_SIZE];

    void clearRamBuffer(void);
    
    void programPage(uint32_t pageAddr, uint8_t* buf, uint16_t len);
    void readPage(uint32_t pageAddr);

  private:
    void lock(void);
    void unlock(void);
    void erasePage(uint32_t pageAddr);
};

#endif // FLASH_H
