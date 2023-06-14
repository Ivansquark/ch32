#include "gpio.h"

namespace Gpio {

void Out::init() {
    // CS-B12 RS-B13 WR-B14 RD-B15 p/p
    RCC->APB2PCENR |= RCC_IOPBEN;
    GPIOB->CFGHR |= GPIO_CFGHR_MODE12 | GPIO_CFGHR_MODE13 | GPIO_CFGHR_MODE14 |
                    GPIO_CFGHR_MODE15;
    GPIOB->CFGHR &= ~(GPIO_CFGHR_CNF12 | GPIO_CFGHR_CNF13 | GPIO_CFGHR_CNF14 |
                      GPIO_CFGHR_CNF15);
    // RST-C10 PEN-B13 T_CS p/p
    RCC->APB2PCENR |= RCC_IOPCEN;
    GPIOC->CFGHR |= GPIO_CFGHR_MODE12 | GPIO_CFGHR_MODE13 | GPIO_CFGHR_MODE14 |
                    GPIO_CFGHR_MODE15;
    GPIOC->CFGHR &= ~(GPIO_CFGHR_CNF12 | GPIO_CFGHR_CNF13 | GPIO_CFGHR_CNF14 |
                      GPIO_CFGHR_CNF15);
    // D0-D15 p/p 0:0:1:1
    RCC->APB2PCENR |= RCC_IOPDEN;
    GPIOD->CFGLR = 0x03030303;
    GPIOD->CFGHR = 0x03030303;
    GPIOD->OUTDR = 0;
    
}

void In::init() {
    
}

} // namespace Gpio
