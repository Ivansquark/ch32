#include "buttons.h"

Buttons::Buttons(uint16_t stackSize) : FR_OS(stackSize) {
    FR_OS::init_FR_OS();
    but_init();
    joy_init();
}

void Buttons::runTask(void* pvParameters) {
    if (ADC1->STATR & ADC_JEOC) {
        ADC1->CTLR2 &= ~ADC_JSWSTART;
        V = getV();
        H = getH();
        ADC1->CTLR2 |= ADC_JSWSTART;
    }
    //TODO but handler()
}

uint16_t Buttons::getV() { return (ADC1->IDATAR1 & 0xFFFC); }
uint16_t Buttons::getH() { return (ADC1->IDATAR2 & 0xFFFC); }

void but_init() {
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
    GPIOB->CFGHR &= ~(GPIO_CFGHR_MODE12 | GPIO_CFGHR_MODE13 |
                      GPIO_CFGHR_MODE14 | GPIO_CFGHR_MODE15);
    GPIOB->CFGHR &= ~(GPIO_CFGHR_CNF12 | GPIO_CFGHR_CNF13 | GPIO_CFGHR_CNF14 |
                      GPIO_CFGHR_CNF15);
    GPIOB->CFGHR |= (GPIO_CFGHR_CNF12_0 | GPIO_CFGHR_CNF13_0 |
                     GPIO_CFGHR_CNF14_0 | GPIO_CFGHR_CNF15_0);
    // Buttons In Joy_but C4 od 0:1:0:0
    RCC->APB2PCENR |= RCC_IOPCEN;
    GPIOC->CFGLR &= ~GPIO_CFGLR_MODE4;
    GPIOC->CFGLR &= ~GPIO_CFGLR_CNF4;
    GPIOC->CFGLR |= GPIO_CFGLR_CNF4_0;
}

void joy_init() {
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
