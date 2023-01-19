#include "gpio.h"

void gpio_init()
{
    RCC->AHBPCENR |= RCC_IOPAEN;
    GPIOA->CFGLR |= GPIO_CFGLR_MODE0;
    GPIOA->CFGLR &= ~GPIO_CFGLR_CNF0;

    GPIOA->BSHR = GPIO_BSHR_BR0;
}
