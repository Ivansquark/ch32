#ifndef GPIO_H
#define GPIO_H

#include "ch32v00x_rcc.h"
#include "main.h"

#define A0_set() (GPIOA->BSHR = GPIO_BSHR_BS0)
#define A0_reset() (GPIOA->BSHR = GPIO_BSHR_BR0)

void gpio_init();

#endif // GPIO_H
