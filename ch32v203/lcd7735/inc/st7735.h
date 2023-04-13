#ifndef ST7735_H
#define ST7735_H

#include "basic_timer.h"
#include "ch32v20x_spi.h"
#include "main.h"

/****** TFT DEFINES ******/
//#define ST7735_IS_160X80 1
//#define ST7735_IS_128X128 1
#define ST7735_IS_160X128 1
#define ST7735_WIDTH 128
#define ST7735_HEIGHT 160

#define DELAY 0x80

#define ST7735_MADCTL_MY 0x80
#define ST7735_MADCTL_MX 0x40
#define ST7735_MADCTL_MV 0x20
#define ST7735_MADCTL_ML 0x10
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH 0x04

#define ST7735_NOP 0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID 0x04
#define ST7735_RDDST 0x09

#define ST7735_SLPIN 0x10
#define ST7735_SLPOUT 0x11
#define ST7735_PTLON 0x12
#define ST7735_NORON 0x13

#define ST7735_INVOFF 0x20
#define ST7735_INVON 0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON 0x29
#define ST7735_CASET 0x2A
#define ST7735_RASET 0x2B
#define ST7735_RAMWR 0x2C
#define ST7735_RAMRD 0x2E

#define ST7735_PTLAR 0x30
#define ST7735_COLMOD 0x3A
#define ST7735_MADCTL 0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR 0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1 0xC0
#define ST7735_PWCTR2 0xC1
#define ST7735_PWCTR3 0xC2
#define ST7735_PWCTR4 0xC3
#define ST7735_PWCTR5 0xC4
#define ST7735_VMCTR1 0xC5

#define ST7735_RDID1 0xDA
#define ST7735_RDID2 0xDB
#define ST7735_RDID3 0xDC
#define ST7735_RDID4 0xDD

#define ST7735_PWCTR6 0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define color565(r, g, b)                                                      \
    (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

class ST7735 {
  public:
    // CS to GND
    inline void setCS() { GPIOA->BSHR |= GPIO_BSHR_BS1; }
    inline void resetCS() { GPIOA->BSHR |= GPIO_BSHR_BR1; }
    // CS W25q
    inline void setCSflash() { GPIOA->BSHR |= GPIO_BSHR_BS2; }
    inline void resetCSflash() { GPIOA->BSHR |= GPIO_BSHR_BR2; }
    // RESET A3
    inline void setRes() { GPIOA->BSHR |= GPIO_BSHR_BS3; }
    inline void resetRes() { GPIOA->BSHR |= GPIO_BSHR_BR3; }
    // RS A4
    inline void setDC() { GPIOA->BSHR |= GPIO_BSHR_BS4; }
    inline void resetDC() { GPIOA->BSHR |= GPIO_BSHR_BR4; }
    ST7735();
    void init();
    void writeByte(uint8_t byte);

    ///////////////////////////////////////////////////////
    // call before initializing any SPI devices
    void ST7735_Unselect();
    void ST7735_Select();

    void ST7735_Init(uint8_t rotation);
    void ST7735_SetRotation(uint8_t m);
    void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
    void ST7735_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                              uint16_t color);
    void ST7735_FillScreen(uint16_t color);
    void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          const uint16_t* data);
    void ST7735_InvertColors(bool invert);
    
    void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1,
                                 uint8_t y1);

    void ST7735_Reset();
    void ST7735_WriteCommand(uint8_t cmd);
    void ST7735_WriteData(uint8_t* buff, int buff_size);
    void DisplayInit(const uint8_t* addr);
    
    int16_t _width;    ///< Display width as modified by current rotation
    int16_t _height;   ///< Display height as modified by current rotation
    int16_t cursor_x;  ///< x location to start print()ing text
    int16_t cursor_y;  ///< y location to start print()ing text
    uint8_t rotation;  ///< Display rotation (0 thru 3)
    uint8_t _colstart; ///< Some displays need this changed to offset
    uint8_t _rowstart; ///< Some displays need this changed to offset
    uint8_t _xstart = 0;
    uint8_t _ystart = 0;

    void delay_ms(uint8_t ms);
    static volatile bool IsDelayTimeout;
    static void delayTimeout();
    void delayTimer_start(uint32_t ms);
    void delayTimer_stop();

    static constexpr uint8_t init_cmds1[] =
        {       // Init for 7735R, part 1 (red or green tab)
            15, // 15 commands in list:
            ST7735_SWRESET,
            DELAY, //  1: Software reset, 0 args, w/delay
            150,   //     150 ms delay
            ST7735_SLPOUT,
            DELAY, //  2: Out of sleep mode, 0 args, w/delay
            255,   //     500 ms delay
            ST7735_FRMCTR1,
            3, //  3: Frame rate ctrl - normal mode, 3 args:
            0x01,
            0x2C,
            0x2D, //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
            ST7735_FRMCTR2,
            3, //  4: Frame rate control - idle mode, 3 args:
            0x01,
            0x2C,
            0x2D, //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
            ST7735_FRMCTR3,
            6, //  5: Frame rate ctrl - partial mode, 6 args:
            0x01,
            0x2C,
            0x2D, //     Dot inversion mode
            0x01,
            0x2C,
            0x2D, //     Line inversion mode
            ST7735_INVCTR,
            1,    //  6: Display inversion ctrl, 1 arg, no delay:
            0x07, //     No inversion
            ST7735_PWCTR1,
            3, //  7: Power control, 3 args, no delay:
            0xA2,
            0x02, //     -4.6V
            0x84, //     AUTO mode
            ST7735_PWCTR2,
            1,    //  8: Power control, 1 arg, no delay:
            0xC5, //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
            ST7735_PWCTR3,
            2,    //  9: Power control, 2 args, no delay:
            0x0A, //     Opamp current small
            0x00, //     Boost frequency
            ST7735_PWCTR4,
            2,    // 10: Power control, 2 args, no delay:
            0x8A, //     BCLK/2, Opamp current small & Medium low
            0x2A,
            ST7735_PWCTR5,
            2, // 11: Power control, 2 args, no delay:
            0x8A,
            0xEE,
            ST7735_VMCTR1,
            1, // 12: Power control, 1 arg, no delay:
            0x0E,
            ST7735_INVOFF,
            0, // 13: Don't invert display, no args, no delay
            ST7735_COLMOD,
            1,     // 15: set color mode, 1 arg, no delay:
            0x05}, //     16-bit color

        init_cmds2[] =
            {      // Init for 7735R, part 2 (1.44" display)
                2, //  2 commands in list:
                ST7735_CASET,
                4, //  1: Column addr set, 4 args, no delay:
                0x00,
                0x00, //     XSTART = 0
                0x00,
                0x7F, //     XEND = 127
                ST7735_RASET,
                4, //  2: Row addr set, 4 args, no delay:
                0x00,
                0x00, //     XSTART = 0
                0x00,
                0x7F}, //     XEND = 127

        init_cmds3[] = { // Init for 7735R, part 3 (red or green tab)
            4,           //  4 commands in list:
            ST7735_GMCTRP1,
            16, //  1: Magical unicorn dust, 16 args, no delay:
            0x02,
            0x1c,
            0x07,
            0x12,
            0x37,
            0x32,
            0x29,
            0x2d,
            0x29,
            0x25,
            0x2B,
            0x39,
            0x00,
            0x01,
            0x03,
            0x10,
            ST7735_GMCTRN1,
            16, //  2: Sparkles and rainbows, 16 args, no delay:
            0x03,
            0x1d,
            0x07,
            0x06,
            0x2E,
            0x2C,
            0x29,
            0x2D,
            0x2E,
            0x2E,
            0x37,
            0x3F,
            0x00,
            0x00,
            0x02,
            0x10,
            ST7735_NORON,
            DELAY, //  3: Normal display on, no args, w/delay
            10,    //     10 ms delay
            ST7735_DISPON,
            DELAY, //  4: Main screen turn on, no args w/delay
            100};  //     100 ms delay
};

#endif // ST7735_H
