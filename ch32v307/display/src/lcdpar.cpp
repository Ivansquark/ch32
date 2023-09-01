#include "lcdpar.h"
void LcdParInterface::fillScreenSequence(uint16_t* color, uint16_t len,
                                         uint16_t num) {
    bool exceed = false;
    
    setColumn(0, 256);
    setRow(0, 0);
    send_command(0x2C);
    for (volatile uint32_t i = 0; i < len; i++) { send_word(*(color + i)); }
}
void LcdParInterface::fillScreen(uint16_t color) {
    setColumn(0, 320);
    setRow(0, 240);
    send_command(0x2C);
    for (volatile uint32_t i = 0; i < 320 * 240; i++) {
        // send_word(color);
        send_word(color);
    }
}
void LcdParInterface::fillHalfScreenHigh(uint16_t* color) {
    setColumn(0, 320);
    setRow(0, 120);
    send_command(0x2C);
    for (volatile uint32_t i = 0; i < HALF_DISPLAY_MEMORY; i++) {
        // send_word(color);
        send_word(*(color + i));
    }
}
void LcdParInterface::fillHalfScreenLow(uint16_t* color) {
    setColumn(0, 320);
    setRow(120, 240);
    send_command(0x2C);
    for (volatile uint32_t i = 0; i < HALF_DISPLAY_MEMORY; i++) {
        // send_word(color);
        //send_word(*(color + i));
        send_data(*(color + i));
    }
}
void LcdParInterface::reset() {
    cs_idle();
    wr_on();
    rd_idle();
    reset_on();
    delay(100 * 72000);
    reset_off();
    cs_on();
    delay(100 * 72000);
}
void LcdParInterface::send_command(uint8_t com) {
    dc_command();
    send_data(com);
    dc_data();
}

void LcdParInterface::sendByte(uint8_t byte) {
    wr_off();
    GPIOD->BSHR = byte;
    wr_on();
}
void LcdParInterface::send_word(uint16_t data) {
#if (WORD_SIZE == BIT8)
    sendByte(data >> 8);
    sendByte(data & 0x00FF);
#elif (WORD_SIZE == BIT16)
    send_data(data);
#endif
}
void LcdParInterface::setColumn(uint16_t StartCol, uint16_t EndCol) {
    send_command(0x2A); // Column Command address
    send_data(StartCol >> 8);
    send_data(StartCol);
    send_data(EndCol >> 8);
    send_data(EndCol);
}
void LcdParInterface::setRow(uint16_t StartRow, uint16_t EndRow) {
    send_command(0x2B); // Column Command address
    send_data(StartRow >> 8);
    send_data(StartRow);
    send_data(EndRow >> 8);
    send_data(EndRow);
}
void LcdParInterface::setXY(int poX, int poY) {
    setColumn(poX, poX);
    setRow(poY, poY);
    send_command(0x2C);
}
void LcdParInterface::setPixel(int poX, int poY, int color) {
    setXY(poX, poY);
    send_word(color);
    // send_word(color);
}

//-----------------------------------------------------------------------------
//
LcdParIni::LcdParIni() {
    lcd_ini();
    fillScreen(GREEN);
}

void LcdParIni::lcd_ini() {
    // CS-B12 RS-B13 WR-B14 RD-B15 p/p
    RCC->APB2PCENR |= RCC_IOPBEN;
    GPIOB->CFGHR |= GPIO_CFGHR_MODE12 | GPIO_CFGHR_MODE13 | GPIO_CFGHR_MODE14 |
                    GPIO_CFGHR_MODE15;
    GPIOB->CFGHR &= ~(GPIO_CFGHR_CNF12 | GPIO_CFGHR_CNF13 | GPIO_CFGHR_CNF14 |
                      GPIO_CFGHR_CNF15);
    // RST-C10 PEN-C11 T_CS-C12 p/p
    RCC->APB2PCENR |= RCC_IOPCEN;
    GPIOC->CFGHR |= GPIO_CFGHR_MODE10 | GPIO_CFGHR_MODE11 | GPIO_CFGHR_MODE12;
    GPIOC->CFGHR &= ~(GPIO_CFGHR_CNF10 | GPIO_CFGHR_CNF11 | GPIO_CFGHR_CNF12);
    // D0-D15 p/p 0:0:1:1
    RCC->APB2PCENR |= RCC_IOPDEN;
    GPIOD->CFGLR = 0x33333333;
    GPIOD->CFGHR = 0x33333333;
    GPIOD->OUTDR = 0;
    // tft_ini();
    tft_ini1();
}

void LcdParIni::tft_ini1() {
    rd_idle();
    reset();
    //---------------------------------------------------

    // Pixel Format Set
    send_command(0x3A);
    send_data(0x55); // 16bit

    send_command(0x2a); // column set
    send_data(0x00);
    send_data(0x00);
    send_data(0x00);
    send_data(0xEF);
    send_command(0x2b); // page address set
    send_data(0x00);
    send_data(0x00);
    send_data(0x01);
    send_data(0x3F);

    // sleep mode off
    send_command(0x11);
    delay(100 * 72000); // 100 ms
    // Display ON
    send_command(0x29); // display on
    // Memory Acsess Control - rotation
    //// 1-полубайт ориентация (через 2) - 2-ой цветовая схема (0 или 8)
    send_command(0x36);
    // send_data(0xf8);
    send_data(TFT9341_ROTATION);
    delay(100 * 72000); // 100 ms
}

void LcdParIni::tft_ini() {
    rd_idle();
    reset();
    //---------------------------------------------------
    send_command(0xEF);
    send_data(0x03);
    send_data(0x80);
    send_data(0x02);
    // Power Control A
    send_command(0xCB);
    send_data(0x39);
    send_data(0x2C);
    send_data(0x00);
    send_data(0x34);
    send_data(0x02);
    // Power Control B
    send_command(0xCF);
    send_data(0x00);
    send_data(0xC1);
    send_data(0x30);
    // Power on Sequence control
    send_command(0xED);
    send_data(0x64);
    send_data(0x03);
    send_data(0x12);
    send_data(0x81);
    // Driver timing control A
    send_command(0xE8);
    send_data(0x85);
    send_data(0x00);
    send_data(0x78);
    // Driver timing control B
    send_command(0xEA);
    send_data(0x00);
    send_data(0x00);
    // Pump ratio control
    send_command(0xF7);
    send_data(0x20);
    // Power Control,VRH[5:0]
    send_command(0xC0); // Power contro
    send_data(0x23);    // VRH[5:0
    // Power Control,SAP[2:0];BT[3:0]
    send_command(0xC1);
    send_data(0x10); // SAP[2:0];BT[3:0
    // VCOM Control 1
    send_command(0xC5); // VCM control
    send_data(0x3E);    // Contrast
    send_data(0x28);
    // VCOM Control 2
    send_command(0xC7); // VCM control2
    send_data(0x86);
    send_command(0x37);
    send_data(0x00);
    // Memory Acsess Control - rotation
    //// 1-полубайт ориентация (через 2) - 2-ой цветовая схема (0 или 8)
    send_command(0x36);
    send_data(0xf8);
    // Pixel Format Set
    send_command(0x3A);
    send_data(0x55); // 16bit
    // Frame Rratio Control, Standard RGB Color
    send_command(0xB1);
    send_data(0x00);
    send_data(0x18);
    send_command(0xB6); // Display Function Control
    send_data(0x08);
    send_data(0x82);
    send_data(0x27);
    // Enable 3G (пока не знаю что это за режим)
    // send_command(0xF2);
    // send_data(0x00); //не включаем
    // Gamma set
    send_command(0x26);
    send_data(0x01); // Gamma Curve (G2.2) (Кривая цветовой гаммы)
    // Positive Gamma  Correction
    send_command(0xE0);
    send_data(0x0F);
    send_data(0x31);
    send_data(0x2B);
    send_data(0x0C);
    send_data(0x0E);
    send_data(0x08);
    send_data(0x4E);
    send_data(0xF1);
    send_data(0x37);
    send_data(0x07);
    send_data(0x10);
    send_data(0x03);
    send_data(0x0E);
    send_data(0x09);
    send_data(0x00);
    // Negative Gamma  Correction
    send_command(0xE1);
    send_data(0x00);
    send_data(0x0E);
    send_data(0x14);
    send_data(0x03);
    send_data(0x11);
    send_data(0x07);
    send_data(0x31);
    send_data(0xC1);
    send_data(0x48);
    send_data(0x08);
    send_data(0x0F);
    send_data(0x0C);
    send_data(0x31);
    send_data(0x36);
    send_data(0x0F);

    send_command(0x2B); // page set
    send_data(0x00);
    send_data(0x00);
    send_data(0x00);
    send_data(0xEF);

    send_command(0x2A); // column set
    send_data(0x00);
    send_data(0x00);
    send_data(0x01);
    send_data(0x3F);

    send_command(0x34); // tearing effect off

    send_command(0xB7); // entry mode set
    send_data(0x07);

    send_command(0x11); //Выйдем из спящего режима
    delay(2 * 72000);   // 100 ms
    // Display ON
    send_command(0x29); // display on
    // send_data(TFT9341_ROTATION);
    delay(2 * 72000); // 100 ms
    send_command(0x13);
}

void Figure::drawRect(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2,
                      uint16_t color) {
    for (int i = y1; i < y2; i++) {
        for (int j = x1; j < x2; j++) { setPixel(j, i, color); }
    }
}

uint32_t Font_interface::char_to_int(char* str, uint8_t size) {
    uint32_t x;
    for (uint8_t i = 0; i < size; i++) {
        uint8_t dec;
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
