#include "dac.h"

Dac *Dac::pThis = nullptr;

Dac::Dac() {
    pThis = this;
    init();
}

void Dac::init() {
    /*!< U-PA4 I-PA5 dac-output >*/
    RCC->APB2PCENR |= RCC_IOPAEN;
    // 1:0:1:1 AF_push/pull max speed
    GPIOA->CFGLR &= ~(GPIO_CFGLR_CNF4 | GPIO_CFGLR_CNF5);
    GPIOA->CFGLR |= GPIO_CFGLR_CNF4_1 | GPIO_CFGLR_CNF5_1;
    GPIOA->CFGLR |= GPIO_CFGLR_MODE4 | GPIO_CFGLR_MODE5;
    RCC->APB1PCENR |= (1 << 29); //RCC_DACEN;
    DAC->CTLR |= DAC_EN1; // DAC channel 1 enabled
    DAC->DOR1 = 0; // DAC channel1 12-bit right-aligned data holding register
    setBacklight(50);
}
