#ifndef ADC_H
#define ADC_H

#include "ch32v30x_adc.h"
#include "ch32v30x_rcc.h"
#include "main.h"

class Adc {
  public:
    Adc();
    static Adc* pThis;
    uint16_t getAdc();
    inline bool startConversion() {
        if (ADC1->STATR & ADC_EOC) return false;
        ADC1->CTLR2 |= ADC_SWSTART; // start conversion
        return true;
    }
    bool isConversionCompleteted() {
        if (ADC1->STATR & ADC_EOC) {
            return true;
        } else {
            return false;
        }
    }

  private:
    void init();
    int8_t Calibrattion_Val = 0;
    uint16_t tempVal = 0;
};

#endif // ADC_H
