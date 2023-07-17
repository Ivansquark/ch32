#include "adc.h"

uint16_t Calibrattion_Val = 0;

void adc_init()
{
    // init ADC1
    //
    // PA1 - ch1 analog input mode : 0:0:0:0
    RCC->APB2PCENR |= RCC_IOPAEN;
    GPIOA->CFGLR &= ~(GPIO_CFGLR_CNF1);
    GPIOA->CFGLR &= ~(GPIO_CFGLR_MODE1);

    // PC4 - ch2 analog input mode : 0:0:0:0
    RCC->APB2PCENR |= RCC_IOPCEN;
    GPIOC->CFGLR &= ~(GPIO_CFGLR_CNF4);
    GPIOC->CFGLR &= ~(GPIO_CFGLR_MODE4);

    RCC->APB2PCENR |= RCC_ADC1EN;
    // PCLK2 - 24MHz / 4 = ADC_CLK = 8 MHz
    RCC->CFGR0 |= RCC_ADCPRE_DIV4;

    // adc in continuos mode with DMA
    ADC1->CTLR2 |= ADC_CONT;    // continous mode
    ADC1->CTLR2 |= ADC_EXTTRIG; // conversion on external signal
    ADC1->CTLR2 |= ADC_EXTSEL;  // 1:1:1 RSWSTART software trigger
    // A2 channel
    ADC1->SAMPTR2 |= ADC_SMP0; // 1:1:1 239 cycles conversion
    // ADC1->SAMPTR2 &= ~ADC_SMP0; //0:0:0 1.5 cycles conversion
    ADC1->RSQR1 = 0;  // 1 conversion
    ADC1->RSQR3 |= 1; // channel 1 1 conversion
    ADC1->RSQR3 |= 2; // channel 2 1 conversion

    // DMA enable
    ADC1->CTLR2 |= ADC_DMA;

    ADC1->CTLR2 |= ADC_ADON;

    // ADC_ResetCalibration(ADC1);
    ADC1->CTLR2 |= ADC_RSTCAL;
    // while(ADC_GetResetCalibrationStatus(ADC1));
    while (ADC1->CTLR2 & ADC_RSTCAL) {}
    // ADC_StartCalibration(ADC1);
    ADC1->CTLR2 |= ADC_CAL;
    // while (ADC_GetCalibrationStatus(ADC1));
    while (ADC1->CTLR2 & ADC_CAL) {}
    Calibrattion_Val = ADC1->RDATAR;
    // Calibrattion_Val = Get_CalibrationValue(ADC1);

    ADC1->CTLR2 |= ADC_SWSTART; // start continuous conversion
}

uint16_t adc_get_conversionVal(uint16_t val)
{
    if ((Calibrattion_Val + val) > 4095 || val == 4095) {
        return 4095;
    }
    return (val + Calibrattion_Val);
}
