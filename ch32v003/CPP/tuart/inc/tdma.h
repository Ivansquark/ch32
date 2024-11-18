#ifndef TDMA_H
#define TDMA_H

#include "ch32v00x.h"
#include <string.h>

namespace TDma {
enum Dmas {
    Dma1,
    Dma2
};

constexpr auto SIZE = 32;
extern char data[SIZE];
extern volatile bool IsWriting;

template <Dmas dma>
void write(const char* arr, uint8_t len);


template <Dmas num, int chan>
concept IsUartTx = (num == Dma1 && chan == 4);

template <Dmas num, int chan>
    requires IsUartTx<num, chan>
class Dma {
  public:
    Dma() {
        // start
        RCC->AHBPCENR |= RCC_DMA1EN;

        DMA1_Channel4->CFGR |= DMA_CFG4_MINC | DMA_CFG4_DIR | DMA_CFG4_TCIE;
        DMA1_Channel4->PADDR = (uint32_t)&USART1->DATAR;
        DMA1_Channel4->MADDR = (uint32_t)data;

        USART1->CTLR3 |= USART_CTLR3_DMAT;
        USART1->CTLR3 |= USART_CTLR3_DMAR;

        DMA1->INTFCR |= DMA_CTCIF4;
        NVIC_EnableIRQ(DMA1_Channel4_IRQn);

        memset((void*)data, 0, SIZE);
    }
};
} // namespace TDma

#endif // TDMA_H
