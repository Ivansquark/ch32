#ifndef GPIO_H
#define GPIO_H

#include "ch32v00x_rcc.h"
#include "main.h"

#define A0_set() (GPIOA->BSHR = GPIO_BSHR_BS0)
#define A0_reset() (GPIOA->BSHR = GPIO_BSHR_BR0)
#define A1_set() (GPIOA->BSHR = GPIO_BSHR_BS1)
#define A1_reset() (GPIOA->BSHR = GPIO_BSHR_BR1)

#define C1_set() (GPIOC->BSHR = GPIO_BSHR_BS1)
#define C1_reset() (GPIOC->BSHR = GPIO_BSHR_BR1)
#define C2_set() (GPIOC->BSHR = GPIO_BSHR_BS1)
#define C2_reset() (GPIOC->BSHR = GPIO_BSHR_BR1)

#define Is_C3() (GPIOC->INDR & GPIO_INDR_IDR3)

void gpio_init();

#endif // GPIO_H
