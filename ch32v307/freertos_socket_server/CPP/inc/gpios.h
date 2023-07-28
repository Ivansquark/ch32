#ifndef GPIOS_H
#define GPIOS_H

#include "ch32v30x.h"

namespace Gpios {

void init_in();

enum PortName
{
    PA,
    PB,
    PC,
    PD,
    PE,
    PF
};

enum InitModeIn
{
    ANALOG,
    FLOATING,
    PULL_UP,
    PULL_DOWN
};

template <PortName portName, uint8_t num> class In {
  public:
    static constexpr GPIO_TypeDef* whichGPIO(PortName name) {
        if (PA == name) {
            return GPIOA;
        } else if (PB == name) {
            return GPIOB;
        }
    }
    static constexpr void setMode(InitModeIn mode) {
        GPIO_TypeDef* name;
        //switch (portName) {
        //case PA:
        //    name = GPIOA;
        //    break;
        //}
        name = whichGPIO(portName);
        if (name == GPIOA) {
            RCC->APB2PCENR |= RCC_IOPAEN;
        } else if (name == GPIOB) {
            RCC->APB2PCENR |= RCC_IOPBEN;
        } else if (name == GPIOC) {
            RCC->APB2PCENR |= RCC_IOPCEN;
        } else if (name == GPIOD) {
            RCC->APB2PCENR |= RCC_IOPDEN;
        }
        switch (mode) {
        case ANALOG:
            switch (num) {
            case 0:
                name->CFGLR &= ~(GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0);
                break;
            case 1:
                name->CFGLR &= ~(GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1);
                break;
            case 2:
                name->CFGLR &= ~(GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2);
                break;
            case 3:
                name->CFGLR &= ~(GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3);
                break;
            case 4:
                name->CFGLR &= ~(GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4);
                break;
            case 5:
                name->CFGLR &= ~(GPIO_CFGLR_MODE5 | GPIO_CFGLR_CNF5);
                break;
            case 6:
                name->CFGLR &= ~(GPIO_CFGLR_MODE6 | GPIO_CFGLR_CNF6);
                break;
            case 7:
                name->CFGLR &= ~(GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7);
                break;
            case 8:
                name->CFGHR &= ~(GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
                break;
            case 9:
                name->CFGHR &= ~(GPIO_CFGHR_MODE9 | GPIO_CFGHR_CNF9);
                break;
            case 10:
                name->CFGHR &= ~(GPIO_CFGHR_MODE10 | GPIO_CFGHR_CNF10);
                break;
            case 11:
                name->CFGHR &= ~(GPIO_CFGHR_MODE11 | GPIO_CFGHR_CNF11);
                break;
            case 12:
                name->CFGHR &= ~(GPIO_CFGHR_MODE12 | GPIO_CFGHR_CNF12);
                break;
            case 13:
                name->CFGHR &= ~(GPIO_CFGHR_MODE13 | GPIO_CFGHR_CNF13);
                break;
            case 14:
                name->CFGHR &= ~(GPIO_CFGHR_MODE14 | GPIO_CFGHR_CNF14);
                break;
            case 15:
                name->CFGHR &= ~(GPIO_CFGHR_MODE15 | GPIO_CFGHR_CNF15);
                break;
            }
            break;
        default:
            break;
        }
    }
    static bool getState();
};

// template <typename GPIO_NAME> class Out {
//  public:
//    Out();
//};

} // namespace Gpios

#endif // GPIOS_H
