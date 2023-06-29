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

    uint16_t averageV = 0;
    uint16_t averageH = 0;

    enum Button : uint8_t
    {
        NONE,
        B0,
        B1,
        B2,
        B3,
        B4,
        B5,
        B6,
        B7,
        B8,
        B9,
        B10,
        B11,
        B12,
        B13,
        B14,
        B15,
        Enter
    };
    Button currentBut = NONE;

    // -- Stack to check last pressed but if several pressed ------------------
    struct Stack {
        Button arr[16] = {Buttons::NONE};
        Button* sp = arr;
        Button* sp0 = arr;
        uint8_t size() { return (sp - sp0); }

        bool isButInStack(Button but) {
            bool state = false;
            for (int i = 0; i < size(); i++) {
                if (arr[i] == but) { state = true; }
            }
            return state;
        }

        void push(Button but) {
            if (size() > 16) return;
            arr[sp - sp0] = but;
            sp++;
        }
        void pop() {
            if (size() > 0) sp--;
        }
        void clear() {
            sp = sp0;
            arr[0] = NONE;
        }
    };
    Stack stack;

    bool B0_once = false;
    bool B1_once = false;
    bool B2_once = false;
    bool B3_once = false;
    bool B4_once = false;
    bool B5_once = false;
    bool B6_once = false;
    bool B7_once = false;
    bool B8_once = false;
    bool B9_once = false;
    bool B10_once = false;
    bool B11_once = false;
    bool B12_once = false;
    bool B13_once = false;
    bool B14_once = false;
    bool B15_once = false;
    bool Enter_once = false;

    bool getButtonState(Button butNum);
    bool isAnyButtonPressed();

  private:
    uint16_t V = 0;
    uint16_t H = 0;
    void but_init();
    void joy_init();
    inline uint16_t getV();
    inline uint16_t getH();

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
    bool isB12 = false;
    bool isB13 = false;
    bool isB14 = false;
    bool isB15 = false;
    bool isEnter = false;

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
    uint8_t counterEnter = 0;
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
    uint16_t moving_average_V(uint16_t val);
    uint16_t moving_average_H(uint16_t val);
};

#endif // BUTTONS_H
