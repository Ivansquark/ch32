#include "uart.h"
Uart* Uart::pThis = nullptr;

Uart::Uart() {
    pThis = this;
    init();
}
void Uart::sendByte(uint8_t byte) {
    uint32_t timeout = 0xFFFF;
    while ((!(USART3->STATR & USART_STATR_TXE)) && timeout--) {}
    USART3->DATAR = byte;
}
void Uart::sendStr(const char* str) {
    uint8_t i = 0;
    while (str[i] != '\0') { sendByte(str[i++]); }
}

void Uart::sendBuf(uint8_t* data, uint8_t len) {
    for (int i = 0; i < len; i++) { sendByte(data[i]); }
}

void Uart::init() {

    //! _______USART 3 init  pin D8-Tx D9-Rx AF7 remap 1:1  ___________________
    RCC->APB2PCENR |= RCC_IOPDEN;
    // Tx AF-push/pull
    GPIOD->CFGHR &= ~GPIO_CFGHR_CNF8_0;
    GPIOD->CFGHR |= GPIO_CFGHR_CNF8_1;
    GPIOD->CFGHR |= GPIO_CFGHR_MODE8;
    // Rx in_floating
    GPIOD->CFGHR &= ~GPIO_CFGHR_CNF9_1;
    GPIOD->CFGHR |= GPIO_CFGHR_CNF9_0;
    GPIOD->CFGHR &= ~GPIO_CFGHR_MODE9;
    // 1:1 remap usart3 on D8,D9
    AFIO->PCFR1 |= AFIO_PCFR1_USART3_REMAP;

    //------------------- USART -------------------------------------
    RCC->APB1PCENR |= RCC_USART3EN;
    USART3->CTLR1 |=
        USART_CTLR1_TE | USART_CTLR1_RE | USART_CTLR1_RXNEIE | USART_CTLR1_UE;
    // 72 MHz 9600 KB/s
    // 9600 = 72000000/(16*USARTDIV) =>
    // USARTDIV = 72000000/(16*9600) = 468,75
    // USARTDIV = DIV_M+(DIV_F/16) = 468 + (16*0.75) = 468_M + 12_F
    USART3->BRR |= (468 << 4) | (12); // 9600 on 24MHz
    NVIC_EnableIRQ(USART3_IRQn);
}

void USART3_IRQHandler(void) {
    if (USART3->STATR & USART_STATR_RXNE) {
        USART3->STATR &= ~USART_STATR_RXNE;
        Uart::pThis->tempBuff[Uart::pThis->tempBuffCounter] = USART3->DATAR;
    }
}
