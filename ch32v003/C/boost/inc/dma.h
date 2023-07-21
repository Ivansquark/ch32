#ifndef DMA_H
#define DMA_H

#include "main.h"
#include <string.h>

// one conversion is 239 us => buffer will fill with 239 * DMA_CH1_BUF_SIZE us
#define DMA_CH1_BUF_SIZE 128 // 30 ms

extern volatile uint16_t dma_ch1_buff[DMA_CH1_BUF_SIZE];
void dma_ch1_adc1_init();
void dma_ch1_enable();
void dma_ch1_disable();

#endif // DMA_H
