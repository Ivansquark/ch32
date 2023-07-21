#include "uart.h"

void uart_init()
{
    // PC0-Tx PC1-Rx USART1
    RCC->APB2PCENR |= RCC_IOPCEN;
    // Tx AF-push/pull
    GPIOC->CFGLR &= ~GPIO_CFGLR_CNF0_0;
    GPIOC->CFGLR |= GPIO_CFGLR_CNF0_1;
    GPIOC->CFGLR |= GPIO_CFGLR_MODE0;
    // Rx in_pull_up
    GPIOC->CFGLR &= ~GPIO_CFGLR_CNF1_0;
    GPIOC->CFGLR |= GPIO_CFGLR_CNF1_1;
    GPIOC->CFGLR &= ~GPIO_CFGLR_MODE1;
    GPIOC->OUTDR |= GPIO_OUTDR_ODR1;

    RCC->APB2PCENR |= RCC_AFIOEN;
    // 0:1 remap uart5 on b4,b5
    AFIO->PCFR1 |= AFIO_PCFR1_USART1_REMAP;
    AFIO->PCFR1 |= (1 << 21) | (1 << 2);

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

void uart_sendbyte(uint8_t byte)
{
    // USART1->DATAR = (byte & (uint16_t)0x01FF);
    uint16_t timeout = 0xffff;
    USART1->DATAR = byte;
    while (!(USART1->STATR & USART_STATR_TC) && timeout--) {}
    // USART1->STATR &= ~USART_STATR_TC;
}

uint8_t uartByte = 0;
void USART1_IRQHandler(void)
{
    if (USART1->STATR & USART_STATR_RXNE) {
        USART1->STATR &= ~USART_STATR_RXNE;
        uartByte = USART1->DATAR;
    }
}
