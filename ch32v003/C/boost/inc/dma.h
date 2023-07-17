#ifndef DMA_H
#define DMA_H

#include "main.h"
#include <string.h>

#define DMA_CH1_BUF_SIZE 32

extern volatile uint16_t dma_ch1_buff[DMA_CH1_BUF_SIZE];
void dma_ch1_adc1_init();
void dma_ch1_enable();
void dma_ch1_disable();

#endif // DMA_H
