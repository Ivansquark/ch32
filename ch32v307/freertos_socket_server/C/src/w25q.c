#include "w25q.h"
uint8_t SPI_FLASH_BUF[FLASH_BUF_SIZE];
uint16_t AddressHeadIndexHtml;
uint16_t AddressIndexHtml;
uint16_t AddressHeadIco;
uint16_t AddressIco;

uint16_t SizeHeadIndexHtml;
uint16_t SizeIndexHtml;
uint16_t SizeHeadIco;
uint16_t SizeIco;
uint16_t SizeHeadCss;
uint16_t SizeCss;
uint16_t SizeHeadJs;
uint16_t SizeJs;
uint16_t SizeHeadContentStream;
uint16_t SizeContentStream;

void W25_writeHtml()
{
    W25_write((uint8_t*)headIndexHtml, AddressHeadIndexHtml,
              strlen(headIndexHtml));
    W25_write((uint8_t*)indexHtml, AddressIndexHtml, strlen(indexHtml));
    W25_write((uint8_t*)headIco, AddressHeadIco, strlen(headIco));
    W25_write((uint8_t*)ico, AddressIco, sizeof(ico));
}

void W25_setCS() { GPIOA->BSHR |= GPIO_BSHR_BR2; }
void W25_resetCS() { GPIOA->BSHR |= GPIO_BSHR_BS2; }


uint16_t W25_readWriteByte(uint8_t TxData)
{
    uint8_t i = 0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) {
        i++;
        if (i > 200)
            return 0;
    }
    SPI_I2S_SendData(SPI1, TxData);
    i = 0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) {
        i++;
        if (i > 200)
            return 0;
    }
    return SPI_I2S_ReceiveData(SPI1);
}

uint16_t W25_readSR(void)
{
    uint8_t byte = 0;
    W25_setCS();
    W25_readWriteByte(W25X_ReadStatusReg);
    byte = W25_readWriteByte(0Xff);
    W25_resetCS();
    return byte;
}
void W25_writeSR(uint8_t sr)
{
    W25_setCS();
    W25_readWriteByte(W25X_WriteStatusReg);
    W25_readWriteByte(sr);
    W25_resetCS();
}

void W25_wait_Busy(void)
{
    while ((W25_readSR() & 0x01) == 0x01) {}
}

void W25_write_Enable(void)
{
    W25_setCS();
    W25_readWriteByte(W25X_WriteEnable);
    W25_resetCS();
}

void W25_write_Disable(void)
{
    W25_setCS();
    W25_readWriteByte(W25X_WriteDisable);
    W25_resetCS();
}

uint16_t W25_readID(void)
{
    uint16_t Temp = 0;
    // setCS();
    Temp = W25_readWriteByte(W25X_ManufactDeviceID);
    Temp = W25_readWriteByte(0x00);
    Temp = W25_readWriteByte(0x00);
    Temp = W25_readWriteByte(0x00);
    Temp |= W25_readWriteByte(0xFF) << 8;
    Temp |= W25_readWriteByte(0xFF);
    // resetCS();
    return Temp;
}

void W25_erase_Sector(uint32_t Dst_Addr)
{
    Dst_Addr *= 4096;
    W25_write_Enable();
    W25_wait_Busy();
    W25_setCS();
    W25_readWriteByte(W25X_SectorErase);
    W25_readWriteByte((uint8_t)((Dst_Addr) >> 16));
    W25_readWriteByte((uint8_t)((Dst_Addr) >> 8));
    W25_readWriteByte((uint8_t)Dst_Addr);
    W25_resetCS();
    W25_wait_Busy();
}

void W25q_read(u8* pBuffer, u32 ReadAddr, u16 size)
{
    uint16_t i = 0;
    W25_setCS();
    W25_readWriteByte(W25X_ReadData);
    W25_delay(100);
    W25_readWriteByte((u8)((ReadAddr) >> 16));
    W25_delay(100);
    W25_readWriteByte((u8)((ReadAddr) >> 8));
    W25_delay(100);
    W25_readWriteByte((u8)ReadAddr);
    W25_delay(100);
    for (i = 0; i < size; i++) {
        pBuffer[i] = W25_readWriteByte(0XFF);
        W25_delay(100);
    }
    W25_resetCS();
}

void W25_write_Page(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t size)
{
    W25_write_Enable();
    W25_setCS();
    W25_readWriteByte(W25X_PageProgram);
    W25_delay(100);
    W25_readWriteByte((uint8_t)((WriteAddr) >> 16));
    W25_delay(100);
    W25_readWriteByte((uint8_t)((WriteAddr) >> 8));
    W25_delay(100);
    W25_readWriteByte((uint8_t)WriteAddr);
    W25_delay(100);
    for (uint16_t i = 0; i < size; i++) {
        W25_readWriteByte(pBuffer[i]);
        W25_delay(100);
    }
    W25_resetCS();
    W25_wait_Busy();
}

void W25_write_NoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t size)
{
    uint16_t pageremain;
    pageremain = 256 - WriteAddr % 256;
    if (size <= pageremain)
        pageremain = size;
    while (1) {
        W25_write_Page(pBuffer, WriteAddr, pageremain);
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

void W25_write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t size)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    secpos = WriteAddr / 4096;
    secoff = WriteAddr % 4096;
    secremain = 4096 - secoff;
    if (size <= secremain)
        secremain = size;
    while (1) {
        W25_read(SPI_FLASH_BUF, secpos * 4096, 4096);
        for (i = 0; i < secremain; i++) {
            if (SPI_FLASH_BUF[secoff + i] != 0XFF)
                break;
        }
        if (i < secremain) {
            W25_erase_Sector(secpos);
            for (i = 0; i < secremain; i++) {
                SPI_FLASH_BUF[i + secoff] = pBuffer[i];
            }
            W25_write_NoCheck(SPI_FLASH_BUF, secpos * 4096, 4096);
        } else {
            W25_write_NoCheck(pBuffer, WriteAddr, secremain);
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

void W25_erase_Chip(void)
{
    W25_write_Enable();
    W25_wait_Busy();
    W25_setCS();
    W25_readWriteByte(W25X_ChipErase);
    W25_resetCS();
    W25_wait_Busy();
}

void W25_powerDown(void)
{
    W25_setCS();
    W25_readWriteByte(W25X_PowerDown);
    W25_resetCS();
    // Delay_Us(3);
}

void W25_WAKEUP(void)
{
    W25_setCS();
    W25_readWriteByte(W25X_ReleasePowerDown);
    W25_resetCS();
    // Delay_Us(3);
}

void W25_reset()
{
    W25_setCS();
    W25_readWriteByte(W25X_EnableReset);
    W25_readWriteByte(W25X_Reset);
    W25_resetCS();
}

void W25_init()
{
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
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    // SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);

    AddressHeadIndexHtml = 0x00;
    AddressIndexHtml = strlen(headIndexHtml);
    AddressHeadIco = AddressIndexHtml + strlen(indexHtml);
    AddressIco = AddressHeadIco + strlen(headIco);

    SizeHeadIndexHtml = strlen(headIndexHtml);
    SizeIndexHtml = strlen(indexHtml);
    SizeHeadIco = strlen(headIco);
    SizeIco = sizeof(ico);
    SizeHeadCss = strlen(headCss);
    SizeCss = strlen(css);
    SizeHeadJs = strlen(headJs);
    SizeJs = strlen(js);
    SizeHeadContentStream = strlen(headContentStream);
    SizeContentStream = 2;
}

void W25_delay(volatile uint32_t val)
{
    while (val--) {}
}


// HTTP info:
//
const char* headIndexHtml = "HTTP/1.1 200 OK\n"
                                   "Content-Type: text/html; charset=UTF-8\n"
                                   "Content-Length: 1062\r\n\r\n";
const char* indexHtml =
    "<!DOCTYPE html>"
    "<html lang=\"en\">"
    "   <head>"
    "       <meta charset=\"UTF-8\" />"
    "       <meta name=\"viewport\" content=\"width=device-width, "
    "       initial-scale=1.0\" />"
    "       <link rel=\"stylesheet\" href=\"style.css\" />"
    "       <title>Progress Steps</title>"
    "   </head>"
    "   <body>"
    "       <div class=\"container\">"
    "       <div class=\"progress-container\">"
    "       <div class=\"progress\" id=\"progress\"></div>"
    "       <div class=\"circle active\">1</div>"
    "       <div class=\"circle\">2</div>"
    "       <div class=\"circle\">3</div>"
    "       </div>"
    "           <button class=\"btn\" id=\"prev\" disabled>Prev</button>"
    "           <button class=\"btn\" id=\"next\">Next</button>"
    "           <button class=\"btn\" id=\"start\">START</button>"
    "       <div style=\"background-color:#8ebf42\">"
    "           <!-- Paragraph -->"
    "           <p>"
    "               <span id=\"info\" style=\"color:#1c87c9\">"
    "                   OPA"
    "               </span>"
    "           </p>"
    "       </div>"
    "       <p></p>"
    "       <p></p>"
    "       <canvas id=\"myChart\" width=\"700\" height=\"400\"></canvas>"
    "       <p></p>"
    "       <p></p>"
    "       <script src=\"script.js\"></script>"
    "   </body>"
    "</html>";
const char* headIco = "HTTP/1.1 200 OK\r\n"
                             "Content-Type: image/png; charset=UTF-8\r\n"
                             "Content-Length: 539\r\n\r\n";

unsigned char ico[539] = {
    0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x32, 0x2a, 0x02, 0x00, 0x01, 0x00,
    0x01, 0x00, 0x05, 0x02, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x89, 0x50,
    0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x48,
    0x44, 0x52, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x2a, 0x08, 0x06,
    0x00, 0x00, 0x00, 0xf1, 0xba, 0x08, 0x47, 0x00, 0x00, 0x01, 0xcc, 0x49,
    0x44, 0x41, 0x54, 0x68, 0x81, 0xed, 0x59, 0x5b, 0x6e, 0xc4, 0x20, 0x0c,
    0x34, 0x55, 0x2f, 0xd1, 0xdb, 0xf6, 0xa8, 0xbd, 0x49, 0xf6, 0xa7, 0x91,
    0x5c, 0xc7, 0x8f, 0xf1, 0x03, 0x65, 0xbb, 0xda, 0x91, 0x22, 0x04, 0x21,
    0xc0, 0x30, 0x60, 0x6c, 0xb2, 0xbe, 0xbe, 0x7f, 0xe8, 0x15, 0xf0, 0x71,
    0xf7, 0x00, 0xa6, 0xf0, 0x26, 0xf2, 0x6c, 0xf8, 0xdc, 0xd0, 0xe6, 0x41,
    0x44, 0xeb, 0x37, 0x95, 0x58, 0xec, 0xfd, 0x28, 0xa6, 0x15, 0x39, 0x44,
    0xca, 0xb1, 0x94, 0xf7, 0x5a, 0xbd, 0x12, 0x26, 0x88, 0x1c, 0xec, 0x91,
    0x90, 0x33, 0xcf, 0xf3, 0xa3, 0x64, 0xba, 0x44, 0xf8, 0x20, 0x16, 0x4b,
    0x97, 0xc8, 0x93, 0x52, 0x8f, 0xd7, 0x6d, 0x93, 0xe9, 0x10, 0x91, 0x24,
    0xac, 0x19, 0x46, 0x95, 0xf2, 0x94, 0x0d, 0x51, 0x25, 0x72, 0x6e, 0x58,
    0x3e, 0xf3, 0x7c, 0x50, 0x5e, 0x9e, 0x97, 0x6b, 0x8a, 0x96, 0x50, 0x21,
    0xe2, 0xcd, 0x98, 0x36, 0x98, 0x68, 0x86, 0x35, 0x32, 0x69, 0x55, 0xb2,
    0x44, 0xac, 0x0e, 0x3c, 0x93, 0x9a, 0x9d, 0xe9, 0x92, 0x32, 0x99, 0x73,
    0x04, 0x59, 0xfb, 0xc8, 0x77, 0x1c, 0x7c, 0x6f, 0x68, 0xe5, 0x30, 0xa9,
    0x8c, 0x22, 0x96, 0x55, 0xe2, 0x40, 0x36, 0xb6, 0x55, 0x5f, 0xb3, 0x76,
    0xf0, 0x12, 0xcb, 0x2a, 0x62, 0x59, 0x1b, 0xcf, 0x6a, 0x51, 0x50, 0xee,
    0xf5, 0x67, 0xf5, 0x7b, 0x01, 0x4a, 0x04, 0x69, 0xcc, 0x52, 0x48, 0x92,
    0xcd, 0x28, 0x09, 0x2f, 0x2f, 0x64, 0x69, 0x75, 0x4e, 0xe0, 0x8c, 0x89,
    0x8d, 0xdc, 0x1a, 0x17, 0x08, 0x11, 0xeb, 0x84, 0xb6, 0x3a, 0xb7, 0xbe,
    0xef, 0xd4, 0x0b, 0xfb, 0xc9, 0x28, 0x52, 0x05, 0x4a, 0x36, 0x7b, 0x3e,
    0xfd, 0x41, 0x46, 0x91, 0x5d, 0xef, 0x89, 0x30, 0x13, 0xed, 0xa2, 0xa2,
    0xc8, 0xdd, 0x79, 0x15, 0x88, 0xd5, 0xd2, 0x0e, 0xad, 0x4a, 0xe7, 0xde,
    0xf7, 0xb2, 0x7c, 0x8b, 0x8b, 0x62, 0xb9, 0xea, 0xe4, 0x94, 0x67, 0xeb,
    0x65, 0xdb, 0xb9, 0xa0, 0x62, 0xb5, 0xee, 0x48, 0x47, 0xf7, 0x48, 0xd5,
    0xce, 0x77, 0x7d, 0x32, 0xe8, 0x50, 0xfc, 0x2f, 0xe7, 0x48, 0x88, 0x8c,
    0xd3, 0x58, 0x75, 0x08, 0xd1, 0x0d, 0x1c, 0x19, 0x08, 0x17, 0xa8, 0xaf,
    0x15, 0x2d, 0x21, 0xeb, 0x0a, 0x28, 0x13, 0xf6, 0xb6, 0xe2, 0x99, 0xa9,
    0x98, 0xfd, 0xcc, 0x7b, 0xae, 0xbe, 0x75, 0x21, 0x91, 0x89, 0xf5, 0x4d,
    0x64, 0xe3, 0x11, 0x6d, 0xb9, 0xa0, 0x26, 0x55, 0xe6, 0x35, 0xa2, 0x5b,
    0xcf, 0x11, 0x09, 0xcb, 0x9b, 0xed, 0xc4, 0x21, 0x96, 0xa9, 0xdd, 0x12,
    0x21, 0x9e, 0x0d, 0xa3, 0x33, 0x8f, 0x0c, 0xc6, 0x0a, 0xd6, 0xa2, 0xef,
    0x2e, 0xe8, 0x28, 0x62, 0xdd, 0x6b, 0x59, 0xf5, 0x91, 0x76, 0x4e, 0x57,
    0x28, 0x6d, 0x8a, 0x2b, 0x44, 0xb8, 0x2a, 0x08, 0x99, 0x48, 0x29, 0x54,
    0x59, 0x17, 0x13, 0xb7, 0xf1, 0x5a, 0xcc, 0x1e, 0xed, 0x9d, 0xb3, 0x6c,
    0xec, 0x92, 0xae, 0x63, 0x7e, 0xb9, 0x89, 0xf5, 0x4c, 0xa7, 0xa7, 0xc8,
    0xd8, 0x4d, 0xe3, 0xd4, 0xff, 0x91, 0xca, 0x61, 0x18, 0x5d, 0x48, 0xa4,
    0x30, 0xf9, 0x7f, 0x44, 0xbb, 0x97, 0x92, 0xef, 0xbd, 0xb4, 0x85, 0x1d,
    0x7f, 0xac, 0x88, 0x70, 0x32, 0x63, 0x78, 0x99, 0x7f, 0x88, 0x6f, 0x22,
    0xcf, 0x86, 0x07, 0xcc, 0x3c, 0x8e, 0xfc, 0xbe, 0xd8, 0x35, 0x28, 0x00,
    0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82};

const char* headCss = "HTTP/1.1 200 OK\n"
                             "Content-Type: text/css; charset=UTF-8\n"
                             "Content-Length: 1522\r\n\r\n";

const char* css = ":root {"
                         "   --line-border-fill: #3498db;"
                         "   --line-border-empty: #e0e0e0;"
                         "}"
                         "* {"
                         "box-sizing: border-box;"
                         "}"
                         "body {"
                         "   font-family: \"Muli\", sans-serif;"
                         "   background-color: #f6f7fb;"
                         "   display: flex;"
                         "   align-items: center;"
                         "   justify-content: center;"
                         "   height: 100vh;"
                         "   margin: 10px;"
                         "}"
                         ".container {"
                         "   text-align: center;"
                         "}"
                         ".progress-container {"
                         "   display: flex;"
                         "   justify-content: space-between;"
                         "   position: relative;"
                         "   margin-bottom: 30px;"
                         "   max-width: 100%;"
                         "   width: 350px;"
                         "}"
                         ".progress-container::before {"
                         "   content: \"\";"
                         "   background-color: var(--line-border-empty);"
                         "   position: absolute;"
                         "   top: 50%;"
                         "   left: 0;"
                         "   transform: translateY(-50%);"
                         "   height: 4px;"
                         "   width: 100%;"
                         "   z-index: -1;"
                         "}"
                         ".progress {"
                         "   background-color: var(--line-border-fill);"
                         "   position: absolute;"
                         "   top: 50%;"
                         "   left: 0;"
                         "   transform: translateY(-50%);"
                         "   height: 4px;"
                         "   width: 0%;"
                         "   z-index: -1;"
                         "   transition: 0.4s ease;"
                         "}"
                         ".circle {"
                         "   background-color: #fff;"
                         "   color: #999;"
                         "   border-radius: 50%;"
                         "   height: 30px;"
                         "   width: 30px;"
                         "   display: flex;"
                         "   align-items: center;"
                         "   justify-content: center;"
                         "   border: 3px solid var(--line-border-empty);"
                         "   transition: 0.4s ease;"
                         "}"
                         ".circle.active {"
                         "   border-color: var(--line-border-fill);"
                         "}"
                         ".btn {"
                         "   background-color: var(--line-border-fill);"
                         "   color: #fff;"
                         "   border: 0;"
                         "   border-radius: 6px;"
                         "   cursor: pointer;"
                         "   font-family: inherit;"
                         "   padding: 8px 30px;"
                         "   margin: 5px;"
                         "   font-size: 14px;"
                         "}"
                         ".btn:active {"
                         "   transform: scale(0.98);"
                         "}"
                         ".btn:focus {"
                         "   outline: 0;"
                         "}"
                         ".btn:disabled {"
                         "   background-color: var(--line-border-empty);"
                         "   cursor: not-allowed;"
                         "}";

const char* headJs = "HTTP/1.1 200 OK\n"
                            "Content-Type: text/javascript; charset=UTF-8\n"
                            "Content-Length: 2829\r\n\r\n";

const char* js =
    "const progress = document.getElementById(\"progress\");\n"
    "const prev = document.getElementById(\"prev\");\n"
    "const next = document.getElementById(\"next\");\n"
    "const start = document.getElementById(\"start\");\n"
    "const circles = document.querySelectorAll(\".circle\");\n"
    "let currentActive = 1;\n"
    "var xhr = new(XMLHttpRequest);\n"
    "next.addEventListener(\"click\", () => {\n"
    "   currentActive++;\n"
    "   if (currentActive > circles.length) currentActive = "
    "circles.length;\n"
    "   update();\n"
    "});\n"
    "prev.addEventListener(\"click\", () => {\n"
    "   currentActive--;\n"
    "   if (currentActive < 1) currentActive = 1;\n"
    "   update();\n"
    "});\n"
    "start.addEventListener(\"click\", startstring);\n"
    "const update = () => {\n"
    "   circles.forEach((circle, index) => {\n"
    "       if (index < currentActive) {\n"
    "           circle.classList.add(\"active\");\n"
    "       } else {\n"
    "           circle.classList.remove(\"active\");\n"
    "       }\n"
    "   });\n"
    "   const actives = document.querySelectorAll(\".active\");\n"
    "   progress.style.width =\n"
    "   ((actives.length - 1) / (circles.length - 1)) * 100 + \"%\";\n"
    "   if (currentActive === 1) {\n"
    "       prev.disabled = true;\n"
    "       xhr.open(\"GET\", \"off.bin?r=\" + Math.random(), true);\n"
    "   } else if (currentActive === circles.length) {\n"
    "       next.disabled = true;\n"
    "       xhr.open(\"GET\", \"off.bin?r=\" + Math.random(), true);\n"
    "   } else {\n"
    "       prev.disabled = false;\n"
    "       next.disabled = false;\n"
    "       xhr.open(\"GET\", \"on.bin?r=\" + Math.random(), true);\n"
    "   }\n"
    "   xhr.responseType = \"arraybuffer\";\n"
    "   xhr.onload = function (oEvent) {\n"
    "       var val = new Uint8Array(this.response);\n"
    "       console.log(val);\n"
    "       if(val[0])\n"
    "           document.getElementById(\"info\").textContent=\"ON\";\n"
    "       else\n"
    "           document.getElementById(\"info\").textContent=\"OFF\";\n"
    "   }\n"
    "   xhr.send(null);\n"
    "};\n"
    "var idTimer1;\n"
    "var temp = false;\n"
    "//var ctx = document.getElementById(\"myChart\");\n"
    "function Timer1() {\n"
    "   xhr.open(\"GET\", \"content.bin?r=\" + Math.random(), true);\n"
    "   xhr.responseType = \"arraybuffer\";\n"
    "   xhr.onload = function (oEvent) {\n"
    "       var val = new Uint16Array(this.response);\n"
    "       myChart.data = val[0];\n"
    "       draw(val);\n"
    "   }\n"
    "   xhr.send(null);\n"
    "   idTimer1 = setTimeout(\"Timer1()\", 10);\n"
    "}\n"
    "var IsStarted = false;\n"
    "function startstring() {\n"
    "   if(!IsStarted) {\n"
    "       IsStarted = true;\n"
    "       document.getElementById('start').innerHTML = 'STOP';\n"
    "       Timer1();\n"
    "   } else {\n"
    "       IsStarted = false;\n"
    "       clearTimeout(idTimer1);\n"
    "       document.getElementById('start').innerHTML = 'START';\n"
    "   }\n"
    "}\n"
    "function draw(i) {\n"
    "   var V = 3*i/4096\n"
    "   var canvas = document.getElementById('myChart');\n"
    "   if (canvas.getContext) {\n"
    "       var ctx = canvas.getContext('2d');\n"
    "       ctx.clearRect(0, 0, canvas.width, canvas.height);\n"
    "       ctx.fillStyle = \"red\";\n"
    "       ctx.fillRect(0, 0, canvas.width*V/3, 50);\n"
    "       ctx.fillStyle = \"blue\";\n"
    "       ctx.font = \"italic 30pt Arial\";\n"
    "       ctx.fillText(V.toFixed(3) + \"V\", 35, 150);\n"
    "   }\n"
    "}";

const char* headContentStream =
    "HTTP/1.1 200 OK\n"
    "Content-Type: application/octet-stream\n"
    "Content-Length: 2\r\n\r\n";

