#ifndef STEP_H
#define STEP_H

#include "main.h"
#include "pwm.h"
#include "adc.h"
#include "dma.h"

#define swap(a, b) ((&(a) == &(b)) ? (a) : ((a)^=(b),(b)^=(a),(a)^=(b)))

extern volatile uint16_t dma_ch1_buff[DMA_CH1_BUF_SIZE];

void quickSort(uint16_t* A, int p,int len);
uint16_t getMedianVal();

void step_init();

void step_forward();
void step_backward();
void step_stop();

void step_handler();

#endif //STEP_H
