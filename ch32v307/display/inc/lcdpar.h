#ifndef LCDPAR_H
#define LCDPAR_H

#include "main.h"
#include "math.h"
#include "stdio.h"
#include "string.h"

#define BIT_8   0
#define BIT_16  1
#define WORD_SIZE BIT16


class LcdParInterface {
  public:
    void fillScreen(uint16_t color);
    //----------- reset C10 -----------------------
    inline void reset_on() { GPIOC->BSHR |= GPIO_BSHR_BR10; }  // low
    inline void reset_off() { GPIOC->BSHR |= GPIO_BSHR_BS10; } // high
    //----------  DC B13 (RS) --------------------------------
    inline void dc_data() { GPIOB->BSHR |= GPIO_BSHR_BS13; }    // High
    inline void dc_command() { GPIOB->BSHR |= GPIO_BSHR_BR13; } // Low
    //----------  WR B14 -----------------------------------
    inline void wr_on() { GPIOB->BSHR |= GPIO_BSHR_BR14; }   // low
    inline void wr_idle() { GPIOB->BSHR |= GPIO_BSHR_BS14; } // high
    //----------  RD B15 -----------------------------------
    inline void rd_on() { GPIOB->BSHR |= GPIO_BSHR_BR14; }   // low
    inline void rd_idle() { GPIOB->BSHR |= GPIO_BSHR_BS14; } // high
    //----------  CS B12 -----------------------------------
    inline void cs_on() { GPIOB->BSHR |= GPIO_BSHR_BR12; }   // low
    inline void cs_idle() { GPIOB->BSHR |= GPIO_BSHR_BS12; } // high
    inline void wr_strobe() {
        wr_on();
        wr_idle();
    }

    void reset();
    void write(uint8_t byte);
    void send_command(uint8_t com);
    void send_data(uint8_t data);
    void sendByte(uint8_t byte);
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

    inline void delay(uint32_t x) {
        while (x > 0) { x--; }
    }
};

class LcdParIni : LcdParInterface {
  public:
    LcdParIni();

  private:
    void lcd_ini();
    void tft_ini();
};

//*********** Класс обработки цифр и букв ********************
class Font_interface : public LcdParInterface {
  public:
    uint32_t char_to_int(char* str, uint8_t size);
    char arr[20];
    char arrFloat[20]{0};
    volatile uint8_t arrSize = 0;

    void intToChar(uint32_t x);
    void floatToChar(const float& val);
};

class Font_30x40 : public Font_interface {
  public:
    void drawSymbol(uint16_t x, uint16_t y, const uint16_t* symbol);
    //void drawString(uint16_t x, uint16_t y, bool red, const char* str);
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

#endif // LCDPAR_H
