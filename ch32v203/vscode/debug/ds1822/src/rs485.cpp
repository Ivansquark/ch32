#include "rs485.h"
RS485* RS485::pThis = nullptr;

RS485::RS485() {
    pThis = this;
    init();
}

void RS485::writeBytes(const uint8_t* arr, uint8_t len) {
    for (int i = 0; i < len; i++) {
        writeByte(arr[i]);
    }
}

void RS485::writeByte(uint8_t byte) {
    setWrite();
    uint32_t timeout = 0xFFFF;
    while ((!(USART1->STATR & USART_STATR_TXE)) && timeout--) {}
    USART1->DATAR = byte;
    timeout = 0xFFFF;
    while ((!(USART1->STATR & USART_STATR_TXE)) && timeout--) {}
    setRead();
}

void RS485::init() {
    // PA9-Tx PA10-Rx - no remap (not used now)

    RCC->APB2PCENR |= RCC_IOPBEN;
    // B6-Tx ALTERNATE_PUSH_PULL - 1:0:1:1
    GPIOB->CFGLR |= GPIO_CFGLR_MODE6;
    GPIOB->CFGLR &= ~GPIO_CFGLR_CNF6;
    GPIOB->CFGLR |= GPIO_CFGLR_CNF6_1;
    // B7-Rx Uart PULL_UP - 0:1:0:0
    GPIOB->CFGLR |= GPIO_CFGLR_MODE7;
    GPIOB->CFGLR |= GPIO_CFGLR_CNF7_1;
    GPIOB->CFGLR &= ~GPIO_CFGLR_CNF7_0;
    GPIOB->OUTDR |= GPIO_OUTDR_ODR7;
    
    // PB6-Tx PB7-Rx Uart remap
    RCC->APB2PCENR |= RCC_AFIOEN;
    AFIO->PCFR1 |= AFIO_PCFR1_USART1_REMAP;

    RCC->APB2PCENR |= RCC_USART1EN;
    USART1->CTLR1 |=
        USART_CTLR1_TE | USART_CTLR1_RE | USART_CTLR1_RXNEIE | USART_CTLR1_UE;
    // 8 MHz 9600 KB/s
    // 9600 = 8000000/(16*USARTDIV) =>
    // USARTDIV = 8000000/(16*9600) = 52,08
    // USARTDIV = DIV_M+(DIV_F/16) = 52 + (0*0.25) = 52_M + 0_F
    USART1->BRR |= (52 << 4); // 9600 on 8MHz
    NVIC_EnableIRQ(USART1_IRQn);
}

extern "C" __attribute__((interrupt)) void USART1_IRQHandler(void) {
    if (USART1->STATR & USART_STATR_RXNE) {
        USART1->STATR &= ~USART_STATR_RXNE;
        RS485::pThis->counter_ms = 0;
        // receive 5 bytes
        if (RS485::pThis->getIsNeedRead()) {
            RS485::pThis->arr[RS485::pThis->counterBytes] = USART1->DATAR;
            RS485::pThis->counterBytes++;
            if (RS485::pThis->counterBytes >= RS485::SIZE_RECEIVE) {
                // TODO: set flag to parse data
                RS485::pThis->setIsNeedRead(false);
                RS485::pThis->counterBytes = 0;
            }
        }
    }
}
