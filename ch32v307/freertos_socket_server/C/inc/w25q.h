#ifndef W25Q_H
#define W25Q_H

#include "ch32v30x_spi.h"
#include "main.h"
#include <string.h>

//@brief Write data to flash.(no need Erase)
void W25_write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t size);
void W25_read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t size);
void W25_erase_Sector(uint32_t Dst_Addr);
uint16_t W25_readID(void);
uint16_t W25_readSR(void);
void W25_reset();
void W25_WAKEUP(void);

/* Winbond SPIFalsh ID */
#define W25Q80 0XEF13
#define W25Q16 0XEF14
#define W25Q32 0XEF15
#define W25Q64 0XEF16
#define W25Q128 0XEF17

void W25_init();
void W25_delay(volatile uint32_t val);
uint16_t W25_readWriteByte(uint8_t TxData);
void W25_writeSR(uint8_t sr);
void W25_wait_Busy(void);
void W25_write_Enable(void);
void W25_write_Disable(void);
void W25_write_Page(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t size);
void W25_write_NoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t size);
void W25_erase_Chip(void);
void W25_powerDown(void);

void W25_setCS();
void W25_resetCS();

/* Winbond SPIFalsh Instruction List */
#define W25X_WriteEnable 0x06
#define W25X_WriteDisable 0x04
#define W25X_ReadStatusReg 0x05
#define W25X_WriteStatusReg 0x01
#define W25X_ReadData 0x03
#define W25X_FastReadData 0x0B
#define W25X_FastReadDual 0x3B
#define W25X_PageProgram 0x02
#define W25X_BlockErase 0xD8
#define W25X_SectorErase 0x20
#define W25X_ChipErase 0xC7
#define W25X_PowerDown 0xB9
#define W25X_ReleasePowerDown 0xAB
#define W25X_EnableReset 0x66
#define W25X_Reset 0x99
#define W25X_DeviceID 0xAB
#define W25X_ManufactDeviceID 0x90
#define W25X_JedecDeviceID 0x9F

#define FLASH_BUF_SIZE 256
extern uint8_t SPI_FLASH_BUF[FLASH_BUF_SIZE];
extern const char* headIndexHtml;
extern const char* indexHtml;
extern const char* headIco;
extern unsigned char ico[539];
extern const char* headCss;
extern const char* css;
extern const char* headJs;
extern const char* js;
extern const char* headContentStream;
// Http server info

// int constexpr length(const char* str) {
//    return *str ? 1 + length(str + 1) : 0;
//}

void W25_writeHtml();
extern uint16_t AddressHeadIndexHtml;
extern uint16_t AddressIndexHtml;
extern uint16_t AddressHeadIco;
extern uint16_t AddressIco;

extern uint16_t SizeHeadIndexHtml;
extern uint16_t SizeIndexHtml;
extern uint16_t SizeHeadIco;
extern uint16_t SizeIco;
extern uint16_t SizeHeadCss;
extern uint16_t SizeCss;
extern uint16_t SizeHeadJs;
extern uint16_t SizeJs;
extern uint16_t SizeHeadContentStream;
extern uint16_t SizeContentStream;

#endif // W25Q_H
