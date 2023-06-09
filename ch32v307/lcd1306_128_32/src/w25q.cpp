#include "w25q.h"
W25q* W25q::pThis = nullptr;
W25q::W25q() {
    pThis = this;
    init();
}

uint16_t W25q::readWriteByte(uint8_t TxData) {
    uint8_t i = 0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) {
        i++;
        if (i > 200) return 0;
    }
    SPI_I2S_SendData(SPI1, TxData);
    i = 0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) {
        i++;
        if (i > 200) return 0;
    }
    return SPI_I2S_ReceiveData(SPI1);
}

uint16_t W25q::readSR(void) {
    uint8_t byte = 0;
    setCS();
    readWriteByte(W25X_ReadStatusReg);
    byte = readWriteByte(0Xff);
    resetCS();
    return byte;
}
void W25q::writeSR(uint8_t sr) {
    setCS();
    readWriteByte(W25X_WriteStatusReg);
    readWriteByte(sr);
    resetCS();
}

void W25q::wait_Busy(void) {
    while ((readSR() & 0x01) == 0x01) {}
}

void W25q::write_Enable(void) {
    setCS();
    readWriteByte(W25X_WriteEnable);
    resetCS();
}

void W25q::write_Disable(void) {
    setCS();
    readWriteByte(W25X_WriteDisable);
    resetCS();
}

uint16_t W25q::readID(void) {
    uint16_t Temp = 0;
    // setCS();
    Temp = readWriteByte(W25X_ManufactDeviceID);
    Temp = readWriteByte(0x00);
    Temp = readWriteByte(0x00);
    Temp = readWriteByte(0x00);
    Temp |= readWriteByte(0xFF) << 8;
    Temp |= readWriteByte(0xFF);
    // resetCS();
    return Temp;
}

void W25q::erase_Sector(uint32_t Dst_Addr) {
    Dst_Addr *= 4096;
    write_Enable();
    wait_Busy();
    setCS();
    readWriteByte(W25X_SectorErase);
    readWriteByte((uint8_t)((Dst_Addr) >> 16));
    readWriteByte((uint8_t)((Dst_Addr) >> 8));
    readWriteByte((uint8_t)Dst_Addr);
    resetCS();
    wait_Busy();
}

void W25q::read(u8* pBuffer, u32 ReadAddr, u16 size) {
    uint16_t i = 0;
    setCS();
    readWriteByte(W25X_ReadData);
    delay(1);
    readWriteByte((u8)((ReadAddr) >> 16));
    delay(1);
    readWriteByte((u8)((ReadAddr) >> 8));
    delay(1);
    readWriteByte((u8)ReadAddr);
    delay(1);
    for (i = 0; i < size; i++) {
        pBuffer[i] = readWriteByte(0XFF);
        delay(1);
    }
    resetCS();
}

void W25q::write_Page(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t size) {
    write_Enable();
    setCS();
    readWriteByte(W25X_PageProgram);
    delay(100);
    readWriteByte((uint8_t)((WriteAddr) >> 16));
    delay(100);
    readWriteByte((uint8_t)((WriteAddr) >> 8));
    delay(100);
    readWriteByte((uint8_t)WriteAddr);
    delay(100);
    for (uint16_t i = 0; i < size; i++) {
        readWriteByte(pBuffer[i]);
        delay(100);
    }
    resetCS();
    wait_Busy();
}

void W25q::write_NoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t size) {
    uint16_t pageremain;
    pageremain = 256 - WriteAddr % 256;
    if (size <= pageremain) pageremain = size;
    while (1) {
        write_Page(pBuffer, WriteAddr, pageremain);
        if (size == pageremain) {
            break;
        } else {
            pBuffer += pageremain;
            WriteAddr += pageremain;
            size -= pageremain;
            if (size > 256)
                pageremain = 256;
            else
                pageremain = size;
        }
    }
}

void W25q::write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t size) {
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    secpos = WriteAddr / 4096;
    secoff = WriteAddr % 4096;
    secremain = 4096 - secoff;
    if (size <= secremain) secremain = size;
    while (1) {
        read(SPI_FLASH_BUF, secpos * 4096, 4096);
        for (i = 0; i < secremain; i++) {
            if (SPI_FLASH_BUF[secoff + i] != 0XFF) break;
        }
        if (i < secremain) {
            erase_Sector(secpos);
            for (i = 0; i < secremain; i++) {
                SPI_FLASH_BUF[i + secoff] = pBuffer[i];
            }
            write_NoCheck(SPI_FLASH_BUF, secpos * 4096, 4096);
        } else {
            write_NoCheck(pBuffer, WriteAddr, secremain);
        }
        if (size == secremain) {
            break;
        } else {
            secpos++;
            secoff = 0;
            pBuffer += secremain;
            WriteAddr += secremain;
            size -= secremain;
            if (size > 4096) {
                secremain = 4096;
            } else {
                secremain = size;
            }
        }
    }
}

void W25q::erase_Chip(void) {
    write_Enable();
    wait_Busy();
    setCS();
    readWriteByte(W25X_ChipErase);
    resetCS();
    wait_Busy();
}

void W25q::powerDown(void) {
    setCS();
    readWriteByte(W25X_PowerDown);
    resetCS();
    // Delay_Us(3);
}

void W25q::WAKEUP(void) {
    setCS();
    readWriteByte(W25X_ReleasePowerDown);
    resetCS();
    // Delay_Us(3);
}

void W25q::reset() {
    setCS();
    readWriteByte(W25X_EnableReset);
    readWriteByte(W25X_Reset);
    resetCS();
}

void W25q::init() {
    // A2-CS,
    RCC->APB2PCENR |= RCC_IOPAEN;
    GPIOA->CFGLR |= GPIO_CFGLR_MODE2;
    GPIOA->CFGLR &= ~GPIO_CFGLR_CNF2;
    // A5-CLK, A7-MOSI
    GPIOA->CFGLR |= GPIO_CFGLR_MODE5 | GPIO_CFGLR_MODE7;
    GPIOA->CFGLR &= ~(GPIO_CFGLR_CNF5 | GPIO_CFGLR_CNF7);
    GPIOA->CFGLR |= (GPIO_CFGLR_CNF5_1 | GPIO_CFGLR_CNF7_1);
    // A6-MISO CNF_0:1-input floating
    GPIOA->CFGLR &= ~GPIO_CFGLR_MODE6;
    GPIOA->CFGLR |= GPIO_CFGLR_CNF6_0;
    GPIOA->CFGLR &= ~GPIO_CFGLR_CNF6_1;

    // TODO: SPI init
    SPI_InitTypeDef SPI_InitStructure = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    RCC->APB2PCENR |= RCC_SPI1EN;

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    // SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    // SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    //SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    // SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
}

void W25q::delay(volatile uint32_t val) {
    while (val--) {}
}
