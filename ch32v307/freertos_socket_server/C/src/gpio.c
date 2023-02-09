#include "gpio.h"


void gpio_init() {
    RCC->APB2PCENR |= RCC_IOPAEN;
    // floating input mode
    GPIOA->CFGLR |= GPIO_CFGLR_MODE0;
    GPIOA->CFGLR |= GPIO_CFGLR_CNF0_0;
    GPIOA->CFGLR &= ~GPIO_CFGLR_CNF0_1;

    RCC->APB2PCENR |= RCC_IOPAEN;
    GPIOA->CFGHR |= GPIO_CFGHR_MODE15;
    GPIOA->CFGHR &= ~GPIO_CFGHR_CNF15;
    RCC->APB2PCENR |= RCC_IOPBEN;
    GPIOB->CFGLR |= GPIO_CFGLR_MODE4;
    GPIOB->CFGLR &= ~GPIO_CFGLR_CNF4;
}
bool gpio_isA0() { return (GPIOA->INDR & GPIO_INDR_IDR0); }
bool gpio_isBlue() { return !(GPIOA->INDR & GPIO_INDR_IDR4); }
bool gpio_isRed() { return !(GPIOA->INDR & GPIO_INDR_IDR15); }

void gpio_setRed() { GPIOA->BSHR |= GPIO_BSHR_BS15; }
void gpio_resetRed() { GPIOA->BSHR |= GPIO_BSHR_BR15; }
void gpio_toggleRed() { GPIOA->OUTDR ^= GPIO_OUTDR_ODR15; }
void gpio_setBlue() { GPIOB->BSHR |= GPIO_BSHR_BS4; }
void gpio_resetBlue() { GPIOB->BSHR |= GPIO_BSHR_BR4; }
void gpio_toggleBlue() { GPIOB->OUTDR ^= GPIO_OUTDR_ODR4; }
