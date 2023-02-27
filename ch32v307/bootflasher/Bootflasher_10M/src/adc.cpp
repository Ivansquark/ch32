#include "adc.h"

Adc* Adc::pThis = nullptr;
Adc::Adc() {
    pThis = this;
    // init();
    init_injected_channels_dma();
}

uint16_t Adc::getU() { return (averageU * kU_mul) / kU_div; }

uint16_t Adc::getI() { return (averageI * kI_mul) / kI_div; }

uint16_t Adc::moving_average_U(uint16_t val) {
    MovAverSum_U -= arr_U[Index_mov_aver_U];
    MovAverSum_U += val;
    arr_U[Index_mov_aver_U] = val;
    Index_mov_aver_U++;
    if (Index_mov_aver_U == MovAverLength) { Index_mov_aver_U = 0; }
    uint8_t tempGrade = MovAverGrade();
    averageU =
        MovAverSum_U >>
        tempGrade; // (if MovAverLength=32) common case:log2(MovAverLength)
    return averageU;
}

uint16_t Adc::moving_average_I(uint16_t val) {
    MovAverSum_I -= arr_I[Index_mov_aver_I];
    MovAverSum_I += val;
    arr_I[Index_mov_aver_I] = val;
    Index_mov_aver_I++;
    if (Index_mov_aver_I == MovAverLength) { Index_mov_aver_I = 0; }
    uint8_t tempGrade = MovAverGrade();
    averageI =
        MovAverSum_I >>
        tempGrade; // (if MovAverLength=32) common case:log2(MovAverLength)
    return averageI;
}

void Adc::adcHandler1() {
    // exclude last 2 digits (sliding average)
    moving_average_U(ADC1->RDATAR & 0xFFFC);

    // medium value
    // arr_U[counterADC] = ADC1->JDR1 & 0xFFFC;
    // arr_I[counterADC] = ADC1->JDR2 & 0xFFFC;
    // if(counterADC >= medianArrLength) {
    //     counterADC = 0;
    // } else {
    //     counterADC++;
    // }
}
void Adc::adcHandler2() {
    // exclude last 2 digits (sliding average)
    moving_average_I(ADC2->RDATAR & 0xFFFC);

    // medium value
    // arr_U[counterADC] = ADC1->JDR1 & 0xFFFC;
    // arr_I[counterADC] = ADC1->JDR2 & 0xFFFC;
    // if(counterADC >= medianArrLength) {
    //     counterADC = 0;
    // } else {
    //     counterADC++;
    // }
}
void Adc::sortAndGetMedian() {
    // buble sort
    for (int i = 0; i < medianArrLength; i++) {
        for (int j = 0; j < medianArrLength - i; j++) {
            if (arr_I[j] > arr_I[j + 1]) {
                uint16_t tempI = arr_I[j];
                arr_I[j] = arr_I[j + 1];
                arr_I[j + 1] = tempI;
                uint16_t tempU = arr_U[j];
                arr_U[j] = arr_U[j + 1];
                arr_U[j + 1] = tempU;
            }
        }
    }

    for (int i = medianArrLength / 2 - MedianMiddle / 2;
         i < medianArrLength / 2 + MedianMiddle / 2; i++) {

        sumI += arr_I[i];
        sumU += arr_U[i];
    }
    averageU = sumU / MedianMiddle;
    averageI = sumI / MedianMiddle;
    sumI = 0;
    sumU = 0;
}

void Adc::init() {
    // init ADC1
    //
    // PA0 - analog input mode : 0:0:0:0
    RCC->APB2PCENR |= RCC_IOPAEN;
    GPIOA->CFGLR &= ~(GPIO_CFGLR_CNF0);
    GPIOA->CFGLR &= ~(GPIO_CFGLR_MODE0);

    RCC->APB2PCENR |= RCC_ADC1EN;
    // PCLK2 - 72MHz / 6 = ADC_CLK = 12 MHz
    RCC->CFGR0 |= RCC_ADCPRE_DIV6;

    ADC1->CTLR2 |= ADC_EXTSEL;  // 1:1:1 RSWSTART software trigger
    ADC1->CTLR2 |= ADC_EXTTRIG; // conversion on external signal
    // A0 channel
    ADC1->SAMPTR2 |= ADC_SMP0; // 1:1:1 239 cycles conversion
    // ADC1->SAMPTR2 &= ~ADC_SMP0; //0:0:0 1.5 cycles conversion
    ADC1->RSQR3 = 0; // channel 0 1 conversion

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
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//________________________________________________________________
//_____________________ ADC_ INJECTED CHANNELS ___________________
//_________________________________________________________________
void Adc::init_injected_channels_dma() {
    //! PC0-ADC1_IN10, PC2-ADC1_IN12 (analog input 0:0:0:0)
    RCC->APB2PCENR |= RCC_IOPCEN;
    GPIOC->CFGLR &= ~(GPIO_CFGLR_CNF0 | GPIO_CFGLR_CNF2 | GPIO_CFGLR_MODE0 |
                      GPIO_CFGLR_MODE2);

    RCC->APB2PCENR |= RCC_ADC1EN;
    RCC->APB2PCENR |= RCC_ADC2EN;
    // PCLK2 - 72MHz / 6 = ADC_CLK = 12 MHz
    RCC->CFGR0 |= RCC_ADCPRE_DIV6;
    // scan mode for sequence of channels (multichannel mode)
    // ADC1->CTLR1 |= ADC_SCAN;
    // ADC2->CTLR1 |= ADC_SCAN;
    // discontinuous mode
    // ADC1->CTLR1 |= ADC_JDISCEN;
    // 1-continuous conversion (for regular channels),   0 -single conversion
    // ADC1->CTLR2 |= ADC_CONT;

    // C0_10  channel C2_12  channel
    ADC1->SAMPTR1 |= ADC_SMP0; // 1:1:1 239 cycles conversion
    ADC1->SAMPTR1 |= ADC_SMP2; // 1:1:1 239 cycles conversion
    ADC2->SAMPTR1 |= ADC_SMP0; // 1:1:1 239 cycles conversion
    ADC2->SAMPTR1 |= ADC_SMP2; // 1:1:1 239 cycles conversion
    // ADC1->SAMPTR1 &= ~ADC_SMP0; // 0:0:0 239 cycles conversion
    // ADC1->SAMPTR1 &= ~ADC_SMP2; // 0:0:0 1,5 cycles conversion
    // ADC1->SQR3 =0; // 1 conversion
    // ADC1->SQR3 =0; // 1 conversion
    // ADC1->SQR3 = 0; // 1 conversion in channel 0
    // ADC1->SQR3 = 1; // 1 conversion in channel 1

    ADC1->CTLR1 &= ~ADC_BUFEN;
    ADC1->CTLR2 |= ADC_EXTTRIG;
    ADC1->CTLR2 |= ADC_EXTSEL;
    ADC1->RSQR1 = 0;
    ADC1->RSQR3 |= 10;
    ADC1->CTLR2 |= ADC_ADON; // enable ADC
                             // ADC_ResetCalibration(ADC1);
    ADC1->CTLR2 |= ADC_RSTCAL;
    // while(ADC_GetResetCalibrationStatus(ADC1));
    while (ADC1->CTLR2 & ADC_RSTCAL) {}
    // ADC_StartCalibration(ADC1);
    ADC1->CTLR2 |= ADC_CAL;
    // while (ADC_GetCalibrationStatus(ADC1));
    while (ADC1->CTLR2 & ADC_CAL) {}
    Calibrattion_Val = ADC1->RDATAR;
    // Calibrattion_Val = Get_CalibrationValue(ADC1);/ADC1->ISQR = (12 << 15);
    ADC1->CTLR1 |= ADC_BUFEN;

    ADC2->CTLR1 &= ~ADC_BUFEN;
    ADC2->CTLR2 |= ADC_EXTTRIG;
    ADC2->CTLR2 |= ADC_EXTSEL;
    ADC2->RSQR1 = 0;
    ADC2->RSQR3 |= 12;
    ADC2->CTLR2 |= ADC_ADON; // enable ADC
                             // ADC_ResetCalibration(ADC1);
    ADC2->CTLR2 |= ADC_RSTCAL;
    // while(ADC_GetResetCalibrationStatus(ADC1));
    while (ADC2->CTLR2 & ADC_RSTCAL) {}
    // ADC_StartCalibration(ADC1);
    ADC2->CTLR2 |= ADC_CAL;
    // while (ADC_GetCalibrationStatus(ADC1));
    while (ADC2->CTLR2 & ADC_CAL) {}
    Calibrattion_Val = ADC2->RDATAR;
    // Calibrattion_Val = Get_CalibrationValue(ADC1);/ADC1->ISQR = (12 << 15);
    ADC2->CTLR1 |= ADC_BUFEN;

//    // conversion on external signal
//    ADC1->CTLR2 |= ADC_JEXTTRIG;
//    // 111: ISWSTART software trigger
//    ADC1->CTLR2 |= ADC_JEXTSEL;
//    // ADC1->CTLR2 |= ADC_JEXTSEL_1 |ADC_JEXTSEL_0;
//    ADC1->ISQR = 0;
//    // 0:1 - 2 converted channels
//    ADC1->ISQR |= ADC_JL;
//    // JSQ1-ch10, JSQ2-ch12
//    ADC1->ISQR |= (10 << 0);
//    ADC1->ISQR |= (12 << 5);
//    // JSQ3-ch10, JSQ4-ch12
//    // ADC1->ISQR = (10 << 10);
//    ADC1->CTLR1 |= ADC_BUFEN;
//    ADC1->CTLR2 |= ADC_ADON; // enable ADC

    // ADC1->CR2 &=~ ADC_CR2_EXTEN; // on software start
    // ADC1->CR2 |= ADC_CR2_EOCS; // The EOC bit is set at the end of each
    // regular conversion ADC1->CR2 |= ADC_CR2_DMA;  // DMA enabled ADC1->CR2 |=
    // ADC_CR2_DDS;  // New DMA requests are issued

    // ADC1->SQR1&=~ADC_SQR1_L; // 0000 - 1 conversion on a channels
    // ADC1->SQR3|=ADC_SQR3_SQ1_4; //10000 - 16 channel on 1 conversion
    // ADC1->SQR3&=~ADC_SQR3_SQ1_3;ADC1->SQR3&=~ADC_SQR3_SQ1_2;ADC1->SQR3&=~ADC_SQR3_SQ1_1;ADC1->SQR3&=~ADC_SQR3_SQ1_0;

    // // ADC2 PB1
    // RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    // GPIOB->MODER |= (GPIO_MODER_MODE1);
    // RCC->APB2ENR |= RCC_APB2ENR_ADC2EN; // clock adc1

    // ADC2->CR2 &= ~ADC_CR2_CONT; // 0-single conversion,                  1 -
    // continious conversion
    // // ADC2->CR2|=ADC_CR2_SWSTART; //event start conversion from software
    // SWSTART ADC2->SMPR2 |= ADC_SMPR2_SMP9; // 111:480 cycles; ADC2->SQR3 = 9;
    // // 1 conversion in channel 9 ADC2->CR2 &= ~ADC_CR2_EXTEN;   // on
    // software start ADC2->CR2 |= ADC_CR2_EOCS;     // The EOC bit is set at
    // the end of each regular conversion ADC2->CR2 |= ADC_CR2_ADON;     //
    // enable ADC

    // RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; // BatVoltKey
    // GPIOE->MODER |= (GPIO_MODER_MODE8_0);
    // GPIOE->MODER &= ~GPIO_MODER_MODE8_1;
}

uint16_t Adc::getAdc() {
    if (startConversion1()) {
        // started
    } else {
        if (isConversionCompleteted1()) {
            uint16_t val = ADC1->RDATAR;
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
