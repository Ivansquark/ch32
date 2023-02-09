#ifndef ADC_H
#define ADC_H

#include "ch32v30x_adc.h"
#include "ch32v30x_rcc.h"
#include "main.h"
#include <stdbool.h>

uint16_t Adc_getAdc();
bool Adc_startConversion();
bool Adc_isConversionCompleteted();

void Adc_init();
extern int8_t Calibrattion_Val;
extern uint16_t tempVal;

#endif // ADC_H
