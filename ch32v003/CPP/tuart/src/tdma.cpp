#include "tdma.h"

namespace TDma {

char data[SIZE] = {0};
volatile bool IsWriting = false;

} // namespace TDma
  //
template <>
void TDma::write<TDma::Dma1>(const char* arr, uint8_t len) {
    IsWriting = true;
    DMA1_Channel4->CFGR &= ~DMA_CFGR2_EN;
    if (len < SIZE) {
        memcpy(data, arr, len);
    }
    DMA1_Channel4->CNTR = len;
    DMA1_Channel4->MADDR = (uint32_t)data;
    DMA1_Channel4->CFGR |= DMA_CFGR2_EN;
}

extern "C" __attribute__((interrupt)) void DMA1_Channel4_IRQHandler() {
    if (DMA1->INTFR & DMA_TCIF4) {
        USART1->CTLR1 |= USART_CTLR1_TCIE;
        DMA1->INTFCR |= DMA_CTCIF4;
    }
}
