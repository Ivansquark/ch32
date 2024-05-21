#include "dma.h"

bool Dma2::IsWriting = false;

Dma2::Dma2() {
    // init
    // tx
    RCC->AHBPCENR |= RCC_DMA2EN;
    DMA2_Channel6->CFGR |= DMA_CFG6_MINC | DMA_CFG6_DIR | DMA_CFG6_TCIE;
    DMA2_Channel6->PADDR = (uint32_t)&UART6->DATAR;
    DMA2_Channel6->MADDR = (uint32_t)data6;
    // enable DMA in uart too
    UART6->CTLR3 |= USART_CTLR3_DMAT;
    UART6->CTLR3 |= USART_CTLR3_DMAR;
    DMA2->INTFCR |= DMA_CTCIF6;
    NVIC_EnableIRQ(DMA2_Channel6_IRQn);

    DMA2_Channel8->CFGR |= DMA_CFGR1_MINC | DMA_CFGR1_DIR | DMA_CFGR1_TCIE;
    DMA2_Channel8->PADDR = (uint32_t)&UART7->DATAR;
    DMA2_Channel8->MADDR = (uint32_t)data8;
    // enable DMA in uart too
    UART7->CTLR3 |= USART_CTLR3_DMAT;
    UART7->CTLR3 |= USART_CTLR3_DMAR;
    DMA2_EXTEN->INTFCR |= DMA_CTCIF8;
    NVIC_EnableIRQ(DMA2_Channel8_IRQn);
}

bool Dma2::write6(const uint8_t* arr, uint16_t len) {
    if (IsWriting) return false;
    DMA2_Channel6->CFGR &= ~DMA_CFG6_EN;
    memcpy(data6, arr, len);
    DMA2_Channel6->CNTR = len;
    DMA2_Channel6->MADDR = (uint32_t)data6;
    DMA2_Channel6->CFGR |= DMA_CFG6_EN;
    IsWriting = true;
    return true;
}

bool Dma2::write8(const uint8_t* arr, uint16_t len) {
    if (IsWriting) return false;
    DMA2_Channel8->CFGR &= ~DMA_CFGR1_EN;
    memcpy(data8, arr, len);
    DMA2_Channel8->CNTR = len;
    DMA2_Channel8->MADDR = (uint32_t)data8;
    DMA2_Channel8->CFGR |= DMA_CFGR1_EN;
    IsWriting = true;
    return true;
}

extern "C" __attribute__((interrupt)) void DMA2_Channel6_IRQHandler() {
    if (DMA2->INTFR & DMA_TCIF6) {
        // Dma2::IsWriting = false;
        UART6->CTLR1 |= USART_CTLR1_TCIE;
        DMA2->INTFCR |= DMA_CTCIF6;
    }
}
extern "C" __attribute__((interrupt)) void DMA2_Channel8_IRQHandler() {
    if (DMA2_EXTEN->INTFR & DMA_TCIF8) {
        UART7->CTLR1 |= USART_CTLR1_TCIE;
        // DMA2_EXTEN->INTFCR |= DMA_CTCIF8; // CMSIS have not that reg
        DMA2_EXTEN->INTFCR |= 2;
    }
}
