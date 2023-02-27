#ifndef ADC_H
#define ADC_H

#include "ch32v30x_adc.h"
#include "ch32v30x_rcc.h"
#include "gptimers.h"
#include "main.h"
#include "math.h"

class Adc {
  public:
    Adc();
    static Adc* pThis;
    uint16_t getAdc();
    inline bool startConversion1() {
        if (ADC1->STATR & ADC_JEOC) return false;
        ADC1->CTLR2 |= ADC_JSWSTART; // start conversion
        return true;
    }
    inline bool isConversionCompleteted1() {
        if (ADC1->STATR & ADC_JEOC) {
            return true;
        } else {
            return false;
        }
    }
    inline bool startConversion2() {
        if (ADC2->STATR & ADC_EOC) return false;
        ADC2->CTLR2 |= ADC_SWSTART; // start conversion
        return true;
    }
    inline bool isConversionCompleteted2() {
        if (ADC2->STATR & ADC_EOC) {
            return true;
        } else {
            return false;
        }
    }


    GpTimer timer2{2};
    static constexpr uint8_t MovAverLength = 128;
    static constexpr uint8_t MovAverGrade() { return log2(MovAverLength); }

    uint8_t UART_BYTE = 0;

    uint16_t getU();
    uint16_t getI();

    void adcHandler1();
    void adcHandler2();
    void sortAndGetMedian();

    uint8_t averageI = 0;
    uint8_t averageU = 0;

  private:
    void init();
    int8_t Calibrattion_Val = 0;
    uint16_t tempVal = 0;

    void init_injected_channels_dma();
    uint8_t Index_mov_aver_U = 0;
    uint8_t Index_mov_aver_I = 0;
    uint16_t MovAverSum_U = 0;
    uint16_t MovAverSum_I = 0;

    uint16_t arr_U[MovAverLength] = {0};
    uint16_t arr_I[MovAverLength] = {0};
    uint8_t counterADC = 0;
    uint32_t sumI = 0;
    uint32_t sumU = 0;
    static constexpr uint8_t MedianMiddle = 2;
    static constexpr uint8_t medianArrLength = 10;
    uint16_t moving_average_U(uint16_t val);
    uint16_t moving_average_I(uint16_t val);

    static constexpr uint8_t kU_mul = 100;
    static constexpr uint8_t kU_div = 152;
    static constexpr uint8_t kI_mul = 100;
    static constexpr uint8_t kI_div = 152;
};

#endif // ADC_H
