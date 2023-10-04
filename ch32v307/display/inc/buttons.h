#ifndef BUTTONS_H
#define BUTTONS_H

#include "ch32v30x_adc.h"
#include "frwrapper.h"
#include "irq.h"
#include "main.h"
#include "math.h"

class Buttons : public Interruptable {
  public:
    Buttons();

    void interruptHandler() override;

    enum WhichBut : uint8_t
    {
        B_0,
        B_1,
        B_2,
        B_3
    };
    WhichBut currentWhichBut = WhichBut::B_0;

    enum Button : uint8_t
    {
        NONE = 0,
        B12 = 0x1d,//z//0x04, // a
        B14 = 0x19,//v//0x07, // d
        B13 = 0x1b,//x//0x16, // s
        B15 = 0x06,//c//0x1a, // w
        B1 = 0x04, //a //0x1e, //1
        B2 = 0x16, //s //0x1f, //2
        B3 = 0x07, //d //0x20, //3
        B4 = 0x02,//Lshift//0x21,//4
        B5 = 0x01,//Ctrl//0x22,//5
        B6 = 0x20,//Rshift//0x23,//6
        B7 = 0x2a,//Backspace//0x24,//7
        B8 = 0x2d,//=//0x25,//8
        B9 = 0x2e,//-//0x26,//9
        B0 = 0x1a,//w//0x27,//0
        Enter = 0x28,
        B10 = 0x29, // escape
        B11 = 0x2b, // tab
        Bright = 0x4f,
        Bleft = 0x50,
        Bup = 0x51,
        Bdown = 0x52,
    };
    Button currentBut = NONE;

    uint16_t averageV = 0;
    uint16_t averageH = 0;

    bool isAnyButtonPressed();
    uint8_t whichNumber();
    uint8_t pressed1 = 0;
    uint8_t pressed2 = 0;

    enum Mode : uint8_t
    {
        KEYBOARD,
        MOUSE
    };
    Mode currentMode = Mode::KEYBOARD;
    volatile bool isJoyB = false;
    bool currentModeOnceTime = false;

    bool isB0 = false;
    bool isB1 = false;
    bool isB2 = false;
    bool isB3 = false;
    bool isB4 = false;
    bool isB5 = false;
    bool isB6 = false;
    bool isB7 = false;
    bool isB8 = false;
    bool isB9 = false;
    bool isB10 = false;
    bool isB11 = false;
    bool isLeft = false;
    bool isRight = false;
    bool isUp = false;
    bool isDown = false;
    bool isEnter = false;
    bool isB13 = false;
    bool isB14 = false;
    bool isB12 = false;
    bool isB15 = false;

  private:
    uint16_t V = 0;
    uint16_t H = 0;
    void but_init();
    void joy_init();
    inline uint16_t getV();
    inline uint16_t getH();

    inline void setB0_out();
    inline void setB1_out();
    inline void setB2_out();
    inline void setB3_out();
    inline void resetB0_out();
    inline void resetB1_out();
    inline void resetB2_out();
    inline void resetB3_out();
    inline bool getB0_in();
    inline bool getB1_in();
    inline bool getB2_in();
    inline bool getB3_in();
    inline bool getEnter();
    inline bool getJoyB();
    uint8_t counterEnter = 0;
    uint16_t counterJoyB = 0;
    static constexpr uint8_t MAX_BUTTON_COUNTER = 10;

    static constexpr uint8_t MovAverLength = 128;
    static constexpr uint8_t MovAverGrade() { return log2(MovAverLength); }

    uint8_t Index_mov_aver_V = 0;
    uint8_t Index_mov_aver_H = 0;
    uint32_t MovAverSum_V = 0;
    uint32_t MovAverSum_H = 0;

    uint16_t arr_V[MovAverLength] = {0};
    uint16_t arr_H[MovAverLength] = {0};
    uint8_t counterADC = 0;
    uint32_t sumV = 0;
    uint32_t sumH = 0;
    static constexpr uint8_t MedianMiddle = 2;
    static constexpr uint8_t medianArrLength = 10;
    inline uint16_t moving_average_V(uint16_t val);
    inline uint16_t moving_average_H(uint16_t val);

    static constexpr uint8_t iB0 = 0;
    static constexpr uint8_t iB1 = 1;
    static constexpr uint8_t iB2 = 2;
    static constexpr uint8_t iB3 = 3;
    static constexpr uint8_t iB4 = 4;
    static constexpr uint8_t iB5 = 5;
    static constexpr uint8_t iB6 = 6;
    static constexpr uint8_t iB7 = 7;
    static constexpr uint8_t iB8 = 8;
    static constexpr uint8_t iB9 = 9;
    static constexpr uint8_t iB10 = 10; // escape
    static constexpr uint8_t iB11 = 11; // tab
    static constexpr uint8_t iB12 = 12; // a
    static constexpr uint8_t iB13 = 13; // s
    static constexpr uint8_t iB14 = 14; // d
    static constexpr uint8_t iB15 = 15; // w
    static constexpr uint8_t iEnter = 16;
    static constexpr uint8_t iBright = 17;
    static constexpr uint8_t iBleft = 18;
    static constexpr uint8_t iBup = 19;
    static constexpr uint8_t iBdown = 20;
    uint8_t butStateArr[21] = {0};
};

#endif // BUTTONS_H
