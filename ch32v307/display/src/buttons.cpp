#include "buttons.h"

Buttons::Buttons() {
    but_init();
    joy_init();
}

uint16_t Buttons::but16Bits() {
    uint16_t bits = 0;
    if(isB0) bits |= 0x0001;
    if(isB1) bits |= 0x0002;
    if(isB2) bits |= 0x0004;
    if(isB3) bits |= 0x0008;
    if(isB4) bits |= 0x0010;
    if(isB5) bits |= 0x0020;
    if(isB6) bits |= 0x0040;
    if(isB7) bits |= 0x0080;
    if(isB8) bits |= 0x0100;
    if(isB9) bits |= 0x0200;
    if(isB10) bits |= 0x0400;
    if(isB11) bits |= 0x0800;
    if(isB12) bits |= 0x1000;
    if(isB13) bits |= 0x2000;
    if(isB14) bits |= 0x4000;
    if(isB15) bits |= 0x8000;
    return bits;
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
    if (!state) { currentBut = NONE; }
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
            if (!stack.isButInStack(B3)) {
                isB3 = true;
                currentBut = B3;
                stack.push(B3);
            }
        } else {
            if(currentBut == B3) {
                stack.pop();
            }
            isB3 = false;
        }
        if (getB1_in()) {
            if (!stack.isButInStack(B7)) {
                isB7 = true;
                currentBut = B7;
                stack.push(B7);
            }
        } else {
            if(currentBut == B7) {
                stack.pop();
            }
            isB7 = false;
        }
        if (getB2_in()) {
            if (!stack.isButInStack(B11)) {
                isB11 = true;
                currentBut = B11;
                stack.push(B11);
            }
        } else {
            if(currentBut == B11) {
                stack.pop();
            }
            isB11 = false;
        }
        if (getB3_in()) {
            if (!stack.isButInStack(B15)) {
                isB15 = true;
                currentBut = B15;
                stack.push(B15);
            }
        } else {
            if(currentBut == B15) {
                stack.pop();
            }
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
            if (!stack.isButInStack(B0)) {
                isB0 = true;
                currentBut = B0;
                stack.push(B0);
            }
        } else {
            if(currentBut == B0) {
                stack.pop();
            }
            isB0 = false;
        }
        if (getB1_in()) {
            if (!stack.isButInStack(B4)) {
                isB4 = true;
                currentBut = B4;
                stack.push(B4);
            }
        } else {
            if(currentBut == B4) {
                stack.pop();
            }
            isB4 = false;
        }
        if (getB2_in()) {
            if (!stack.isButInStack(B8)) {
                isB8 = true;
                currentBut = B8;
                stack.push(B8);
            }
        } else {
            if(currentBut == B8) {
                stack.pop();
            }
            isB8 = false;
        }
        if (getB3_in()) {
            if (!stack.isButInStack(B12)) {
                isB12 = true;
                currentBut = B12;
                stack.push(B12);
            }
        } else {
            if(currentBut == B12) {
                stack.pop();
            }
            isB12 = false;
        }
        resetB0_out();
        resetB1_out();
        resetB2_out();
        resetB3_out();
        setB1_out();
        currentWhichBut = WhichBut::B_2;
        break;
    case B_2:
        if (getB0_in()) {
            if (!stack.isButInStack(B1)) {
                isB1 = true;
                currentBut = B1;
                stack.push(B1);
            }
        } else {
            if(currentBut == B1) {
                stack.pop();
            }
            isB1 = false;
        }
        if (getB1_in()) {
            if (!stack.isButInStack(B5)) {
                isB5 = true;
                currentBut = B5;
                stack.push(B5);
            }
        } else {
            if(currentBut == B5) {
                stack.pop();
            }
            isB5 = false;
        }
        if (getB2_in()) {
            if (!stack.isButInStack(B9)) {
                isB9 = true;
                currentBut = B9;
                stack.push(B9);
            }
        } else {
            if(currentBut == B9) {
                stack.pop();
            }
            isB9 = false;
        }
        if (getB3_in()) {
            if (!stack.isButInStack(B13)) {
                isB13 = true;
                currentBut = B13;
                stack.push(B13);
            }
        } else {
            if(currentBut == B13) {
                stack.pop();
            }
            isB13 = false;
        }
        resetB0_out();
        resetB1_out();
        resetB2_out();
        resetB3_out();
        setB2_out();
        currentWhichBut = WhichBut::B_3;
        break;
    case B_3:
        if (getB0_in()) {
            if (!stack.isButInStack(B2)) {
                isB2 = true;
                currentBut = B2;
                stack.push(B2);
            }
        } else {
            if(currentBut == B2) {
                stack.pop();
            }
            isB2 = false;
        }
        if (getB1_in()) {
            if (!stack.isButInStack(B6)) {
                isB6 = true;
                currentBut = B6;
                stack.push(B6);
            }
        } else {
            if(currentBut == B6) {
                stack.pop();
            }
            isB6 = false;
        }
        if (getB2_in()) {
            if (!stack.isButInStack(B10)) {
                isB10 = true;
                currentBut = B10;
                stack.push(B10);
            }
        } else {
            if(currentBut == B10) {
                stack.pop();
            }
            isB10 = false;
        }
        if (getB3_in()) {
            if (!stack.isButInStack(B14)) {
                isB14 = true;
                currentBut = B14;
                stack.push(B14);
            }
        } else {
            if(currentBut == B14) {
                stack.pop();
            }
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
    if (!isAnyButtonPressed()) {
        stack.clear();
        currentBut = NONE;
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
        ADC1->STATR = 0;
        //ADC1->STATR &= ~ADC_JSTRT;
        //ADC1->STATR &= ~ADC_JEOC;
        //ADC1->CTLR2 &= ~ADC_JSWSTART;
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

    ADC1->SAMPTR1 &= ~(ADC_SMP10 | ADC_SMP11); // 0:0:0 1,5 cycles conversion
    ADC1->SAMPTR1 |= ADC_SMP10_1 | ADC_SMP10_0; // 0:1:1 29 cycles conversion
    ADC1->SAMPTR1 |= ADC_SMP11_1 | ADC_SMP11_0; // 0:1:1 29 cycles conversion
    ADC1->CTLR2 |= ADC_JEXTTRIG;
    // 111: ISWSTART software trigger
    ADC1->CTLR2 |= ADC_JEXTSEL; // 1:1:1 ISWSTART software trigger
    //ADC1->CTLR2 |= ADC_JEXTSEL_1 | ADC_JEXTSEL_0;
    //ADC1->CTLR1 |= ADC_JAUTO;
    ADC1->CTLR1 |= ADC_SCAN;
    //ADC1->CTLR1 |= ADC_JDISCEN;
    ADC1->ISQR |= ADC_JL_0; // 0:1 - two channels
    // JSQ1-ch10, JSQ2-ch11
    //ADC1->ISQR |= (10 << 0);
    //ADC1->ISQR |= (11 << 5);
    ADC1->ISQR |= (10 << 10);
    ADC1->ISQR |= (11 << 15);
    // ADC1->ISQR = (10 << 10);
    ADC1->CTLR2 |= ADC_ADON; // enable ADC

    // ADC_ResetCalibration(ADC1);
    
    ADC1->CTLR1 &= ~ADC_OutputBuffer_Enable;
    ADC1->CTLR2 |= ADC_RSTCAL;
    // while(ADC_GetResetCalibrationStatus(ADC1));
    while (ADC1->CTLR2 & ADC_RSTCAL) {}
    // ADC_StartCalibration(ADC1);
    ADC1->CTLR2 |= ADC_CAL;
    // while (ADC_GetCalibrationStatus(ADC1));
    while (ADC1->CTLR2 & ADC_CAL) {}
    ADC1->CTLR1 |= ADC_OutputBuffer_Enable;
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
