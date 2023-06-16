#ifndef BUTTONS_H
#define BUTTONS_H

#include "main.h"
#include "frwrapper.h"
#include "ch32v30x_adc.h"
#include "irq.h"
#include "math.h"

class Buttons : FR_OS , Interruptable {
    public:
        Buttons(uint16_t stackSize = 16);

        void runTask(void* pvParameters) override;
        void interruptHandler() override;

        enum WhichBut : uint8_t {
            B_0,
            B_1,
            B_2,
            B_3
        };
        WhichBut currentWhichBut = WhichBut::B_0;

    uint16_t averageV = 0;
    uint16_t averageH = 0;

    enum Button : uint8_t {
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
        B16,
        Enter
    };

    bool getButtonState(Button butNum);

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

#endif //BUTTONS_H
