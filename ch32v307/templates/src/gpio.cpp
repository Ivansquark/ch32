#include "gpio.h"

namespace Gpio {


void In::init() {
    RCC->APB2PCENR |= RCC_IOPAEN;
    // floating input mode
    GPIOA->CFGLR |= GPIO_CFGLR_MODE0;
    GPIOA->CFGLR |= GPIO_CFGLR_CNF0_0;
    GPIOA->CFGLR &= ~GPIO_CFGLR_CNF0_1;
}

void Out::init() {
    RCC->APB2PCENR |= RCC_IOPAEN;
    GPIOA->CFGHR |= GPIO_CFGHR_MODE15;
    GPIOA->CFGHR &= ~GPIO_CFGHR_CNF15;
    RCC->APB2PCENR |= RCC_IOPBEN;
    GPIOB->CFGLR |= GPIO_CFGLR_MODE4;
    GPIOB->CFGLR &= ~GPIO_CFGLR_CNF4;
}

} // namespace Gpio
