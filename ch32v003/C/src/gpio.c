#include "gpio.h"

void gpio_init()
{
    RCC->APB2PCENR |= RCC_IOPAEN;
    GPIOA->CFGLR |= GPIO_CFGLR_MODE0;
    GPIOA->CFGLR &= ~GPIO_CFGLR_CNF0;

    GPIOA->CFGLR |= GPIO_CFGLR_MODE1;
    GPIOA->CFGLR &= ~GPIO_CFGLR_CNF1;
    
    RCC->APB2PCENR |= RCC_IOPCEN;
    //C1 C2 out
    GPIOC->CFGLR |= GPIO_CFGLR_MODE1 | GPIO_CFGLR_MODE2;
    GPIOC->CFGLR &= ~(GPIO_CFGLR_CNF1 | GPIO_CFGLR_CNF2);
    //C3 in
    GPIOC->CFGLR |= GPIO_CFGLR_MODE3;
    GPIOC->CFGLR &= ~GPIO_CFGLR_CNF3;
}
