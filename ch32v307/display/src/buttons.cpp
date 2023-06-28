#include "buttons.h"

Buttons::Buttons() {
    but_init();
    joy_init();
}

bool Buttons::isAnyButtonPressed() {
    bool state = false;
    if (isB0) {
        state = true;
    } else if (isB1) {
        state = true;
    } else if (isB2) {
        state = true;
    } else if (isB3) {
        state = true;
    } else if (isB4) {
        state = true;
    } else if (isB5) {
        state = true;
    } else if (isB6) {
        state = true;
    } else if (isB7) {
        state = true;
    } else if (isB8) {
        state = true;
    } else if (isB9) {
        state = true;
    } else if (isB10) {
        state = true;
    } else if (isB11) {
        state = true;
    } else if (isB12) {
        state = true;
    } else if (isB13) {
        state = true;
    } else if (isB14) {
        state = true;
    } else if (isB15) {
        state = true;
    } else if (isEnter) {
        state = true;
    }
    if(!state) {
        currentBut = NONE;
    }
    return state;
}

bool Buttons::getButtonState(Button butNum) {
    bool state = false;
    switch (butNum) {
    case B0:
        state = isB0;
        break;
    case B1:
        state = isB1;
        break;
    case B2:
        state = isB2;
        break;
    case B3:
        state = isB3;
        break;
    case B4:
        state = isB4;
        break;
    case B5:
        state = isB5;
        break;
    case B6:
        state = isB6;
        break;
    case B7:
        state = isB7;
        break;
    case B8:
        state = isB8;
        break;
    case B9:
        state = isB9;
        break;
    case B10:
        state = isB10;
        break;
    case B11:
        state = isB11;
        break;
    case B12:
        state = isB12;
        break;
    case B13:
        state = isB13;
        break;
    case B14:
        state = isB14;
        break;
    case B15:
        state = isB15;
        break;
    case Enter:
        state = isEnter;
        break;
    default:
        break;
    }
    return state;
}

void Buttons::interruptHandler() {
    // once in ms
    // TODO: check buttons 4 times strobes
    switch (currentWhichBut) {
    case B_0:
        if (getB0_in()) {
            isB3 = true;
            currentBut = B3;
        } else {
            isB3 = false;
        }
        if (getB1_in()) {
            isB7 = true;
            currentBut = B7;
        } else {
            isB7 = false;
        }
        if (getB2_in()) {
            isB11 = true;
            currentBut = B11;
        } else {
            isB11 = false;
        }
        if (getB3_in()) {
            isB15 = true;
            currentBut = B15;
        } else {
            isB15 = false;
        }
        resetB0_out();
        resetB1_out();
        resetB2_out();
        resetB3_out();
        setB0_out();
        // for(volatile int i = 0; i< 10000; i++) {}
        currentWhichBut = WhichBut::B_1;
        break;
    case B_1:
        if (getB0_in()) {
            isB0 = true;
            currentBut = B0;
        } else {
            isB0 = false;
        }
        if (getB1_in()) {
            isB4 = true;
            currentBut = B4;
        } else {
            isB4 = false;
        }
        if (getB2_in()) {
            isB8 = true;
            currentBut = B8;
        } else {
            isB8 = false;
        }
        if (getB3_in()) {
            isB12 = true;
            currentBut = B12;
        } else {
            isB12 = false;
        }
        resetB0_out();
        resetB1_out();
        resetB2_out();
        resetB3_out();
        setB1_out();
        // for(volatile int i = 0; i< 10000; i++) {}
        currentWhichBut = WhichBut::B_2;
        break;
    case B_2:
        if (getB0_in()) {
            isB1 = true;
            currentBut = B1;
        } else {
            isB1 = false;
        }
        if (getB1_in()) {
            isB5 = true;
            currentBut = B5;
        } else {
            isB5 = false;
        }
        if (getB2_in()) {
            isB9 = true;
            currentBut = B9;
        } else {
            isB9 = false;
        }
        if (getB3_in()) {
            isB13 = true;
            currentBut = B13;
        } else {
            isB13 = false;
        }
        resetB0_out();
        resetB1_out();
        resetB2_out();
        resetB3_out();
        setB2_out();
        // for(volatile int i = 0; i< 10000; i++) {}
        currentWhichBut = WhichBut::B_3;
        break;
    case B_3:
        if (getB0_in()) {
            isB2 = true;
            currentBut = B2;
        } else {
            isB2 = false;
        }
        if (getB1_in()) {
            isB6 = true;
            currentBut = B6;
        } else {
            isB6 = false;
        }
        if (getB2_in()) {
            isB10 = true;
            currentBut = B10;
        } else {
            isB10 = false;
        }
        if (getB3_in()) {
            isB14 = true;
            currentBut = B14;
        } else {
            isB14 = false;
        }
        resetB0_out();
        resetB1_out();
        resetB2_out();
        resetB3_out();
        setB3_out();
        currentWhichBut = WhichBut::B_0;
        break;
    default:
        break;
    }
    if (getEnter()) {
        if (counterEnter > MAX_BUTTON_COUNTER) {
            counterEnter = 0;
            isEnter = true;
        } else {
            counterEnter++;
        }
    } else {
        counterEnter = 0;
        isEnter = false;
    }
    if (ADC1->STATR & ADC_JEOC) {
        ADC1->CTLR2 &= ~ADC_JSWSTART;
        moving_average_H(getH());
        moving_average_V(getV());
        ADC1->CTLR2 |= ADC_JSWSTART;
    }
}

void Buttons::setB0_out() { GPIOE->BSHR |= GPIO_BSHR_BR8; }
void Buttons::setB1_out() { GPIOE->BSHR |= GPIO_BSHR_BR9; }
void Buttons::setB2_out() { GPIOE->BSHR |= GPIO_BSHR_BR10; }
void Buttons::setB3_out() { GPIOE->BSHR |= GPIO_BSHR_BR11; }
void Buttons::resetB0_out() { GPIOE->BSHR |= GPIO_BSHR_BS8; }
void Buttons::resetB1_out() { GPIOE->BSHR |= GPIO_BSHR_BS9; }
void Buttons::resetB2_out() { GPIOE->BSHR |= GPIO_BSHR_BS10; }
void Buttons::resetB3_out() { GPIOE->BSHR |= GPIO_BSHR_BS11; }
bool Buttons::getB0_in() { return !(GPIOE->INDR & GPIO_INDR_IDR12); }
bool Buttons::getB1_in() { return !(GPIOE->INDR & GPIO_INDR_IDR13); }
bool Buttons::getB2_in() { return !(GPIOE->INDR & GPIO_INDR_IDR14); }
bool Buttons::getB3_in() { return !(GPIOE->INDR & GPIO_INDR_IDR15); }
bool Buttons::getEnter() { return !(GPIOB->INDR & GPIO_INDR_IDR2); }

uint16_t Buttons::getV() { return (ADC1->IDATAR1 & 0xFFFC); }
uint16_t Buttons::getH() { return (ADC1->IDATAR2 & 0xFFFC); }

void Buttons::but_init() {
    // Buttons Out B_0-B_3 => PE8-PE11 od 0:1:1:1
    RCC->APB2PCENR |= RCC_IOPEEN;
    GPIOE->CFGHR |= GPIO_CFGHR_MODE8 | GPIO_CFGHR_MODE9 | GPIO_CFGHR_MODE10 |
                    GPIO_CFGHR_MODE11;
    GPIOE->CFGHR &= ~(GPIO_CFGHR_CNF8 | GPIO_CFGHR_CNF9 | GPIO_CFGHR_CNF10 |
                      GPIO_CFGHR_CNF11);
    GPIOE->CFGHR |= (GPIO_CFGHR_CNF8_0 | GPIO_CFGHR_CNF9_0 |
                     GPIO_CFGHR_CNF10_0 | GPIO_CFGHR_CNF11_0);

    // Buttons In B_enter-B2 B_0:B_3-PE12:PE15 od 0:1:0:0
    RCC->APB2PCENR |= RCC_IOPBEN;
    GPIOB->CFGLR &= ~GPIO_CFGLR_MODE2;
    GPIOB->CFGLR &= ~GPIO_CFGLR_CNF2;
    GPIOB->CFGLR |= GPIO_CFGLR_CNF2_0;
    GPIOE->CFGHR &= ~(GPIO_CFGHR_MODE12 | GPIO_CFGHR_MODE13 |
                      GPIO_CFGHR_MODE14 | GPIO_CFGHR_MODE15);
    GPIOE->CFGHR &= ~(GPIO_CFGHR_CNF12 | GPIO_CFGHR_CNF13 | GPIO_CFGHR_CNF14 |
                      GPIO_CFGHR_CNF15);
    GPIOE->CFGHR |= (GPIO_CFGHR_CNF12_0 | GPIO_CFGHR_CNF13_0 |
                     GPIO_CFGHR_CNF14_0 | GPIO_CFGHR_CNF15_0);
    // Buttons In Joy_but C4 od 0:1:0:0
    RCC->APB2PCENR |= RCC_IOPCEN;
    GPIOC->CFGLR &= ~GPIO_CFGLR_MODE4;
    GPIOC->CFGLR &= ~GPIO_CFGLR_CNF4;
    GPIOC->CFGLR |= GPIO_CFGLR_CNF4_0;
}

void Buttons::joy_init() {
    //// init ADC1
    //
    // PC0,PC1 - analog input mode : 0:0:0:0
    RCC->APB2PCENR |= RCC_IOPCEN;
    GPIOC->CFGLR &= ~(GPIO_CFGLR_CNF0 | GPIO_CFGLR_CNF1);
    GPIOC->CFGLR &= ~(GPIO_CFGLR_MODE0 | GPIO_CFGLR_MODE1);

    RCC->APB2PCENR |= RCC_ADC1EN;
    // PCLK2 - 72MHz / 6 = ADC_CLK = 12 MHz
    RCC->CFGR0 |= RCC_ADCPRE_DIV6;

    ADC1->SAMPTR1 |= ADC_SMP10_1 | ADC_SMP10_0; // 0:1:1 29 cycles conversion
    ADC1->SAMPTR1 |= ADC_SMP11_1 | ADC_SMP11_0; // 0:1:1 29 cycles conversion
    ADC1->CTLR2 |= ADC_JEXTTRIG;
    // 111: ISWSTART software trigger
    ADC1->CTLR2 |= ADC_JEXTSEL; // 1:1:1 ISWSTART software trigger
    ADC1->CTLR2 |= ADC_JEXTSEL_1 | ADC_JEXTSEL_0;
    ADC1->ISQR |= ADC_JL_0; // 0:1 - two channels
    // JSQ1-ch10, JSQ2-ch11
    ADC1->ISQR |= (10 << 0);
    ADC1->ISQR |= (11 << 5);
    // ADC1->ISQR = (10 << 10);
    ADC1->CTLR2 |= ADC_ADON; // enable ADC

    // ADC_ResetCalibration(ADC1);
    ADC1->CTLR2 |= ADC_RSTCAL;
    // while(ADC_GetResetCalibrationStatus(ADC1));
    while (ADC1->CTLR2 & ADC_RSTCAL) {}
    // ADC_StartCalibration(ADC1);
    ADC1->CTLR2 |= ADC_CAL;
    // while (ADC_GetCalibrationStatus(ADC1));
    while (ADC1->CTLR2 & ADC_CAL) {}
    // Calibrattion_Val = ADC1->RDATAR;
    // Calibrattion_Val = Get_CalibrationValue(ADC1);
    ADC1->CTLR2 |= ADC_JSWSTART;
}

uint16_t Buttons::moving_average_V(uint16_t val) {
    MovAverSum_V -= arr_V[Index_mov_aver_V];
    MovAverSum_V += val;
    arr_V[Index_mov_aver_V] = val;
    Index_mov_aver_V++;
    if (Index_mov_aver_V == MovAverLength) { Index_mov_aver_V = 0; }
    uint8_t tempGrade = MovAverGrade();
    // (if MovAverLength=32) common case:log2(MovAverLength)
    averageV = MovAverSum_V >> tempGrade;
    return averageV;
}

uint16_t Buttons::moving_average_H(uint16_t val) {
    MovAverSum_H -= arr_H[Index_mov_aver_H];
    MovAverSum_H += val;
    arr_H[Index_mov_aver_H] = val;
    Index_mov_aver_H++;
    if (Index_mov_aver_H == MovAverLength) { Index_mov_aver_H = 0; }
    uint8_t tempGrade = MovAverGrade();
    // (if MovAverLength=32) common case:log2(MovAverLength)
    averageH = MovAverSum_H >> tempGrade;
    return averageH;
}
