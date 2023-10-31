#ifndef GPIOS_H
#define GPIOS_H

#include "ch32v30x.h"

namespace Gpios {
inline void initLedGreen() {
    RCC->APB2PCENR |= RCC_IOPBEN;
    GPIOB->CFGLR |= GPIO_CFGLR_MODE4;
    GPIOB->CFGLR &= ~GPIO_CFGLR_CNF4;
}

inline void setLedGreen() { GPIOB->BSHR = GPIO_BSHR_BS4; }

inline void resetLedGreen() { GPIOB->BSHR = GPIO_BSHR_BR4; }

using io = volatile uint32_t;

constexpr uint32_t CFGLR = 0x0;
constexpr uint32_t CFGHR = 0x4;
constexpr uint32_t INDR = 0x8;
constexpr uint32_t OUTDR = 0xC;
constexpr uint32_t BSHR = 0x10;

enum PortName
{
    PA,
    PB,
    PC,
    PD,
    PE
};

enum InitModeIn : uint8_t
{
    ANALOG,
    FLOATING,
    PULL_DOWN,
    PULL_UP
};

enum InitModeOut
{
    PUSH_PULL,
    OPEN_DRAIN,
    ALTERNATE_PUSH_PULL,
    ALTERNATE_OPEN_DRAIN
};

template <PortName portName, uint8_t num, InitModeIn mode = FLOATING> class In {
  public:
    In() {
        setRCC();
        setMode();
    }

    static constexpr void setMode() {
        GPIO_TypeDef* name = whichGPIO();
        switch (mode) {
        case ANALOG:
            switch (num) {
            case 0:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0);
                break;
            case 1:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1);
                break;
            case 2:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2);
                break;
            case 3:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3);
                break;
            case 4:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4);
                break;
            case 5:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE5 | GPIO_CFGLR_CNF5);
                break;
            case 6:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE6 | GPIO_CFGLR_CNF6);
                break;
            case 7:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7);
                break;
            case 8:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
                break;
            case 9:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE9 | GPIO_CFGHR_CNF9);
                break;
            case 10:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE10 | GPIO_CFGHR_CNF10);
                break;
            case 11:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE11 | GPIO_CFGHR_CNF11);
                break;
            case 12:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE12 | GPIO_CFGHR_CNF12);
                break;
            case 13:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE13 | GPIO_CFGHR_CNF13);
                break;
            case 14:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE14 | GPIO_CFGHR_CNF14);
                break;
            case 15:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE15 | GPIO_CFGHR_CNF15);
                break;
            }
            break;
        case FLOATING:
            switch (num) {
            case 0:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF0_0;
                break;
            case 1:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF1_0;
                break;
            case 2:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF2_0;
                break;
            case 3:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF3_0;
                break;
            case 4:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF4_0;
                break;
            case 5:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE5 | GPIO_CFGLR_CNF5);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF5_0;
                break;
            case 6:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE6 | GPIO_CFGLR_CNF6);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF6_0;
                break;
            case 7:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF7_0;
                break;
            case 8:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF8_0;
                break;
            case 9:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE9 | GPIO_CFGHR_CNF9);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF9_0;
                break;
            case 10:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE10 | GPIO_CFGHR_CNF10);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF10_0;
                break;
            case 11:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE11 | GPIO_CFGHR_CNF11);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF11_0;
                break;
            case 12:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE12 | GPIO_CFGHR_CNF12);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF12_0;
                break;
            case 13:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE13 | GPIO_CFGHR_CNF13);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF13_0;
                break;
            case 14:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE14 | GPIO_CFGHR_CNF14);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF14_0;
                break;
            case 15:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE15 | GPIO_CFGHR_CNF15);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF15_0;
                break;
            }
            break;
        case PULL_DOWN:
            switch (num) {
            case 0:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF0_1;
                *(io*)(addr + OUTDR) &= ~GPIO_OUTDR_ODR0;
                break;
            case 1:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF1_1;
                *(io*)(addr + OUTDR) &= ~GPIO_OUTDR_ODR1;
                break;
            case 2:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF2_1;
                *(io*)(addr + OUTDR) &= ~GPIO_OUTDR_ODR2;
                break;
            case 3:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF3_1;
                *(io*)(addr + OUTDR) &= ~GPIO_OUTDR_ODR3;
                break;
            case 4:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF4_1;
                *(io*)(addr + OUTDR) &= ~GPIO_OUTDR_ODR4;
                break;
            case 5:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE5 | GPIO_CFGLR_CNF5);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF5_1;
                *(io*)(addr + OUTDR) &= ~GPIO_OUTDR_ODR5;
                break;
            case 6:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE6 | GPIO_CFGLR_CNF6);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF6_1;
                *(io*)(addr + OUTDR) &= ~GPIO_OUTDR_ODR6;
                break;
            case 7:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF7_1;
                *(io*)(addr + OUTDR) &= ~GPIO_OUTDR_ODR7;
                break;
            case 8:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF8_1;
                *(io*)(addr + OUTDR) &= ~GPIO_OUTDR_ODR8;
                break;
            case 9:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE9 | GPIO_CFGHR_CNF9);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF9_1;
                *(io*)(addr + OUTDR) &= ~GPIO_OUTDR_ODR9;
                break;
            case 10:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE10 | GPIO_CFGHR_CNF10);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF10_1;
                *(io*)(addr + OUTDR) &= ~GPIO_OUTDR_ODR10;
                break;
            case 11:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE11 | GPIO_CFGHR_CNF11);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF11_1;
                *(io*)(addr + OUTDR) &= ~GPIO_OUTDR_ODR11;
                break;
            case 12:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE12 | GPIO_CFGHR_CNF12);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF12_1;
                *(io*)(addr + OUTDR) &= ~GPIO_OUTDR_ODR12;
                break;
            case 13:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE13 | GPIO_CFGHR_CNF13);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF13_1;
                *(io*)(addr + OUTDR) &= ~GPIO_OUTDR_ODR13;
                break;
            case 14:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE14 | GPIO_CFGHR_CNF14);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF14_1;
                *(io*)(addr + OUTDR) &= ~GPIO_OUTDR_ODR14;
                break;
            case 15:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE15 | GPIO_CFGHR_CNF15);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF15_1;
                *(io*)(addr + OUTDR) &= ~GPIO_OUTDR_ODR15;
                break;
            }
            break;
        case PULL_UP:
            switch (num) {
            case 0:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF0_1;
                *(io*)(addr + OUTDR) |= GPIO_OUTDR_ODR0;
                break;
            case 1:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF1_1;
                *(io*)(addr + OUTDR) |= GPIO_OUTDR_ODR1;
                break;
            case 2:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF2_1;
                *(io*)(addr + OUTDR) |= GPIO_OUTDR_ODR2;
                break;
            case 3:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF3_1;
                *(io*)(addr + OUTDR) |= GPIO_OUTDR_ODR3;
                break;
            case 4:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF4_1;
                *(io*)(addr + OUTDR) |= GPIO_OUTDR_ODR4;
                break;
            case 5:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE5 | GPIO_CFGLR_CNF5);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF5_1;
                *(io*)(addr + OUTDR) |= GPIO_OUTDR_ODR5;
                break;
            case 6:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE6 | GPIO_CFGLR_CNF6);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF6_1;
                *(io*)(addr + OUTDR) |= GPIO_OUTDR_ODR6;
                break;
            case 7:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_CNF7_1;
                *(io*)(addr + OUTDR) |= GPIO_OUTDR_ODR7;
                break;
            case 8:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF8_1;
                *(io*)(addr + OUTDR) |= GPIO_OUTDR_ODR8;
                break;
            case 9:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE9 | GPIO_CFGHR_CNF9);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF9_1;
                *(io*)(addr + OUTDR) |= GPIO_OUTDR_ODR9;
                break;
            case 10:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE10 | GPIO_CFGHR_CNF10);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF10_1;
                *(io*)(addr + OUTDR) |= GPIO_OUTDR_ODR10;
                break;
            case 11:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE11 | GPIO_CFGHR_CNF11);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF11_1;
                *(io*)(addr + OUTDR) |= GPIO_OUTDR_ODR11;
                break;
            case 12:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE12 | GPIO_CFGHR_CNF12);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF12_1;
                *(io*)(addr + OUTDR) |= GPIO_OUTDR_ODR12;
                break;
            case 13:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE13 | GPIO_CFGHR_CNF13);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF13_1;
                *(io*)(addr + OUTDR) |= GPIO_OUTDR_ODR13;
                break;
            case 14:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE14 | GPIO_CFGHR_CNF14);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF14_1;
                *(io*)(addr + OUTDR) |= GPIO_OUTDR_ODR14;
                break;
            case 15:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_MODE15 | GPIO_CFGHR_CNF15);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_CNF15_1;
                *(io*)(addr + OUTDR) |= GPIO_OUTDR_ODR15;
                break;
            }
            break;
        default:
            break;
        }
    }
    static bool getState() {
        GPIO_TypeDef* name = whichGPIO();
        switch (num) {
        case 0:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR0;
            break;
        case 1:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR1;
            break;
        case 2:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR2;
            break;
        case 3:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR3;
            break;
        case 4:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR4;
            break;
        case 5:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR5;
            break;
        case 6:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR6;
            break;
        case 7:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR7;
            break;
        case 8:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR8;
            break;
        case 9:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR9;
            break;
        case 10:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR10;
            break;
        case 11:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR11;
            break;
        case 12:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR12;
            break;
        case 13:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR13;
            break;
        case 14:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR14;
            break;
        case 15:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR15;
            break;
        default:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR0;
            break;
        }
    }

  private:
    static constexpr uint32_t whichGPIO() {
        if (PA == portName) {
            return GPIOA_BASE;
        } else if (PB == portName) {
            return GPIOB_BASE;
        } else if (PC == portName) {
            return GPIOC_BASE;
        } else if (PD == portName) {
            return GPIOD_BASE;
        } else
            return GPIOE_BASE;
    }
    static constexpr uint32_t addr = whichGPIO();
    static constexpr void setRCC() {
        if (addr == GPIOA_BASE) {
            RCC->APB2PCENR |= RCC_IOPAEN;
        } else if (addr == GPIOB_BASE) {
            RCC->APB2PCENR |= RCC_IOPBEN;
        } else if (addr == GPIOC_BASE) {
            RCC->APB2PCENR |= RCC_IOPCEN;
        } else if (addr == GPIOD_BASE) {
            RCC->APB2PCENR |= RCC_IOPDEN;
        } else {
            RCC->APB2PCENR |= RCC_IOPEEN;
        }
    }
};

template <PortName portName, uint8_t num, InitModeOut mode = PUSH_PULL>
class Out {
  public:
    __attribute__((always_inline)) Out() {
        setRCC();
        setMode();
    }
    static constexpr void setMode() {
        switch (mode) {
        case PUSH_PULL:
            switch (num) {
            case 0:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_CNF0);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_MODE0;
                break;
            case 1:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_CNF1);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_MODE1;
                break;
            case 2:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_CNF2);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_MODE2;
                break;
            case 3:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_CNF3);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_MODE3;
                break;
            case 4:
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_MODE4;
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_CNF4);
                break;
            case 5:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_CNF5);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_MODE5;
                break;
            case 6:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_CNF6);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_MODE6;
                break;
            case 7:
                *(io*)(addr + CFGLR) &= ~(GPIO_CFGLR_CNF7);
                *(io*)(addr + CFGLR) |= GPIO_CFGLR_MODE7;
                break;
            case 8:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_CNF8);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_MODE8;
                break;
            case 9:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_CNF9);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_MODE9;
                break;
            case 10:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_CNF10);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_MODE10;
                break;
            case 11:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_CNF11);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_MODE11;
                break;
            case 12:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_CNF12);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_MODE12;
                break;
            case 13:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_CNF13);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_MODE13;
                break;
            case 14:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_CNF14);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_MODE14;
                break;
            case 15:
                *(io*)(addr + CFGHR) &= ~(GPIO_CFGHR_CNF15);
                *(io*)(addr + CFGHR) |= GPIO_CFGHR_MODE15;
                break;
            }
            break;
        case OPEN_DRAIN:
            switch (num) {
            case 0:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0);
                *(io*)(addr + CFGLR) &= ~GPIO_CFGLR_CNF0_1;
                break;
            case 1:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1);
                *(io*)(addr + CFGLR) &= ~GPIO_CFGLR_CNF1_1;
                break;
            case 2:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2);
                *(io*)(addr + CFGLR) &= ~GPIO_CFGLR_CNF2_1;
                break;
            case 3:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3);
                *(io*)(addr + CFGLR) &= ~GPIO_CFGLR_CNF3_1;
                break;
            case 4:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4);
                *(io*)(addr + CFGLR) &= ~GPIO_CFGLR_CNF4_1;
                break;
            case 5:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE5 | GPIO_CFGLR_CNF5);
                *(io*)(addr + CFGLR) &= ~GPIO_CFGLR_CNF5_1;
                break;
            case 6:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE6 | GPIO_CFGLR_CNF6);
                *(io*)(addr + CFGLR) &= ~GPIO_CFGLR_CNF6_1;
                break;
            case 7:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7);
                *(io*)(addr + CFGLR) &= ~GPIO_CFGLR_CNF7_1;
                break;
            case 8:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
                *(io*)(addr + CFGHR) &= ~GPIO_CFGHR_CNF8_1;
                break;
            case 9:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE9 | GPIO_CFGHR_CNF9);
                *(io*)(addr + CFGHR) &= ~GPIO_CFGHR_CNF9_1;
                break;
            case 10:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE10 | GPIO_CFGHR_CNF10);
                *(io*)(addr + CFGHR) &= ~GPIO_CFGHR_CNF10_1;
                break;
            case 11:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE11 | GPIO_CFGHR_CNF11);
                *(io*)(addr + CFGHR) &= ~GPIO_CFGHR_CNF11_1;
                break;
            case 12:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE12 | GPIO_CFGHR_CNF12);
                *(io*)(addr + CFGHR) &= ~GPIO_CFGHR_CNF12_1;
                break;
            case 13:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE13 | GPIO_CFGHR_CNF13);
                *(io*)(addr + CFGHR) &= ~GPIO_CFGHR_CNF13_1;
                break;
            case 14:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE14 | GPIO_CFGHR_CNF14);
                *(io*)(addr + CFGHR) &= ~GPIO_CFGHR_CNF14_1;
                break;
            case 15:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE15 | GPIO_CFGHR_CNF15);
                *(io*)(addr + CFGHR) &= ~GPIO_CFGHR_CNF15_1;
                break;
            }
            break;
        case ALTERNATE_PUSH_PULL:
            switch (num) {
            case 0:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0);
                *(io*)(addr + CFGLR) &= ~GPIO_CFGLR_CNF0_0;
                break;
            case 1:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1);
                *(io*)(addr + CFGLR) &= ~GPIO_CFGLR_CNF1_0;
                break;
            case 2:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2);
                *(io*)(addr + CFGLR) &= ~GPIO_CFGLR_CNF2_0;
                break;
            case 3:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3);
                *(io*)(addr + CFGLR) &= ~GPIO_CFGLR_CNF3_0;
                break;
            case 4:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4);
                *(io*)(addr + CFGLR) &= ~GPIO_CFGLR_CNF4_0;
                break;
            case 5:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE5 | GPIO_CFGLR_CNF5);
                *(io*)(addr + CFGLR) &= ~GPIO_CFGLR_CNF5_0;
                break;
            case 6:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE6 | GPIO_CFGLR_CNF6);
                *(io*)(addr + CFGLR) &= ~GPIO_CFGLR_CNF6_0;
                break;
            case 7:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7);
                *(io*)(addr + CFGLR) &= ~GPIO_CFGLR_CNF7_0;
                break;
            case 8:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
                *(io*)(addr + CFGHR) &= ~GPIO_CFGHR_CNF8_0;
                break;
            case 9:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE9 | GPIO_CFGHR_CNF9);
                *(io*)(addr + CFGHR) &= ~GPIO_CFGHR_CNF9_0;
                break;
            case 10:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE10 | GPIO_CFGHR_CNF10);
                *(io*)(addr + CFGHR) &= ~GPIO_CFGHR_CNF10_0;
                break;
            case 11:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE11 | GPIO_CFGHR_CNF11);
                *(io*)(addr + CFGHR) &= ~GPIO_CFGHR_CNF11_0;
                break;
            case 12:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE12 | GPIO_CFGHR_CNF12);
                *(io*)(addr + CFGHR) &= ~GPIO_CFGHR_CNF12_0;
                break;
            case 13:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE13 | GPIO_CFGHR_CNF13);
                *(io*)(addr + CFGHR) &= ~GPIO_CFGHR_CNF13_0;
                break;
            case 14:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE14 | GPIO_CFGHR_CNF14);
                *(io*)(addr + CFGHR) &= ~GPIO_CFGHR_CNF14_0;
                break;
            case 15:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE15 | GPIO_CFGHR_CNF15);
                *(io*)(addr + CFGHR) &= ~GPIO_CFGHR_CNF15_0;
                break;
            }
            break;
        case ALTERNATE_OPEN_DRAIN:
            switch (num) {
            case 0:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0);
                break;
            case 1:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1);
                break;
            case 2:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2);
                break;
            case 3:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3);
                break;
            case 4:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4);
                break;
            case 5:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE5 | GPIO_CFGLR_CNF5);
                break;
            case 6:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE6 | GPIO_CFGLR_CNF6);
                break;
            case 7:
                *(io*)(addr + CFGLR) |= (GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7);
                break;
            case 8:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
                break;
            case 9:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE9 | GPIO_CFGHR_CNF9);
                break;
            case 10:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE10 | GPIO_CFGHR_CNF10);
                break;
            case 11:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE11 | GPIO_CFGHR_CNF11);
                break;
            case 12:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE12 | GPIO_CFGHR_CNF12);
                break;
            case 13:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE13 | GPIO_CFGHR_CNF13);
                break;
            case 14:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE14 | GPIO_CFGHR_CNF14);
                break;
            case 15:
                *(io*)(addr + CFGHR) |= (GPIO_CFGHR_MODE15 | GPIO_CFGHR_CNF15);
                break;
            }
            break;
        default:
            break;
        }
    }
    static constexpr void setHigh() {
        switch (num) {
        case 0:
            *(io*)(addr + BSHR) = GPIO_BSHR_BS0;
            break;
        case 1:
            *(io*)(addr + BSHR) = GPIO_BSHR_BS1;
            break;
        case 2:
            *(io*)(addr + BSHR) = GPIO_BSHR_BS2;
            break;
        case 3:
            *(io*)(addr + BSHR) = GPIO_BSHR_BS3;
            break;
        case 4:
            *(io*)(addr + BSHR) = GPIO_BSHR_BS4;
            break;
        case 5:
            *(io*)(addr + BSHR) = GPIO_BSHR_BS5;
            break;
        case 6:
            *(io*)(addr + BSHR) = GPIO_BSHR_BS6;
            break;
        case 7:
            *(io*)(addr + BSHR) = GPIO_BSHR_BS7;
            break;
        case 8:
            *(io*)(addr + BSHR) = GPIO_BSHR_BS8;
            break;
        case 9:
            *(io*)(addr + BSHR) = GPIO_BSHR_BS9;
            break;
        case 10:
            *(io*)(addr + BSHR) = GPIO_BSHR_BS10;
            break;
        case 11:
            *(io*)(addr + BSHR) = GPIO_BSHR_BS11;
            break;
        case 12:
            *(io*)(addr + BSHR) = GPIO_BSHR_BS12;
            break;
        case 13:
            *(io*)(addr + BSHR) = GPIO_BSHR_BS13;
            break;
        case 14:
            *(io*)(addr + BSHR) = GPIO_BSHR_BS14;
            break;
        case 15:
            *(io*)(addr + BSHR) = GPIO_BSHR_BS15;
            break;
        default:
            break;
        }
    }
    static constexpr void setLow() {
        switch (num) {
        case 0:
            *(io*)(addr + BSHR) = GPIO_BSHR_BR0;
            break;
        case 1:
            *(io*)(addr + BSHR) = GPIO_BSHR_BR1;
            break;
        case 2:
            *(io*)(addr + BSHR) = GPIO_BSHR_BR2;
            break;
        case 3:
            *(io*)(addr + BSHR) = GPIO_BSHR_BR3;
            break;
        case 4:
            *(io*)(addr + BSHR) = GPIO_BSHR_BR4;
            break;
        case 5:
            *(io*)(addr + BSHR) = GPIO_BSHR_BR5;
            break;
        case 6:
            *(io*)(addr + BSHR) = GPIO_BSHR_BR6;
            break;
        case 7:
            *(io*)(addr + BSHR) = GPIO_BSHR_BR7;
            break;
        case 8:
            *(io*)(addr + BSHR) = GPIO_BSHR_BR8;
            break;
        case 9:
            *(io*)(addr + BSHR) = GPIO_BSHR_BS9;
            break;
        case 10:
            *(io*)(addr + BSHR) = GPIO_BSHR_BR10;
            break;
        case 11:
            *(io*)(addr + BSHR) = GPIO_BSHR_BR11;
            break;
        case 12:
            *(io*)(addr + BSHR) = GPIO_BSHR_BR12;
            break;
        case 13:
            *(io*)(addr + BSHR) = GPIO_BSHR_BR13;
            break;
        case 14:
            *(io*)(addr + BSHR) = GPIO_BSHR_BR14;
            break;
        case 15:
            *(io*)(addr + BSHR) = GPIO_BSHR_BR15;
            break;
        default:
            break;
        }
    }
    static constexpr void toggle() {
        GPIO_TypeDef* name = (GPIO_TypeDef*)addr;
        switch (num) {
        case 0:
            *(io*)(addr + OUTDR) ^= GPIO_OUTDR_ODR0;
            break;
        case 1:
            *(io*)(addr + OUTDR) ^= GPIO_OUTDR_ODR1;
            break;
        case 2:
            *(io*)(addr + OUTDR) ^= GPIO_OUTDR_ODR2;
            break;
        case 3:
            *(io*)(addr + OUTDR) ^= GPIO_OUTDR_ODR3;
            break;
        case 4:
            *(io*)(addr + OUTDR) ^= GPIO_OUTDR_ODR4;
            break;
        case 5:
            *(io*)(addr + OUTDR) ^= GPIO_OUTDR_ODR5;
            break;
        case 6:
            *(io*)(addr + OUTDR) ^= GPIO_OUTDR_ODR6;
            break;
        case 7:
            *(io*)(addr + OUTDR) ^= GPIO_OUTDR_ODR7;
            break;
        case 8:
            *(io*)(addr + OUTDR) ^= GPIO_OUTDR_ODR8;
            break;
        case 9:
            *(io*)(addr + OUTDR) ^= GPIO_OUTDR_ODR9;
            break;
        case 10:
            *(io*)(addr + OUTDR) ^= GPIO_OUTDR_ODR10;
            break;
        case 11:
            *(io*)(addr + OUTDR) ^= GPIO_OUTDR_ODR11;
            break;
        case 12:
            *(io*)(addr + OUTDR) ^= GPIO_OUTDR_ODR12;
            break;
        case 13:
            *(io*)(addr + OUTDR) ^= GPIO_OUTDR_ODR13;
            break;
        case 14:
            *(io*)(addr + OUTDR) ^= GPIO_OUTDR_ODR14;
            break;
        case 15:
            *(io*)(addr + OUTDR) ^= GPIO_OUTDR_ODR15;
            break;
        default:
            break;
        }
    }
    static constexpr bool getState() {
        GPIO_TypeDef* name = (GPIO_TypeDef*)addr;
        switch (num) {
        case 0:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR0;
            break;
        case 1:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR1;
            break;
        case 2:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR2;
            break;
        case 3:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR3;
            break;
        case 4:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR4;
            break;
        case 5:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR5;
            break;
        case 6:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR6;
            break;
        case 7:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR7;
            break;
        case 8:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR8;
            break;
        case 9:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR9;
            break;
        case 10:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR10;
            break;
        case 11:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR11;
            break;
        case 12:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR12;
            break;
        case 13:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR13;
            break;
        case 14:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR14;
            break;
        case 15:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR15;
            break;
        default:
            return *(io*)(addr + INDR) & GPIO_INDR_IDR0;
            break;
        }
    }

  private:
    static constexpr uint32_t whichGPIO() {
        if (PA == portName) {
            return GPIOA_BASE;
        } else if (PB == portName) {
            return GPIOB_BASE;
        } else if (PC == portName) {
            return GPIOC_BASE;
        } else if (PD == portName) {
            return GPIOD_BASE;
        } else
            return GPIOE_BASE;
    }
    static constexpr uint32_t addr = whichGPIO();
    static constexpr void setRCC() {
        if (addr == GPIOA_BASE) {
            RCC->APB2PCENR |= RCC_IOPAEN;
        } else if (addr == GPIOB_BASE) {
            RCC->APB2PCENR |= RCC_IOPBEN;
        } else if (addr == GPIOC_BASE) {
            RCC->APB2PCENR |= RCC_IOPCEN;
        } else if (addr == GPIOD_BASE) {
            RCC->APB2PCENR |= RCC_IOPDEN;
        } else {
            RCC->APB2PCENR |= RCC_IOPEEN;
        }
    }
};

//

template <PortName portName, uint8_t num, InitModeOut mode> class OUT {
  public:
    OUT() {
        setRCC();
        setMode();
    }
    static constexpr void setMode() {
        switch (mode) {
        case PUSH_PULL:
            *(io*)(addr + CFGHR * IsLowOrHigh) &=
                ~(0xF << (num - (8 - num % 8) * (IsLowOrHigh)) * 4);
            *(io*)(addr + CFGHR * IsLowOrHigh) |=
                (3 << (num - (8 - num % 8) * IsLowOrHigh) * 4);
            break;
        case OPEN_DRAIN:
            *(io*)(addr + CFGHR * IsLowOrHigh) &=
                ~(0xF << (num - (8 - num % 8) * (IsLowOrHigh)) * 4);
            *(io*)(addr + CFGHR * IsLowOrHigh) |=
                (7 << (num - (8 - num % 8) * IsLowOrHigh) * 4);
            break;
        case ALTERNATE_PUSH_PULL:
            *(io*)(addr + CFGHR * IsLowOrHigh) &=
                ~(0xF << (num - (8 - num % 8) * (IsLowOrHigh)) * 4);
            *(io*)(addr + CFGHR * IsLowOrHigh) |=
                (0xB << (num - (8 - num % 8) * IsLowOrHigh) * 4);
            break;
        case ALTERNATE_OPEN_DRAIN:
            *(io*)(addr + CFGHR * IsLowOrHigh) |=
                (0xF << (num - (8 - num % 8) * IsLowOrHigh) * 4);
            break;
        }
    }
    static constexpr void setHigh() { *(io*)(addr + BSHR) = (0x1 << num); }
    static constexpr void setLow() {
        *(io*)(addr + BSHR) = (0x1 << (num + 16));
    }
    static constexpr void toggle() { *(io*)(addr + OUTDR) ^= (0x1 << num); }
    static constexpr bool getState() {
        return (*(io*)(addr + INDR) & (0x1 << num));
    }

  private:
    static constexpr bool isLowOrHigh() { return (num < 8) ? false : true; }
    static constexpr bool IsLowOrHigh = isLowOrHigh();
    static constexpr uint32_t GPIO_ADDR() {
        // return (Regs::GPIOA_BASE_C + portName*0x400);
        if (PA == portName) {
            return GPIOA_BASE;
        } else if (PB == portName) {
            return GPIOB_BASE;
        } else if (PC == portName) {
            return GPIOC_BASE;
        } else if (PD == portName) {
            return GPIOD_BASE;
        } else
            return GPIOE_BASE;
    }
    static constexpr uint32_t addr = GPIO_ADDR();
    static constexpr void setRCC() {
        if (portName == PA) {
            RCC->APB2PCENR |= RCC_IOPAEN;
        } else if (portName == PB) {
            RCC->APB2PCENR |= RCC_IOPBEN;
        } else if (portName == PC) {
            RCC->APB2PCENR |= RCC_IOPCEN;
        } else if (portName == PD) {
            RCC->APB2PCENR |= RCC_IOPDEN;
        } else {
            RCC->APB2PCENR |= RCC_IOPEEN;
        }
    }
};

} // namespace Gpios

#endif // GPIOS_H
