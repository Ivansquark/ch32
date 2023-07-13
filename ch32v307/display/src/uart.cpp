#include "uart.h"

Uart5* Uart5::pThis = nullptr;

Uart5::Uart5() {
    init();
    pThis = this;
}
void Uart5::sendByte(uint8_t byte) {
    uint32_t timeout = 0xFFFF;
    while ((!(UART5->STATR & USART_STATR_TXE)) && timeout--) {}
    UART5->DATAR = byte;
}
void Uart5::sendStr(const char* str) {
    uint8_t i = 0;
    while (str[i] != '\0') { sendByte(str[i++]); }
}

void Uart5::sendBuf(uint8_t* data, uint8_t len) {
    for (int i = 0; i < len; i++) { sendByte(data[i]); }
}
void Uart5::init() {
    //! _______USART 3 init  pin D8-Tx D9-Rx AF7 remap 1:1  ___________________
    RCC->APB2PCENR |= RCC_IOPBEN;
    // Tx AF-push/pull
    GPIOB->CFGLR &= ~GPIO_CFGLR_CNF4_0;
    GPIOB->CFGLR |= GPIO_CFGLR_CNF4_1;
    GPIOB->CFGLR |= GPIO_CFGLR_MODE4;
    // Rx in_floating
    GPIOB->CFGLR &= ~GPIO_CFGLR_CNF5_1;
    GPIOB->CFGLR |= GPIO_CFGLR_CNF5_0;
    GPIOB->CFGLR &= ~GPIO_CFGLR_MODE5;
    
    RCC->APB2PCENR |= RCC_AFIOEN;
    // 0:1 remap uart5 on b4,b5
    AFIO->PCFR2 |= AFIO_PCFR2_UART5_REMAP_01;

    //------------------- USART -------------------------------------
    RCC->APB1PCENR |= RCC_UART5EN;
    UART5->CTLR1 |=
        USART_CTLR1_TE | USART_CTLR1_RE | USART_CTLR1_RXNEIE | USART_CTLR1_UE;
    // 72 MHz 9600 KB/s
    // 9600 = 72000000/(16*USARTDIV) =>
    // USARTDIV = 72000000/(16*9600) = 468,75
    // USARTDIV = DIV_M+(DIV_F/16) = 468 + (16*0.75) = 468_M + 12_F
    //UART5->BRR |= (468 << 4) | (12); // 9600
    // 144 MHz 9600 KB/s
    // 9600 = 144000000/(16*USARTDIV) =>
    // USARTDIV = 144000000/(16*9600) = 937,5
    // USARTDIV = DIV_M+(DIV_F/16) = 937 + (16*0.5) = 937_M + 8_F
    UART5->BRR |= (937 << 4) | (8); // 9600
    NVIC_EnableIRQ(UART5_IRQn);
}

extern "C" __attribute__((interrupt)) void UART5_IRQHandler(void) {
    if (UART5->STATR & USART_STATR_RXNE) {
        UART5->STATR &= ~USART_STATR_RXNE;
    }
}

Uart1* Uart1::pThis = nullptr;

Uart1::Uart1() {
    init();
    pThis = this;
}
void Uart1::sendByte(uint8_t byte) {
    uint32_t timeout = 0xFFFF;
    while ((!(USART1->STATR & USART_STATR_TXE)) && timeout--) {}
    USART1->DATAR = byte;
}
void Uart1::sendStr(const char* str) {
    uint8_t i = 0;
    while (str[i] != '\0') { sendByte(str[i++]); }
}

void Uart1::sendBuf(uint8_t* data, uint8_t len) {
    for (int i = 0; i < len; i++) { sendByte(data[i]); }
}
void Uart1::init() {
    //! _______USART 1 init  pin A9-Tx A10-Rx _________________________________
    RCC->APB2PCENR |= RCC_IOPAEN;
    // Tx AF-push/pull
    GPIOA->CFGHR &= ~GPIO_CFGHR_CNF9_0;
    GPIOA->CFGHR |= GPIO_CFGHR_CNF9_1;
    GPIOA->CFGHR |= GPIO_CFGHR_MODE9;
    // Rx in_floating
    GPIOB->CFGHR &= ~GPIO_CFGHR_CNF10_1;
    GPIOB->CFGHR |= GPIO_CFGHR_CNF10_0;
    GPIOB->CFGHR &= ~GPIO_CFGHR_MODE10;

    //------------------------- USART -------------------------------------
    RCC->APB1PCENR |= RCC_USART1EN;
    USART1->CTLR1 |=
        USART_CTLR1_TE | USART_CTLR1_RE | USART_CTLR1_RXNEIE | USART_CTLR1_UE;
    // 72 MHz 9600 KB/s
    // 9600 = 72000000/(16*USARTDIV) =>
    // USARTDIV = 72000000/(16*9600) = 468,75
    // USARTDIV = DIV_M+(DIV_F/16) = 468 + (16*0.75) = 468_M + 12_F
    //UART5->BRR |= (468 << 4) | (12); // 9600
    // 144 MHz 9600 KB/s
    // 9600 = 144000000/(16*USARTDIV) =>
    // USARTDIV = 144000000/(16*9600) = 937,5
    // USARTDIV = DIV_M+(DIV_F/16) = 937 + (16*0.5) = 937_M + 8_F
    USART1->BRR |= (937 << 4) | (8); // 9600
    NVIC_EnableIRQ(USART1_IRQn);
}

extern "C" __attribute__((interrupt)) void USART1_IRQHandler(void) {
    if (USART1->STATR & USART_STATR_RXNE) {
        USART1->STATR &= ~USART_STATR_RXNE;
        Uart1::pThis->rxBuf[Uart1::pThis->rxCounter] = USART1->DATAR;
    }
}
//-----------------------------------------------------------------------------
Uart3* Uart3::pThis = nullptr;

Uart3::Uart3() {
    init();
    pThis = this;
}
void Uart3::sendByte(uint8_t byte) {
    uint32_t timeout = 0xFFFF;
    while ((!(USART3->STATR & USART_STATR_TXE)) && timeout--) {}
    USART3->DATAR = byte;
}
void Uart3::sendStr(const char* str) {
    uint8_t i = 0;
    while (str[i] != '\0') { sendByte(str[i++]); }
}
void Uart3::sendBuf(uint8_t* data, uint8_t len) {
    for (int i = 0; i < len; i++) { sendByte(data[i]); }
}
void Uart3::init() {
    //! _______USART 3 init  pin B10-Tx B11-Rx ________________________________
    RCC->APB2PCENR |= RCC_IOPBEN;
    // Tx AF-push/pull
    GPIOB->CFGHR &= ~GPIO_CFGHR_CNF10_0;
    GPIOB->CFGHR |= GPIO_CFGHR_CNF10_1;
    GPIOB->CFGHR |= GPIO_CFGHR_MODE10;
    // Rx in_floating
    GPIOB->CFGHR &= ~GPIO_CFGHR_CNF11_1;
    GPIOB->CFGHR |= GPIO_CFGHR_CNF11_0;
    GPIOB->CFGHR &= ~GPIO_CFGHR_MODE11;

    //------------------- USART -------------------------------------
    RCC->APB1PCENR |= RCC_UART5EN;
    USART3->CTLR1 |=
        USART_CTLR1_TE | USART_CTLR1_RE | USART_CTLR1_RXNEIE | USART_CTLR1_UE;
    // 72 MHz 9600 KB/s
    // 9600 = 72000000/(16*USARTDIV) =>
    // USARTDIV = 72000000/(16*9600) = 468,75
    // USARTDIV = DIV_M+(DIV_F/16) = 468 + (16*0.75) = 468_M + 12_F
    //UART5->BRR |= (468 << 4) | (12); // 9600
    // 144 MHz 9600 KB/s
    // 9600 = 144000000/(16*USARTDIV) =>
    // USARTDIV = 144000000/(16*9600) = 937,5
    // USARTDIV = DIV_M+(DIV_F/16) = 937 + (16*0.5) = 937_M + 8_F
    USART3->BRR |= (937 << 4) | (8); // 9600
    NVIC_EnableIRQ(USART3_IRQn);
}

extern "C" __attribute__((interrupt)) void USART3_IRQHandler(void) {
    if (USART3->STATR & USART_STATR_RXNE) {
        USART3->STATR &= ~USART_STATR_RXNE;
        Uart3::pThis->rxBuf[Uart3::pThis->rxCounter] = USART3->DATAR;
    }
}
