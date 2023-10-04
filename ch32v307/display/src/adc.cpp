#include "adc.h"

Adc* Adc::pThis = nullptr;
Adc::Adc() {
    pThis = this;
    init();
}

void Adc::init() {
    // init ADC1 15 channel
    //
    // PC5 - analog input mode : 0:0:0:0
    RCC->APB2PCENR |= RCC_IOPCEN;
    GPIOC->CFGLR &= ~(GPIO_CFGLR_CNF5);
    GPIOC->CFGLR &= ~(GPIO_CFGLR_MODE5);

    RCC->APB2PCENR |= RCC_ADC2EN;
    // PCLK2 - 72MHz / 6 = ADC_CLK = 12 MHz
    RCC->CFGR0 |= RCC_ADCPRE_DIV8;

    ADC2->CTLR2 |= ADC_EXTSEL;  // 1:1:1 RSWSTART software trigger
    ADC2->CTLR2 |= ADC_EXTTRIG; // conversion on external signal
    // A0 channel
    ADC2->SAMPTR2 |= ADC_SMP0; // 1:1:1 239 cycles conversion
    // ADC1->SAMPTR2 &= ~ADC_SMP0; //0:0:0 1.5 cycles conversion
    ADC2->RSQR3 = 15; // channel 15 1 conversion

    ADC2->CTLR2 |= ADC_ADON;

    // ADC_ResetCalibration(ADC1);
    ADC2->CTLR2 |= ADC_RSTCAL;
    // while(ADC_GetResetCalibrationStatus(ADC1));
    while (ADC2->CTLR2 & ADC_RSTCAL) {}
    // ADC_StartCalibration(ADC1);
    ADC2->CTLR2 |= ADC_CAL;
    // while (ADC_GetCalibrationStatus(ADC1));
    while (ADC2->CTLR2 & ADC_CAL) {}
    Calibrattion_Val = ADC2->RDATAR;
    // Calibrattion_Val = Get_CalibrationValue(ADC1);
}

uint16_t Adc::getAdc() {
    if (startConversion()) {
        // started
    } else {
        if (isConversionCompleteted()) {
            uint16_t val = ADC2->RDATAR;
            if ((val + Calibrattion_Val) < 0) {
                tempVal = 0;
            } else if ((Calibrattion_Val + val) > 4095 || val == 4095) {
                tempVal = 4095;
            } else {
                tempVal = val + Calibrattion_Val;
            }
        }
    }
    return tempVal;
}
