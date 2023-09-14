#include "buttons.h"

Buttons::Buttons() {
    but_init();
    joy_init();
}

uint8_t Buttons::whichNumber() {
    if (pressed2) {
        return pressed2;
    } else if (pressed1) {
        return pressed1;
    } else
        return Button::NONE;
    return 0;
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
    } else if (averageV > 3000) {
        state = true;
    } else if (averageV < 1000) {
        state = true;
    } else if (averageH > 3000) {
        state = true;
    } else if (averageH < 1000) {
        state = true;
    };
    // if (!state) { currentBut = NONE; }
    return state;
}

void Buttons::interruptHandler() {
    // once in ms
    // TODO: check buttons 4 times strobes
    if (averageV < 1000) {
        if (!pressed2) {
            isDown = true;
            butStateArr[iBdown] = Button::Bdown;
            if (!pressed1) {
                pressed1 = Button::Bdown;
            } else {
                if (pressed1 != Button::Bdown) pressed2 = Button::Bdown;
            }
        }
    } else {
        isDown = false;
        butStateArr[iBdown] = Button::NONE;
        if (pressed2 == Button::Bdown) {
            pressed2 = Button::NONE;
        } else if (pressed1 == Button::Bdown) {
            pressed1 = pressed2;
            pressed2 = Button::NONE;
        }
    }
    if (averageV > 3000) {
        if (!pressed2) {
            isUp = true;
            butStateArr[iBup] = Button::Bup;
            if (!pressed1) {
                pressed1 = Button::Bup;
            } else {
                if (pressed1 != Button::Bup) pressed2 = Button::Bup;
            }
        }
    } else {
        isUp = false;
        butStateArr[iBup] = Button::NONE;
        if (pressed2 == Button::Bup) {
            pressed2 = Button::NONE;
        } else if (pressed1 == Button::Bup) {
            pressed1 = pressed2;
            pressed2 = Button::NONE;
        }
    }
    if (averageH > 3000) {
        if (!pressed2) {
            isRight = true;
            butStateArr[iBright] = Button::Bright;
            if (!pressed1) {
                pressed1 = Button::Bright;
            } else {
                if (pressed1 != Button::Bright) pressed2 = Button::Bright;
            }
        }
    } else {
        isRight = false;
        butStateArr[iBright] = Button::NONE;
        if (pressed2 == Button::Bright) {
            pressed2 = Button::NONE;
        } else if (pressed1 == Button::Bright) {
            pressed1 = pressed2;
            pressed2 = Button::NONE;
        }
    }
    if (averageH < 1000) {
        if (!pressed2) {
            isLeft = true;
            butStateArr[iBleft] = Button::Bleft;
            if (!pressed1) {
                pressed1 = Button::Bleft;
            } else {
                if (pressed1 != Button::Bleft) pressed2 = Button::Bleft;
            }
        }
    } else {
        isLeft = false;
        butStateArr[iBleft] = Button::NONE;
        if (pressed2 == Button::Bleft) {
            pressed2 = Button::NONE;
        } else if (pressed1 == Button::Bleft) {
            pressed1 = pressed2;
            pressed2 = Button::NONE;
        }
    }
    switch (currentWhichBut) {
    case B_0:
        if (getB0_in()) {
            if (!pressed2) {
                isB3 = true;
                butStateArr[iB3] = Button::B3;
                if (!pressed1) {
                    pressed1 = Button::B3;
                } else {
                    if (pressed1 != Button::B3) pressed2 = Button::B3;
                }
            }
        } else {
            isB3 = false;
            butStateArr[iB3] = Button::NONE;
            if (pressed2 == Button::B3) {
                pressed2 = Button::NONE;
            } else if (pressed1 == Button::B3) {
                pressed1 = pressed2;
                pressed2 = Button::NONE;
            }
        }
        if (getB1_in()) {
            if (!pressed2) {
                isB7 = true;
                butStateArr[iB7] = Button::B7;
                if (!pressed1) {
                    pressed1 = Button::B7;
                } else {
                    if (pressed1 != Button::B7) pressed2 = Button::B7;
                }
            }
        } else {
            isB7 = false;
            butStateArr[iB7] = Button::NONE;
            if (pressed2 == Button::B7) {
                pressed2 = Button::NONE;
            } else if (pressed1 == Button::B7) {
                pressed1 = pressed2;
                pressed2 = Button::NONE;
            }
        }
        if (getB2_in()) {
            if (!pressed2) {
                isB11 = true;
                butStateArr[iB11] = Button::B11;
                if (!pressed1) {
                    pressed1 = Button::B11;
                } else {
                    if (pressed1 != Button::B11) pressed2 = Button::B11;
                }
            }
        } else {
            isB11 = false;
            butStateArr[iB11] = Button::NONE;
            if (pressed2 == Button::B11) {
                pressed2 = Button::NONE;
            } else if (pressed1 == Button::B11) {
                pressed1 = pressed2;
                pressed2 = Button::NONE;
            }
        }
        if (getB3_in()) {
            if (!pressed2) {
                isB15 = true;
                butStateArr[iB15] = Button::B15;
                if (!pressed1) {
                    pressed1 = Button::B15;
                } else {
                    if (pressed1 != Button::B15) pressed2 = Button::B15;
                }
            }
        } else {
            isB15 = false;
            butStateArr[iB15] = Button::NONE;
            if (pressed2 == Button::B15) {
                pressed2 = Button::NONE;
            } else if (pressed1 == Button::B15) {
                pressed1 = pressed2;
                pressed2 = Button::NONE;
            }
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
            if (!pressed2) {
                isB0 = true;
                butStateArr[iB0] = Button::B0;
                if (!pressed1) {
                    pressed1 = Button::B0;
                } else {
                    if (pressed1 != Button::B0) pressed2 = Button::B0;
                }
            }
        } else {
            isB0 = false;
            butStateArr[iB0] = Button::NONE;
            if (pressed2 == Button::B0) {
                pressed2 = Button::NONE;
            } else if (pressed1 == Button::B0) {
                pressed1 = pressed2;
                pressed2 = Button::NONE;
            }
        }
        if (getB1_in()) {
            if (!pressed2) {
                isB4 = true;
                butStateArr[iB4] = Button::B4;
                if (!pressed1) {
                    pressed1 = Button::B4;
                } else {
                    if (pressed1 != Button::B4) pressed2 = Button::B4;
                }
            }
        } else {
            isB4 = false;
            butStateArr[iB4] = Button::NONE;
            if (pressed2 == Button::B4) {
                pressed2 = Button::NONE;
            } else if (pressed1 == Button::B4) {
                pressed1 = pressed2;
                pressed2 = Button::NONE;
            }
        }
        if (getB2_in()) {
            if (!pressed2) {
                isB8 = true;
                butStateArr[iB8] = Button::B8;
                if (!pressed1) {
                    pressed1 = Button::B8;
                } else {
                    if (pressed1 != Button::B8) pressed2 = Button::B8;
                }
            }
        } else {
            isB8 = false;
            butStateArr[iB8] = Button::NONE;
            if (pressed2 == Button::B8) {
                pressed2 = Button::NONE;
            } else if (pressed1 == Button::B8) {
                pressed1 = pressed2;
                pressed2 = Button::NONE;
            }
        }
        if (getB3_in()) {
            if (!pressed2) {
                isB12 = true;
                butStateArr[iB12] = Button::B12;
                if (!pressed1) {
                    pressed1 = Button::B12;
                } else {
                    if (pressed1 != Button::B12) pressed2 = Button::B12;
                }
            }
        } else {
            isB12 = false;
            butStateArr[iB12] = Button::NONE;
            if (pressed2 == Button::B12) {
                pressed2 = Button::NONE;
            } else if (pressed1 == Button::B12) {
                pressed1 = pressed2;
                pressed2 = Button::NONE;
            }
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
            if (!pressed2) {
                isB1 = true;
                butStateArr[iB1] = Button::B1;
                if (!pressed1) {
                    pressed1 = Button::B1;
                } else {
                    if (pressed1 != Button::B1) pressed2 = Button::B1;
                }
            }
        } else {
            isB1 = false;
            butStateArr[iB1] = Button::NONE;
            if (pressed2 == Button::B1) {
                pressed2 = Button::NONE;
            } else if (pressed1 == Button::B1) {
                pressed1 = pressed2;
                pressed2 = Button::NONE;
            }
        }

        if (getB1_in()) {
            if (!pressed2) {
                isB5 = true;
                butStateArr[iB5] = Button::B5;
                if (!pressed1) {
                    pressed1 = Button::B5;
                } else {
                    if (pressed1 != Button::B5) pressed2 = Button::B5;
                }
            }
        } else {
            isB5 = false;
            butStateArr[iB5] = Button::NONE;
            if (pressed2 == Button::B5) {
                pressed2 = Button::NONE;
            } else if (pressed1 == Button::B5) {
                pressed1 = pressed2;
                pressed2 = Button::NONE;
            }
        }
        if (getB2_in()) {
            if (!pressed2) {
                isB9 = true;
                butStateArr[iB9] = Button::B9;
                if (!pressed1) {
                    pressed1 = Button::B9;
                } else {
                    if (pressed1 != Button::B9) pressed2 = Button::B9;
                }
            }
        } else {
            isB9 = false;
            butStateArr[iB9] = Button::NONE;
            if (pressed2 == Button::B9) {
                pressed2 = Button::NONE;
            } else if (pressed1 == Button::B9) {
                pressed1 = pressed2;
                pressed2 = Button::NONE;
            }
        }
        if (getB3_in()) {
            if (!pressed2) {
                isB13 = true;
                butStateArr[iB13] = Button::B13;
                if (!pressed1) {
                    pressed1 = Button::B13;
                } else {
                    if (pressed1 != Button::B13) pressed2 = Button::B13;
                }
            }
        } else {
            isB13 = false;
            butStateArr[iB13] = Button::NONE;
            if (pressed2 == Button::B13) {
                pressed2 = Button::NONE;
            } else if (pressed1 == Button::B13) {
                pressed1 = pressed2;
                pressed2 = Button::NONE;
            }
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
            if (!pressed2) {
                isB2 = true;
                butStateArr[iB2] = Button::B2;
                if (!pressed1) {
                    pressed1 = Button::B2;
                } else {
                    if (pressed1 != Button::B2) pressed2 = Button::B2;
                }
            }
        } else {
            isB2 = false;
            butStateArr[iB2] = Button::NONE;
            if (pressed2 == Button::B2) {
                pressed2 = Button::NONE;
            } else if (pressed1 == Button::B2) {
                pressed1 = pressed2;
                pressed2 = Button::NONE;
            }
        }
        if (getB1_in()) {
            if (!pressed2) {
                isB6 = true;
                butStateArr[iB6] = Button::B6;
                if (!pressed1) {
                    pressed1 = Button::B6;
                } else {
                    if (pressed1 != Button::B6) pressed2 = Button::B6;
                }
            }
        } else {
            isB6 = false;
            butStateArr[iB6] = Button::NONE;
            if (pressed2 == Button::B6) {
                pressed2 = Button::NONE;
            } else if (pressed1 == Button::B6) {
                pressed1 = pressed2;
                pressed2 = Button::NONE;
            }
        }
        if (getB2_in()) {
            if (!pressed2) {
                isB10 = true;
                butStateArr[iB10] = Button::B10;
                if (!pressed1) {
                    pressed1 = Button::B10;
                } else {
                    if (pressed1 != Button::B10) pressed2 = Button::B10;
                }
            }
        } else {
            isB10 = false;
            butStateArr[iB10] = Button::NONE;
            if (pressed2 == Button::B10) {
                pressed2 = Button::NONE;
            } else if (pressed1 == Button::B10) {
                pressed1 = pressed2;
                pressed2 = Button::NONE;
            }
        }
        if (getB3_in()) {
            if (!pressed2) {
                isB14 = true;
                butStateArr[iB14] = Button::B14;
                if (!pressed1) {
                    pressed1 = Button::B14;
                } else {
                    if (pressed1 != Button::B14) pressed2 = Button::B14;
                }
            }
        } else {
            isB14 = false;
            butStateArr[iB14] = Button::NONE;
            if (pressed2 == Button::B14) {
                pressed2 = Button::NONE;
            } else if (pressed1 == Button::B14) {
                pressed1 = pressed2;
                pressed2 = Button::NONE;
            }
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
            if (!pressed2) {
                isEnter = true;
                butStateArr[iEnter] = Button::Enter;
                if (!pressed1) {
                    pressed1 = Button::Enter;
                } else {
                    if (pressed1 != Button::Enter) pressed2 = Button::Enter;
                }
            }
        } else {
            counterEnter++;
        }
    } else {
        counterEnter = 0;
        isEnter = false;
        butStateArr[iEnter] = Button::NONE;
        if (pressed2 == Button::Enter) {
            pressed2 = Button::NONE;
        } else if (pressed1 == Button::Enter) {
            pressed1 = pressed2;
            pressed2 = Button::NONE;
        }
    }
    if (getJoyB()) {
        if (counterJoyB > 1000) {
            counterJoyB = 0;
            isJoyB = true;
        } else {
            counterJoyB++;
        }
    } else {
        isJoyB = false;
        counterJoyB = 0;
    }
    if (!isAnyButtonPressed()) {
        pressed2 = Button::NONE;
        pressed1 = Button::NONE;
    }
    if (ADC1->STATR & ADC_JEOC) {
        ADC1->STATR = 0;
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
bool Buttons::getJoyB() { return !(GPIOC->INDR & GPIO_INDR_IDR4); }

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

    ADC1->SAMPTR1 &= ~(ADC_SMP10 | ADC_SMP11);  // 0:0:0 1,5 cycles conversion
    ADC1->SAMPTR1 |= ADC_SMP10_1 | ADC_SMP10_0; // 0:1:1 29 cycles conversion
    ADC1->SAMPTR1 |= ADC_SMP11_1 | ADC_SMP11_0; // 0:1:1 29 cycles conversion
    ADC1->CTLR2 |= ADC_JEXTTRIG;
    // 111: ISWSTART software trigger
    ADC1->CTLR2 |= ADC_JEXTSEL; // 1:1:1 ISWSTART software trigger
    // ADC1->CTLR2 |= ADC_JEXTSEL_1 | ADC_JEXTSEL_0;
    // ADC1->CTLR1 |= ADC_JAUTO;
    ADC1->CTLR1 |= ADC_SCAN;
    // ADC1->CTLR1 |= ADC_JDISCEN;
    ADC1->ISQR |= ADC_JL_0; // 0:1 - two channels
    // JSQ1-ch10, JSQ2-ch11
    // ADC1->ISQR |= (10 << 0);
    // ADC1->ISQR |= (11 << 5);
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
