#include "st7735.h"

volatile bool ST7735::IsDelayTimeout = false;

ST7735::ST7735() {
    __enable_irq();
    setCSflash();
    BasicTimer6::Instance().setST7735Callback(delayTimeout);
    init();
    ST7735_Init(1);
}

void ST7735::delayTimeout() { IsDelayTimeout = true; }
void ST7735::delayTimer_start(uint32_t ms) {
    IsDelayTimeout = false;
    BasicTimer6::Instance().ST7755_timer_start(ms);
}
void ST7735::delayTimer_stop() {
    IsDelayTimeout = false;
    BasicTimer6::Instance().ST7755_timer_stop();
}
void ST7735::ST7735::ST7735_Select() { resetCS(); }

void ST7735::ST7735_Unselect() { setCS(); }

void ST7735::ST7735_Reset() {
    setRes();
    delay_ms(5);
    resetRes();
    delay_ms(5);
    setRes();
}

void ST7735::ST7735_WriteCommand(uint8_t cmd) {
    resetDC();
    writeByte(cmd);
}

void ST7735::ST7735_WriteData(uint8_t* buff, int buff_size) {
    setDC();
    for (int i = 0; i < buff_size; i++) { writeByte(buff[i]); }
}

void ST7735::DisplayInit(const uint8_t* addr) {
    uint8_t numCommands, numArgs;
    uint16_t ms;
    numCommands = *addr++;
    while (numCommands--) {
        uint8_t cmd = *addr++;
        ST7735_WriteCommand(cmd);
        numArgs = *addr++;
        // If high bit set, delay follows args
        ms = numArgs & DELAY;
        numArgs &= ~DELAY;
        if (numArgs) {
            ST7735_WriteData((uint8_t*)addr, numArgs);
            addr += numArgs;
        }
        if (ms) {
            ms = *addr++;
            if (ms == 255) ms = 500;
            delay_ms(ms);
        }
    }
}

void ST7735::ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1,
                                     uint8_t y1) {
    // column address set
    ST7735_WriteCommand(ST7735_CASET);
    uint8_t data[] = {0x00, (uint8_t)(x0 + _xstart), 0x00,
                      (uint8_t)(x1 + _xstart)};
    ST7735_WriteData(data, sizeof(data));
    // row address set
    ST7735_WriteCommand(ST7735_RASET);
    data[1] = y0 + _ystart;
    data[3] = y1 + _ystart;
    ST7735_WriteData(data, sizeof(data));
    // write to RAM
    ST7735_WriteCommand(ST7735_RAMWR);
}

void ST7735::writeByte(uint8_t byte) {
    uint8_t i = 0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) {
        i++;
        if (i > 200) return;
    }
    SPI_I2S_SendData(SPI1, byte);
    i = 0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) {
        i++;
        if (i > 200) return;
    }
}

void ST7735::ST7735_Init(uint8_t rotation) {
    ST7735_Select();
    ST7735_Reset();
    DisplayInit(init_cmds1);
    DisplayInit(init_cmds2);
    DisplayInit(init_cmds3);
    _colstart = 0;
    _rowstart = 0;
    ST7735_SetRotation(rotation);
    ST7735_Unselect();
}

void ST7735::ST7735_SetRotation(uint8_t m) {
    uint8_t madctl = 0;
    rotation = m % 4; // can't be higher than 3
    switch (rotation) {
    case 0:
        madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_RGB;
        _height = ST7735_HEIGHT;
        _width = ST7735_WIDTH;
        _xstart = _colstart;
        _ystart = _rowstart;
        break;
    case 1:
        madctl = ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_RGB;
        _width = ST7735_HEIGHT;
        _height = ST7735_WIDTH;
        _ystart = _colstart;
        _xstart = _rowstart;
        break;
    case 2:
        madctl = ST7735_MADCTL_RGB;
        _height = ST7735_HEIGHT;
        _width = ST7735_WIDTH;
        _xstart = _colstart;
        _ystart = _rowstart;
        break;
    case 3:
        madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_RGB;
        _width = ST7735_HEIGHT;
        _height = ST7735_WIDTH;
        _ystart = _colstart;
        _xstart = _rowstart;
        break;
    }
    ST7735_Select();
    ST7735_WriteCommand(ST7735_MADCTL);
    ST7735_WriteData(&madctl, 1);
    ST7735_Unselect();
}

void ST7735::ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if ((x >= _width) || (y >= _height)) return;
    ST7735_Select();
    ST7735_SetAddressWindow(x, y, x + 1, y + 1);
    uint8_t data[] = {(uint8_t)(color >> 8), (uint8_t)(color & 0xFF)};
    ST7735_WriteData(data, sizeof(data));
    ST7735_Unselect();
}

void ST7735::ST7735_FillRectangle(uint16_t x, uint16_t y, uint16_t w,
                                  uint16_t h, uint16_t color) {
    if ((x >= _width) || (y >= _height)) return;
    if ((x + w - 1) >= _width) w = _width - x;
    if ((y + h - 1) >= _height) h = _height - y;

    ST7735_Select();
    ST7735_SetAddressWindow(x, y, x + w - 1, y + h - 1);

    uint8_t data[] = {(uint8_t)(color >> 8), (uint8_t)(color & 0xFF)};
    setDC();
    for (y = h; y > 0; y--) {
        for (x = w; x > 0; x--) {
            for(int i = 0; i < 2; i++) {
                writeByte(data[i]);
            }
        }
    }
    ST7735_Unselect();
}

void ST7735::ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                              const uint16_t* data) {
    if ((x >= _width) || (y >= _height)) return;
    if ((x + w - 1) >= _width) return;
    if ((y + h - 1) >= _height) return;

    ST7735_Select();
    ST7735_SetAddressWindow(x, y, x + w - 1, y + h - 1);
    ST7735_WriteData((uint8_t*)data, sizeof(uint16_t) * w * h);
    ST7735_Unselect();
}

void ST7735::ST7735_InvertColors(bool invert) {
    ST7735_Select();
    ST7735_WriteCommand(invert ? ST7735_INVON : ST7735_INVOFF);
    ST7735_Unselect();
}

void ST7735::init() {
    // A2-CSflash,
    RCC->APB2PCENR |= RCC_IOPAEN;
    GPIOA->CFGLR |= GPIO_CFGLR_MODE2;
    GPIOA->CFGLR &= ~GPIO_CFGLR_CNF2;
    // A1-CSflash,
    RCC->APB2PCENR |= RCC_IOPAEN;
    GPIOA->CFGLR |= GPIO_CFGLR_MODE1;
    GPIOA->CFGLR &= ~GPIO_CFGLR_CNF1;
    // A3-RESET,
    RCC->APB2PCENR |= RCC_IOPAEN;
    GPIOA->CFGLR |= GPIO_CFGLR_MODE3;
    GPIOA->CFGLR &= ~GPIO_CFGLR_CNF3;
    // A4-RS (CD),
    RCC->APB2PCENR |= RCC_IOPAEN;
    GPIOA->CFGLR |= GPIO_CFGLR_MODE4;
    GPIOA->CFGLR &= ~GPIO_CFGLR_CNF4;
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
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    //SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    // SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
}

void ST7735::delay_ms(uint8_t ms) {
    delayTimer_start(ms);
    while (!IsDelayTimeout) {}
}
