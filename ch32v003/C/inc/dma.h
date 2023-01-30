#ifndef DMA_H
#define DMA_H

#include "main.h"

#define DMA_CH1_BUF_SIZE 32

extern volatile uint16_t dma_ch1_buff[32];
void dma_ch1_adc1_init();
void dma_ch1_enable();
void dma_ch1_disable();

#endif // DMA_H
