#include "lcdspi.h"
void LcdSpiInterface::writeByte(uint8_t byte) {
    volatile uint32_t i = 0;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) {
        i++;
        if (i > 20000) return;
    }
    SPI_I2S_SendData(SPI2, byte);
    i = 0;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) {
        i++;
        if (i > 20000) return;
    }
}

void LcdSpiInterface::fillScreen(uint16_t color) {
    setColumn(0, 320);
    setRow(0, 240);
    send_command(0x2C);
    for (volatile uint32_t i = 0; i < 320 * 240; i++) {
        // send_word(color);
        send_word(color);
    }
}
void LcdSpiInterface::fillHalfScreenHigh(uint16_t* color, uint8_t percent) {
    setColumn(0, 320);
    setRow(0, 120);
    send_command(0x2C);
    uint16_t batColor = 0;
    if (percent > 80 && percent <= 100) batColor = GREEN;
    if (percent > 40 && percent <= 80) batColor = YELLOW;
    if (percent <= 40) batColor = RED;
    // int startBatSymb = 320 - (percent/10);
    for (int i = 0; i < HALF_DISPLAY_MEMORY; i++) {
        // send_word(color);
        if ((i > 320 - percent / 10) && (i < 320)) {
            send_word((batColor));
        } else if ((i > 320 * 2 - percent / 10) && (i < 320 * 2)) {
            send_word((batColor));
        } else
            send_word(*(color + i));
    }
}
void LcdSpiInterface::fillHalfScreenLow(uint16_t* color) {
    setColumn(0, 320);
    setRow(120, 240);
    send_command(0x2C);
    for (volatile uint32_t i = 0; i < HALF_DISPLAY_MEMORY; i++) {
        // send_word(color);
        // send_word(*(color + i));
        send_data(*(color + i));
    }
}
void LcdSpiInterface::reset() {
    reset_on();
    delay(100 * 72000);
    reset_off();
    delay(100 * 72000);
}
void LcdSpiInterface::send_command(uint8_t com) {
    dc_command();
    // send_data(com);
    writeByte(com);
    dc_data();
}

void LcdSpiInterface::send_word(uint16_t data) {
#if (WORD_SIZE == BIT8)
    writeByte(data >> 8);
    writeByte(data & 0x00FF);
#elif (WORD_SIZE == BIT16)
    send_data(data);
#endif
}
void LcdSpiInterface::setColumn(uint16_t StartCol, uint16_t EndCol) {
    send_command(0x2A);  // Column Command address
    send_word(StartCol); //
    send_word(EndCol);   //
    // send_data(StartCol >> 8);
    // send_data(StartCol);
    // send_data(EndCol >> 8);
    // send_data(EndCol);
}
void LcdSpiInterface::setRow(uint16_t StartRow, uint16_t EndRow) {
    send_command(0x2B); // Column Command address
    send_word(StartRow);
    send_word(EndRow);
    // send_data(StartRow >> 8);
    // send_data(StartRow);
    // send_data(EndRow >> 8);
    // send_data(EndRow);
}
void LcdSpiInterface::setXY(int poX, int poY) {
    setColumn(poX, poX);
    setRow(poY, poY);
    send_command(0x2C);
}
void LcdSpiInterface::setPixel(int poX, int poY, int color) {
    setXY(poX, poY);
    send_word(color);
    // send_word(color);
}

//-----------------------------------------------------------------------------
//
LcdSpiIni::LcdSpiIni() {
    lcd_ini();
    fillScreen(YELLOW);
}

void LcdSpiIni::lcd_ini() {
    // E13-reset,
    RCC->APB2PCENR |= RCC_IOPEEN;
    GPIOE->CFGHR |= GPIO_CFGHR_MODE13;
    GPIOE->CFGHR &= ~GPIO_CFGHR_CNF13;
    // E14-DC,
    RCC->APB2PCENR |= RCC_IOPEEN;
    GPIOE->CFGHR |= GPIO_CFGHR_MODE14;
    GPIOE->CFGHR &= ~GPIO_CFGHR_CNF14;
    // B13-CLK, B15-MOSI
    RCC->APB2PCENR |= RCC_IOPBEN;
    GPIOB->CFGHR |= GPIO_CFGHR_MODE13 | GPIO_CFGHR_MODE15;
    GPIOB->CFGHR &= ~(GPIO_CFGHR_CNF13 | GPIO_CFGHR_CNF15);
    GPIOB->CFGHR |= (GPIO_CFGHR_CNF13_1 | GPIO_CFGHR_CNF15_1);

    // TODO: SPI init
    SPI_InitTypeDef SPI_InitStructure = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    RCC->APB1PCENR |= (1 << 14); // RCC_SPI2EN;

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    // SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    // SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    // SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);

    SPI_Cmd(SPI2, ENABLE);

    // tft_ini();
    tft_ini1();
}

void LcdSpiIni::tft_ini1() {
    reset();
    //---------------------------------------------------

    // Pixel Format Set
    send_command(0x3A);
    writeByte(0x55); // 16bit pixel

    send_command(0x2a); // column set
    writeByte(0x00);
    writeByte(0x00);
    writeByte(0x00);
    writeByte(0xEF);
    send_command(0x2b); // page address set
    writeByte(0x00);
    writeByte(0x00);
    writeByte(0x01);
    writeByte(0x3F);

    // sleep mode off
    send_command(0x11);
    delay(100 * 72000); // 100 ms
    // Display ON
    send_command(0x29); // display on
    // Memory Acsess Control - rotation
    //// 1-полубайт ориентация (через 2) - 2-ой цветовая схема (0 или 8)
    send_command(0x36);
    // send_data(0xf8);
    writeByte(TFT9341_ROTATION);
    delay(100 * 72000); // 100 ms
    // send_command(0x13);
}

void LcdSpiIni::tft_ini() {}

void Figure::drawRect(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2,
                      uint16_t color) {
    for (int i = y1; i < y2; i++) {
        for (int j = x1; j < x2; j++) { setPixel(j, i, color); }
    }
}

void Figure::drawPicture(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2,
                         uint16_t* arr) {
    for (volatile int i = y1; i < y2; i++) {
        for (volatile int j = x1; j < x2; j++) { setPixel(j, i, 
                //arr[i * 50 + j - 270])
                ((uint8_t)arr[i * 50 + j - x1 - y1] << 8) + 
                ((uint8_t)arr[i * 50 + j - x1 - y1] >> 8))
            ; }
    }
}

Font_interface::Font_interface() {
    Font24.Height = 24;
    Font24.Width = 17;
    lcdprop.BackColor = WHITE;
    lcdprop.TextColor = BLACK;
    lcdprop.pFont = &Font24;
}

void Font_interface::drawChar(uint16_t x, uint16_t y, uint8_t c) {
    uint32_t i = 0, j = 0;
    uint16_t height, width;
    uint8_t offset;
    uint8_t* c_t;
    uint8_t* pchar;
    uint32_t line = 0;
    height = lcdprop.pFont->Height;
    width = lcdprop.pFont->Width;
    offset = 8 * ((width + 7) / 8) - width;
    c_t = (uint8_t*)&(lcdprop.pFont->table[(c - ' ') * lcdprop.pFont->Height *
                                           ((lcdprop.pFont->Width + 7) / 8)]);
    for (i = 0; i < height; i++) {
        pchar = ((uint8_t*)c_t + (width + 7) / 8 * i);
        switch (((width + 7) / 8)) {
        case 1:
            line = pchar[0];
            break;
        case 2:
            line = (pchar[0] << 8) | pchar[1];
            break;
        case 3:
        default:
            line = (pchar[0] << 16) | (pchar[1] << 8) | pchar[2];
            break;
        }
        for (j = 0; j < width; j++) {
            if (line & (1 << (width - j + offset - 1))) {
                setPixel((x + j), y, lcdprop.TextColor);
            } else {
                setPixel((x + j), y, lcdprop.BackColor);
            }
        }
        y++;
    }
}
//-------------------------------------------------------------------
void Font_interface::drawString(uint16_t x, uint16_t y, const char* str) {
    while (*str) {
        drawChar(x, y, str[0]);
        x += lcdprop.pFont->Width;
        (void)*str++;
    }
}

uint32_t Font_interface::char_to_int(char* str, uint8_t size) {
    uint32_t x = 0;
    for (uint8_t i = 0; i < size; i++) {
        uint8_t dec = 0;
        if (str[i] == 48) { dec = 0; }
        if (str[i] == 49) { dec = 1; }
        if (str[i] == 50) { dec = 2; }
        if (str[i] == 51) { dec = 3; }
        if (str[i] == 52) { dec = 4; }
        if (str[i] == 53) { dec = 5; }
        if (str[i] == 54) { dec = 6; }
        if (str[i] == 55) { dec = 7; }
        if (str[i] == 56) { dec = 8; }
        if (str[i] == 57) { dec = 9; }
        x += dec * pow(10, size - i);
    }
    return x;
}

void Font_interface::intToChar(uint32_t x) {
    uint32_t y = x;
    uint8_t count = 0;
    while (y > 0) {
        y = y / 10;
        count++;
    } //считаем количество цифр
    y = x;
    arrSize = count;
    if (x == 0) {
        arrSize = 1;
        arr[arrSize - 1] = '0';
        arr[arrSize] = '\0';
        return;
    }
    for (uint8_t i = 0; i < arrSize; i++) {
        int x = y % 10;
        if (x == 0) { arr[arrSize - 1 - i] = '0'; }
        if (x == 1) { arr[arrSize - 1 - i] = '1'; }
        if (x == 2) { arr[arrSize - 1 - i] = '2'; }
        if (x == 3) { arr[arrSize - 1 - i] = '3'; }
        if (x == 4) { arr[arrSize - 1 - i] = '4'; }
        if (x == 5) { arr[arrSize - 1 - i] = '5'; }
        if (x == 6) { arr[arrSize - 1 - i] = '6'; }
        if (x == 7) { arr[arrSize - 1 - i] = '7'; }
        if (x == 8) { arr[arrSize - 1 - i] = '8'; }
        if (x == 9) { arr[arrSize - 1 - i] = '9'; }
        y = y / 10;
    }
    if (arrSize + 1 < 10) { arr[arrSize] = '\0'; }
}

void Font_interface::floatToChar(const float& val) {
    sprintf(arrFloat, "%.1f", val);
}

void Font_30x40::drawSymbol(uint16_t x, uint16_t y, const uint16_t* symbol) {
    setColumn(x, x + 30 - 1);
    setRow(y, y + 40 - 1);
    send_command(0x2C);
    for (int i = 0; i < 1200; i++) { send_word(*(symbol + i)); }
}
// void Font_30x40::drawString(uint16_t x, uint16_t y, bool red, const char*
// str) {
//    if (red) {
//        for (uint32_t i = 0; i < strlen(str); i++) {
//            if (str[i] == '0') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x30_R);
//            }
//            if (str[i] == '1') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x31_R);
//            }
//            if (str[i] == '2') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x32_R);
//            }
//            if (str[i] == '3') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x33_R);
//            }
//            if (str[i] == '4') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x34_R);
//            }
//            if (str[i] == '5') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x35_R);
//            }
//            if (str[i] == '6') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x36_R);
//            }
//            if (str[i] == '7') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x37_R);
//            }
//            if (str[i] == '8') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x38_R);
//            }
//            if (str[i] == '9') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x39_R);
//            }
//            if (str[i] == '.') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x2e_R);
//            }
//            if (str[i] == ' ') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x00);
//            }
//        }
//    } else {
//        for (uint32_t i = 0; i < strlen(str); i++) {
//            if (str[i] == '0') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x30_B);
//            }
//            if (str[i] == '1') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x31_B);
//            }
//            if (str[i] == '2') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x32_B);
//            }
//            if (str[i] == '3') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x33_B);
//            }
//            if (str[i] == '4') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x34_B);
//            }
//            if (str[i] == '5') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x35_B);
//            }
//            if (str[i] == '6') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x36_B);
//            }
//            if (str[i] == '7') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x37_B);
//            }
//            if (str[i] == '8') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x38_B);
//            }
//            if (str[i] == '9') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x39_B);
//            }
//            if (str[i] == '.') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x2e_B);
//            }
//            if (str[i] == ' ') {
//                drawSymbol(x + i * 30, y, image_data_temp_0x00);
//            }
//        }
//    }
//}
void Font_30x40::drawTemperature() {
    floatToChar(temperature);
    // if (temperature < 10) { drawString(70, 40, true, "    "); }
    // drawString(70, 40, true, arrFloat);
    // drawSymbol(240, 40, image_data_temp_0x43_R);
}
void Font_30x40::drawHumidity() {
    floatToChar(humidity);
    // if (humidity < 10) { drawString(70, 140, false, "    "); }
    // drawString(70, 140, false, arrFloat);
    // drawSymbol(240, 140, image_data_temp_0x25_B);
}
void Font_30x40::setTemperature(const float& temp) { temperature = temp; }
const float& Font_30x40::getTemperature() { return temperature; }
void Font_30x40::setHumidity(const float& hum) { humidity = hum; }
const float& Font_30x40::getHumidity() { return humidity; }
