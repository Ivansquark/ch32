#ifndef W25Q_H
#define W25Q_H

#include "ch32v30x_spi.h"
#include "main.h"

class W25q {
  public:
    W25q();
    //@brief Write data to flash.(no need Erase)
    void write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t size);
    void read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t size);
    void erase_Sector(uint32_t Dst_Addr);
    uint16_t readID(void);
    uint16_t readSR(void);
    void reset();
    void WAKEUP(void);

    /* Winbond SPIFalsh ID */
    static constexpr uint16_t W25Q80 = 0XEF13;
    static constexpr uint16_t W25Q16 = 0XEF14;
    static constexpr uint16_t W25Q32 = 0XEF15;
    static constexpr uint16_t W25Q64 = 0XEF16;
    static constexpr uint16_t W25Q128 = 0XEF17;
  private:
    void init();
    void delay(volatile uint32_t val);
    uint16_t readWriteByte(uint8_t TxData);
    void writeSR(uint8_t sr);
    void wait_Busy(void);
    void write_Enable(void);
    void write_Disable(void);
    void write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t size);
    void write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t size);
    void erase_Chip(void);
    void powerDown(void);

    inline void setCS() { GPIOA->BSHR |= GPIO_BSHR_BR2; }
    inline void resetCS() { GPIOA->BSHR |= GPIO_BSHR_BS2; }

    /* Winbond SPIFalsh Instruction List */
    static constexpr uint16_t W25X_WriteEnable = 0x06;
    static constexpr uint16_t W25X_WriteDisable = 0x04;
    static constexpr uint16_t W25X_ReadStatusReg = 0x05;
    static constexpr uint16_t W25X_WriteStatusReg = 0x01;
    static constexpr uint16_t W25X_ReadData = 0x03;
    static constexpr uint16_t W25X_FastReadData = 0x0B;
    static constexpr uint16_t W25X_FastReadDual = 0x3B;
    static constexpr uint16_t W25X_PageProgram = 0x02;
    static constexpr uint16_t W25X_BlockErase = 0xD8;
    static constexpr uint16_t W25X_SectorErase = 0x20;
    static constexpr uint16_t W25X_ChipErase = 0xC7;
    static constexpr uint16_t W25X_PowerDown = 0xB9;
    static constexpr uint16_t W25X_ReleasePowerDown = 0xAB;
    static constexpr uint16_t W25X_EnableReset = 0x66;
    static constexpr uint16_t W25X_Reset = 0x99;
    static constexpr uint16_t W25X_DeviceID = 0xAB;
    static constexpr uint16_t W25X_ManufactDeviceID = 0x90;
    static constexpr uint16_t W25X_JedecDeviceID = 0x9F;

    uint8_t SPI_FLASH_BUF[4096] = {0};
};

#endif // W25Q_H
