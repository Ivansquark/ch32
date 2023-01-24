#include "uart.h"

void uart_init()
{
    // PD5-Tx PD6-Rx
    RCC->APB2PCENR |= RCC_IOPDEN;
    GPIOD->CFGLR &= ~(GPIO_CFGLR_CNF5_0 | GPIO_CFGLR_CNF6);
    GPIOD->CFGLR |= GPIO_CFGLR_CNF5_1 | GPIO_CFGLR_CNF6_1;
    GPIOD->CFGLR |= GPIO_CFGLR_MODE5 | GPIO_CFGLR_MODE6;
    // remap not need

    RCC->APB2PCENR |= RCC_USART1EN;
    USART1->CTLR1 |=
        USART_CTLR1_TE | USART_CTLR1_RE | USART_CTLR1_RXNEIE | USART_CTLR1_UE;
    // 24 MHz 9600 KB/s
    // 9600 = 24000000/(16*USARTDIV) =>
    // USARTDIV = 24000000/(16*9600) = 156,25
    // USARTDIV = DIV_M+(DIV_F/16) = 156 + (16*0.25) = 156_M + 4_F
    USART1->BRR |= (156 << 4) | (4); // 9600 on 24MHz
    NVIC_EnableIRQ(USART1_IRQn);
}

void uart_sendbyte(uint8_t byte) { USART1->DATAR = byte; }


uint8_t uartByte = 0;
void USART1_IRQHandler(void) {
    if(USART1->STATR & USART_STATR_RXNE) {
    USART1->STATR &= ~ USART_STATR_RXNE;
        uartByte = USART1->DATAR;
    }
}
