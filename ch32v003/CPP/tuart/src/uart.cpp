#include "uart.h"

uint8_t Uart::uartByte = 0;
char Uart::data[64] = {0};
char Uart::rxdata[64] = {0};
uint8_t Uart::counter = 0;
volatile bool Uart::IsReceived = false;

Uart::Uart() { init(); }

void Uart::sendStr(const char* arr, int len) {
    for (int i = 0; i < len; i++) {
        sendbyte(arr[i]);
    }
}

void Uart::init() {
    // PD5-Tx PD6-Rx
    RCC->APB2PCENR |= RCC_IOPDEN;
    // Tx AF-push/pull
    GPIOD->CFGLR &= ~GPIO_CFGLR_CNF5_0;
    GPIOD->CFGLR |= GPIO_CFGLR_CNF5_1;
    GPIOD->CFGLR |= GPIO_CFGLR_MODE5;
    // Rx in_floating
    GPIOD->CFGLR &= ~GPIO_CFGLR_CNF6_1;
    GPIOD->CFGLR |= GPIO_CFGLR_CNF6_0;
    GPIOD->CFGLR &= ~GPIO_CFGLR_MODE6;
    // remap not need

    RCC->APB2PCENR |= RCC_USART1EN;
    USART1->CTLR1 |=
        USART_CTLR1_TE | USART_CTLR1_RE | USART_CTLR1_RXNEIE | USART_CTLR1_UE;
    // 24 MHz 9600 KB/s
    // 9600 = 24000000/(16*USARTDIV) =>
    // USARTDIV = 24000000/(16*9600) = 156,25
    // USARTDIV = DIV_M+(DIV_F/16) = 156 + (16*0.25) = 156_M + 4_F
    // USART1->BRR |= (156 << 4) | (4); // 9600 on 24MHz

    // 24 MHz 115200 KB/s
    // 115200 = 24000000/(16*USARTDIV) =>
    // USARTDIV = 24000000/(16*115200) = 13.02
    // USARTDIV = DIV_M+(DIV_F/16) = 13 + (16*0.02) = 13_M + 0_F
    USART1->BRR |= (13 << 4) | (0); // 115200 on 24MHz
    // USART1->BRR |= (6 << 4) | (8); // 115200 on 24MHz
    NVIC_EnableIRQ(USART1_IRQn);
}

void Uart::sendbyte(uint8_t byte) {
    // USART1->DATAR = (byte & (uint16_t)0x01FF);
    uint16_t timeout = 0xffff;
    USART1->DATAR = byte;
    while (!(USART1->STATR & USART_STATR_TC) && timeout--) {}
    // USART1->STATR &= ~USART_STATR_TC;
}

//void USART1_IRQHandler(void) {
//    if (USART1->STATR & USART_STATR_RXNE) {
//        USART1->STATR &= ~USART_STATR_RXNE;
//        char c = USART1->DATAR;
//        Uart::rxdata[Uart::counter++] = c;
//        Uart::sendStr(&c, 1);
//        if ((Uart::rxdata[Uart::counter - 1] == '\r') ||
//            (Uart::counter == sizeof(Uart::rxdata))) {
//            char c1 = '\n';
//            Uart::sendStr(&c1, 1);
//            // parse
//            memcpy(Uart::data, Uart::rxdata, Uart::counter - 1);
//            Uart::data[Uart::counter - 1] = 0;
//            Uart::counter = 0;
//            Uart::IsReceived = true;
//        }
//    }
//}
