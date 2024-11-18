#include "tuart.h"

namespace TUart {
volatile bool IsReceived = false;
// bool IsReceived = false;
char data[32] = {0};
char rxdata[32] = {0};
int counter = 0;

} // namespace TUart
  //
template <>
void TUart::sendByte<TUart::UART1_NO_DMA>(char byte) {
    static_assert(getVal());
    uint16_t timeout = 0xffff;
    USART1->DATAR = byte;
    while (!(USART1->STATR & USART_STATR_TC) && timeout--) {}
}

template <>
void TUart::sendStr<TUart::UART1_NO_DMA>(const char* arr, int len) {
    for (int i = 0; i < len; i++) {
        sendByte<UART1_NO_DMA>(arr[i]);
    }
}

template <>
void TUart::sendByte<TUart::UART1_DMA>(char byte) {
    while (TDma::IsWriting) {}
    TDma::write<TDma::Dma1>(&byte, 1);
}
template <>
void TUart::sendStr<TUart::UART1_DMA>(const char* arr, int len) {
    while (TDma::IsWriting) {}
    TDma::write<TDma::Dma1>(arr, len);
    //for (int i = 0; i < len; i++) {
    //    sendByte<UART1_DMA>(arr[i]);
    //}
}

void TUart::sendStr(const char* arr, int len) {
    if constexpr (Uart1Dma == Uarts::UART1_NO_DMA) {
        sendStr<TUart::UART1_NO_DMA>(arr, len);
    } else if constexpr (Uart1Dma == Uarts::UART1_DMA) {
        sendStr<TUart::UART1_DMA>(arr, len);
    }
}

extern "C" __attribute__((interrupt("WCH-Interrupt-fast"))) void
USART1_IRQHandler(void) {
    if (USART1->STATR & USART_STATR_RXNE) {
        USART1->STATR &= ~USART_STATR_RXNE;
        char c = USART1->DATAR;
        TUart::rxdata[TUart::counter++] = c;
        TUart::sendStr<TUart::UART1_NO_DMA>(&c, 1);
        if ((TUart::rxdata[TUart::counter - 1] == '\r') ||
            (TUart::counter == sizeof(TUart::rxdata))) {
            char c1 = '\n';
            TUart::sendStr<TUart::UART1_NO_DMA>(&c1, 1);
            memcpy(TUart::data, TUart::rxdata, TUart::counter - 1);
            TUart::data[TUart::counter - 1] = 0;
            TUart::counter = 0;
            // parse
            TUart::IsReceived = true;
        }
    }

    //------------- send handler ----------------------------------------------
    if (USART1->STATR & USART_STATR_TC) {
        USART1->STATR &= ~USART_STATR_TC;
        TDma::IsWriting = false;
        USART1->CTLR1 &= ~USART_CTLR1_TCIE;
    }
}
