#ifndef GPIO_H
#define GPIO_H

#include "main.h"

void gpio_init();
bool gpio_isA0();
bool gpio_isBlue();
bool gpio_isRed();

void gpio_setRed();
void gpio_resetRed();
void gpio_toggleRed();
void gpio_setBlue();
void gpio_resetBlue();
void gpio_toggleBlue();

#endif // GPIO_H
