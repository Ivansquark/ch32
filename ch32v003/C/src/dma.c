#include "dma.h"

uint16_t volatile dma_ch1_buff[DMA_CH1_BUF_SIZE] = {0};

void dma_ch1_adc1_init()
{
    // start
    RCC->AHBPCENR |= RCC_DMA1EN;

    DMA1_Channel1->CFGR |= DMA_CFG4_PL; //highest priority
    DMA1_Channel1->CFGR |= DMA_CFG4_PSIZE_0;
    DMA1_Channel1->CFGR &= ~DMA_CFG4_PSIZE_1; // 0:1 16 bits
    DMA1_Channel1->CFGR |= DMA_CFG4_MSIZE_0;
    DMA1_Channel1->CFGR &= ~DMA_CFG4_MSIZE_1; // 0:1 16 bits

    DMA1_Channel1->CFGR |= DMA_CFG4_MINC;
    DMA1_Channel1->CFGR &= ~DMA_CFG4_PINC;
    DMA1_Channel1->CFGR &= ~DMA_CFG4_DIR; // 0 - from peripheral

    DMA1_Channel1->CNTR = DMA_CH1_BUF_SIZE;

    DMA1_Channel1->PADDR = (uint32_t)(long)(&ADC1->RDATAR);
    DMA1_Channel1->MADDR = (uint32_t)(long)(dma_ch1_buff);
}
void dma_ch1_enable() { DMA1_Channel1->CFGR |= DMA_CFG4_EN; }
void dma_ch1_disable() { DMA1_Channel1->CFGR &= ~DMA_CFG4_EN; }
