#ifndef LCDSPI_H
#define LCDSPI_H

#include "ch32v30x_rcc.h"
#include "ch32v30x_spi.h"
#include "fonts.h"
#include "main.h"
#include "math.h"
#include "stdio.h"
#include "string.h"

#define BIT8 0
#define BIT16 1
#define WORD_SIZE BIT8

class LcdSpiInterface {
  public:
    static constexpr uint8_t TFT9341_MADCTL_MY = 0x80;
    static constexpr uint8_t TFT9341_MADCTL_MX = 0x40;
    static constexpr uint8_t TFT9341_MADCTL_MV = 0x20;
    static constexpr uint8_t TFT9341_MADCTL_ML = 0x10;
    static constexpr uint8_t TFT9341_MADCTL_RGB = 0x00;
    static constexpr uint8_t TFT9341_MADCTL_BGR = 0x08;
    static constexpr uint8_t TFT9341_MADCTL_MH = 0x04;
    static constexpr uint8_t TFT9341_ROTATION =
        (TFT9341_MADCTL_MV | TFT9341_MADCTL_BGR);

    static constexpr uint16_t HALF_DISPLAY_MEMORY = 320 * 120;
    enum Colors : uint16_t
    {
        BLACK = 0x0000,
        BLUE = 0x001F,
        GREEN = 0x07E0,
        CYAN = 0x07FF,
        RED = 0xF800,
        MAGENTA = 0xF81F,
        YELLOW = 0xFFE0,
        WHITE = 0xFFFF
    };

    void fillScreen(uint16_t color);
    void fillHalfScreenHigh(uint16_t* color, uint8_t percent);
    void fillHalfScreenLow(uint16_t* color);
    //----------- reset E13 -----------------------
    inline void reset_on() { GPIOE->BSHR |= GPIO_BSHR_BR13; }  // low
    inline void reset_off() { GPIOE->BSHR |= GPIO_BSHR_BS13; } // high
    //----------  CS B12 -----------------------------------
    // inline void cs_on() { GPIOB->BSHR |= GPIO_BSHR_BR12; }   // low
    // inline void cs_idle() { GPIOB->BSHR |= GPIO_BSHR_BS12; } // high
    //----------  DC E14 (RS) --------------------------------
    inline void dc_data() { GPIOE->BSHR |= GPIO_BSHR_BS14; }    // High
    inline void dc_command() { GPIOE->BSHR |= GPIO_BSHR_BR14; } // Low
    //----------  WR B14 -----------------------------------
    // inline void wr_on() { GPIOB->BSHR |= GPIO_BSHR_BS14; }  // high
    // inline void wr_off() { GPIOB->BSHR |= GPIO_BSHR_BR14; } // low
    //----------  RD B15 -----------------------------------
    // inline void rd_on() { GPIOB->BSHR |= GPIO_BSHR_BR15; }   // low
    // inline void rd_idle() { GPIOB->BSHR |= GPIO_BSHR_BS15; } // high
    // inline void wr_strobe() {
    //    wr_off();
    //    for (volatile int i = 0; i < 100; i++) {}
    //    wr_on();
    //}

    void reset();
    void send_command(uint8_t com);
    void writeByte(uint8_t byte);
    // void send_data(uint16_t data);
    void send_data(uint16_t data) { send_word(data); }
    void send_word(uint16_t data);
    void setColumn(uint16_t StartCol, uint16_t EndCol);
    void setRow(uint16_t StartRow, uint16_t EndRow);
    void setXY(int poX, int poY);
    void setPixel(int poX, int poY, int color);

    inline void swap(uint16_t x1, uint16_t x2)
        __attribute__((__always_inline__)) {
        if (x2 > x1) {
            uint16_t z = x2;
            x2 = x1;
            x1 = z;
        }
    }
    inline void checkXYswap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
        __attribute__((__always_inline__)) {
        swap(x1, x2);
        swap(y1, y2);
    }

    inline void delay(volatile uint32_t x) {
        while (x > 0) { x--; }
    }
};

class LcdSpiIni : public LcdSpiInterface {
  public:
    LcdSpiIni();

  private:
    void lcd_ini();
    void tft_ini();
    void tft_ini1();
};

class Figure : public LcdSpiIni {
  public:
    //uint16_t buff[HALF_DISPLAY_MEMORY] = {0};
    void drawRect(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2,
                  uint16_t color);
    void drawPicture(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2,
                     uint16_t* arr);
};

//*********** Класс обработки цифр и букв ********************
extern const uint8_t Font24_Table[];
class Font_interface : public LcdSpiInterface {
  public:
    Font_interface();
    struct LCD_DrawPropTypeDef {
        uint16_t TextColor = WHITE;
        uint16_t BackColor = BLACK;
        sFONT* pFont = &Font24;
    };
    LCD_DrawPropTypeDef lcdprop;

    uint32_t char_to_int(char* str, uint8_t size);
    char arr[20];
    char arrFloat[20]{0};
    volatile uint8_t arrSize = 0;

    void drawChar(uint16_t x, uint16_t y, uint8_t c);
    void drawString(uint16_t x, uint16_t y, const char* str);

    void intToChar(uint32_t x);
    void floatToChar(const float& val);
};

class Font_30x40 : public Font_interface {
  public:
    void drawSymbol(uint16_t x, uint16_t y, const uint16_t* symbol);
    // void drawString(uint16_t x, uint16_t y, bool red, const char* str);
    void drawTemperature();
    void drawHumidity();
    void setTemperature(const float& temp);
    const float& getTemperature();
    void setHumidity(const float& hum);
    const float& getHumidity();

  private:
    float temperature = 0;
    float humidity = 0;
};

#endif // LCDSPI_H
