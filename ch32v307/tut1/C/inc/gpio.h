#ifndef GPIO_H
#define GPIO_H

#include "main.h"

#define Is_C3() ((GPIOC->INDR) & (GPIO_INDR_IDR3))
#define setRed() (GPIOA->BSHR |= GPIO_BSHR_BS15)
#define resetRed() (GPIOA->BSHR |= GPIO_BSHR_BR15)
#define setBlue() (GPIOB->BSHR |= GPIO_BSHR_BS4)
#define resetBlue() (GPIOB->BSHR |= GPIO_BSHR_BR4)

void gpio_init();

#endif // GPIO_H
