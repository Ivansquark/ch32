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

enum PortSpeed
{
    HIGH = 1,
    LOW = 2,
    MAX = 3
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

template <PortName portName, uint8_t num, InitModeIn mode = FLOATING>
class In {
  public:
    In() {
        setRCC();
        setMode();
    }
    static constexpr void setMode() {
        if constexpr (mode == ANALOG) {
            if constexpr (num == 0) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0);
            } else if constexpr (num == 1) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1);
            } else if constexpr (num == 2) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2);
            } else if constexpr (num == 3) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3);
            } else if constexpr (num == 4) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4);
            } else if constexpr (num == 5) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE5 | GPIO_CFGLR_CNF5);
            } else if constexpr (num == 6) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE6 | GPIO_CFGLR_CNF6);
            } else if constexpr (num == 7) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7);
            } else if constexpr (num == 8) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
            } else if constexpr (num == 9) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
            } else if constexpr (num == 10) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
            } else if constexpr (num == 11) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
            } else if constexpr (num == 12) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
            } else if constexpr (num == 13) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
            } else if constexpr (num == 14) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
            } else if constexpr (num == 15) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
            }
        } else if constexpr (mode == FLOATING) {
            if constexpr (num == 0) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF0_0;
            } else if constexpr (num == 1) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF1_0;
            } else if constexpr (num == 2) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF2_0;
            } else if constexpr (num == 3) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF3_0;
            } else if constexpr (num == 4) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF4_0;
            } else if constexpr (num == 5) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE5 | GPIO_CFGLR_CNF5);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF5_0;
            } else if constexpr (num == 6) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE6 | GPIO_CFGLR_CNF6);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF6_0;
            } else if constexpr (num == 7) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF7_0;
            } else if constexpr (num == 8) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF8_0;
            } else if constexpr (num == 9) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE9 | GPIO_CFGHR_CNF9);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF9_0;
            } else if constexpr (num == 10) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE10 | GPIO_CFGHR_CNF10);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF10_0;
            } else if constexpr (num == 11) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE11 | GPIO_CFGHR_CNF11);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF11_0;
            } else if constexpr (num == 12) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE12 | GPIO_CFGHR_CNF12);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF12_0;
            } else if constexpr (num == 13) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE13 | GPIO_CFGHR_CNF13);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF13_0;
            } else if constexpr (num == 14) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE14 | GPIO_CFGHR_CNF14);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF14_0;
            } else if constexpr (num == 15) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE15 | GPIO_CFGHR_CNF15);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF15_0;
            }
        } else if constexpr (mode == PULL_DOWN) {
            if constexpr (num == 0) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF0_1;
                *reinterpret_cast<io*>(addr + OUTDR) &= ~GPIO_OUTDR_ODR0;
            } else if constexpr (num == 1) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF1_1;
                *reinterpret_cast<io*>(addr + OUTDR) &= ~GPIO_OUTDR_ODR1;
            } else if constexpr (num == 2) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF2_1;
                *reinterpret_cast<io*>(addr + OUTDR) &= ~GPIO_OUTDR_ODR2;
            } else if constexpr (num == 3) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF3_1;
                *reinterpret_cast<io*>(addr + OUTDR) &= ~GPIO_OUTDR_ODR3;
            } else if constexpr (num == 4) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF4_1;
                *reinterpret_cast<io*>(addr + OUTDR) &= ~GPIO_OUTDR_ODR4;
            } else if constexpr (num == 5) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE5 | GPIO_CFGLR_CNF5);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF5_1;
                *reinterpret_cast<io*>(addr + OUTDR) &= ~GPIO_OUTDR_ODR5;
            } else if constexpr (num == 6) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE6 | GPIO_CFGLR_CNF6);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF6_1;
                *reinterpret_cast<io*>(addr + OUTDR) &= ~GPIO_OUTDR_ODR6;
            } else if constexpr (num == 7) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF7_1;
                *reinterpret_cast<io*>(addr + OUTDR) &= ~GPIO_OUTDR_ODR7;
            } else if constexpr (num == 8) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF8_1;
                *reinterpret_cast<io*>(addr + OUTDR) &= ~GPIO_OUTDR_ODR8;
            } else if constexpr (num == 9) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE9 | GPIO_CFGHR_CNF9);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF9_1;
                *reinterpret_cast<io*>(addr + OUTDR) &= ~GPIO_OUTDR_ODR9;
            } else if constexpr (num == 10) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE10 | GPIO_CFGHR_CNF10);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF10_1;
                *reinterpret_cast<io*>(addr + OUTDR) &= ~GPIO_OUTDR_ODR10;
            } else if constexpr (num == 11) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE11 | GPIO_CFGHR_CNF11);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF11_1;
                *reinterpret_cast<io*>(addr + OUTDR) &= ~GPIO_OUTDR_ODR11;
            } else if constexpr (num == 12) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE12 | GPIO_CFGHR_CNF12);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF12_1;
                *reinterpret_cast<io*>(addr + OUTDR) &= ~GPIO_OUTDR_ODR12;
            } else if constexpr (num == 13) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE13 | GPIO_CFGHR_CNF13);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF13_1;
                *reinterpret_cast<io*>(addr + OUTDR) &= ~GPIO_OUTDR_ODR13;
            } else if constexpr (num == 14) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE14 | GPIO_CFGHR_CNF14);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF14_1;
                *reinterpret_cast<io*>(addr + OUTDR) &= ~GPIO_OUTDR_ODR14;
            } else if constexpr (num == 15) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE15 | GPIO_CFGHR_CNF15);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF15_1;
                *reinterpret_cast<io*>(addr + OUTDR) &= ~GPIO_OUTDR_ODR15;
            }
        } else if constexpr (mode == PULL_UP) {
            if constexpr (num == 0) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF0_1;
                *reinterpret_cast<io*>(addr + OUTDR) |= GPIO_OUTDR_ODR0;
            } else if constexpr (num == 1) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF1_1;
                *reinterpret_cast<io*>(addr + OUTDR) |= GPIO_OUTDR_ODR1;
            } else if constexpr (num == 2) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF2_1;
                *reinterpret_cast<io*>(addr + OUTDR) |= GPIO_OUTDR_ODR2;
            } else if constexpr (num == 3) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF3_1;
                *reinterpret_cast<io*>(addr + OUTDR) |= GPIO_OUTDR_ODR3;
            } else if constexpr (num == 4) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF4_1;
                *reinterpret_cast<io*>(addr + OUTDR) |= GPIO_OUTDR_ODR4;
            } else if constexpr (num == 5) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE5 | GPIO_CFGLR_CNF5);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF5_1;
                *reinterpret_cast<io*>(addr + OUTDR) |= GPIO_OUTDR_ODR5;
            } else if constexpr (num == 6) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE6 | GPIO_CFGLR_CNF6);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF6_1;
                *reinterpret_cast<io*>(addr + OUTDR) |= GPIO_OUTDR_ODR6;
            } else if constexpr (num == 7) {
                *reinterpret_cast<io*>(addr + CFGLR) &=
                    ~(GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_CNF7_1;
                *reinterpret_cast<io*>(addr + OUTDR) |= GPIO_OUTDR_ODR7;
            } else if constexpr (num == 8) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF8_1;
                *reinterpret_cast<io*>(addr + OUTDR) |= GPIO_OUTDR_ODR8;
            } else if constexpr (num == 9) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE9 | GPIO_CFGHR_CNF9);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF9_1;
                *reinterpret_cast<io*>(addr + OUTDR) |= GPIO_OUTDR_ODR9;
            } else if constexpr (num == 10) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE10 | GPIO_CFGHR_CNF10);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF10_1;
                *reinterpret_cast<io*>(addr + OUTDR) |= GPIO_OUTDR_ODR10;
            } else if constexpr (num == 11) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE11 | GPIO_CFGHR_CNF11);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF11_1;
                *reinterpret_cast<io*>(addr + OUTDR) |= GPIO_OUTDR_ODR11;
            } else if constexpr (num == 12) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE12 | GPIO_CFGHR_CNF12);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF12_1;
                *reinterpret_cast<io*>(addr + OUTDR) |= GPIO_OUTDR_ODR12;
            } else if constexpr (num == 13) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE13 | GPIO_CFGHR_CNF13);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF13_1;
                *reinterpret_cast<io*>(addr + OUTDR) |= GPIO_OUTDR_ODR13;
            } else if constexpr (num == 14) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE14 | GPIO_CFGHR_CNF14);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF14_1;
                *reinterpret_cast<io*>(addr + OUTDR) |= GPIO_OUTDR_ODR14;
            } else if constexpr (num == 15) {
                *reinterpret_cast<io*>(addr + CFGHR) &=
                    ~(GPIO_CFGHR_MODE15 | GPIO_CFGHR_CNF15);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_CNF15_1;
                *reinterpret_cast<io*>(addr + OUTDR) |= GPIO_OUTDR_ODR15;
            }
        }
    }
    static bool getState() {
        if constexpr (num == 0) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR0;
        } else if constexpr (num == 1) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR1;
        } else if constexpr (num == 2) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR2;
        } else if constexpr (num == 3) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR3;
        } else if constexpr (num == 4) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR4;
        } else if constexpr (num == 5) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR5;
        } else if constexpr (num == 6) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR6;
        } else if constexpr (num == 7) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR7;
        } else if constexpr (num == 8) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR8;
        } else if constexpr (num == 9) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR9;
        } else if constexpr (num == 10) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR10;
        } else if constexpr (num == 11) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR11;
        } else if constexpr (num == 12) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR12;
        } else if constexpr (num == 13) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR13;
        } else if constexpr (num == 14) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR14;
        } else if constexpr (num == 15) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR15;
        } else return false;
    }

  private:
    static constexpr uint32_t whichGPIO() {
        if constexpr (PA == portName) {
            return GPIOA_BASE;
        } else if constexpr (PB == portName) {
            return GPIOB_BASE;
        } else if constexpr (PC == portName) {
            return GPIOC_BASE;
        } else if constexpr (PD == portName) {
            return GPIOD_BASE;
        } else if constexpr (PE == portName) {
            return GPIOE_BASE;
        }
    }
    static constexpr uint32_t addr = whichGPIO();
    static constexpr void setRCC() {
        if constexpr (addr == GPIOA_BASE) {
            RCC->APB2PCENR |= RCC_IOPAEN;
        } else if constexpr (addr == GPIOB_BASE) {
            RCC->APB2PCENR |= RCC_IOPBEN;
        } else if constexpr (addr == GPIOC_BASE) {
            RCC->APB2PCENR |= RCC_IOPCEN;
        } else if constexpr (addr == GPIOD_BASE) {
            RCC->APB2PCENR |= RCC_IOPDEN;
        } else if constexpr (addr == GPIOE_BASE) {
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
        if constexpr (mode == PUSH_PULL) {
            if constexpr (num == 0) {
                *reinterpret_cast<io*>(addr + CFGLR) &= ~(GPIO_CFGLR_CNF0);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_MODE0;
            } else if constexpr (num == 1) {
                *reinterpret_cast<io*>(addr + CFGLR) &= ~(GPIO_CFGLR_CNF1);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_MODE1;
            } else if constexpr (num == 2) {
                *reinterpret_cast<io*>(addr + CFGLR) &= ~(GPIO_CFGLR_CNF2);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_MODE2;
            } else if constexpr (num == 3) {
                *reinterpret_cast<io*>(addr + CFGLR) &= ~(GPIO_CFGLR_CNF3);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_MODE3;
            } else if constexpr (num == 4) {
                *reinterpret_cast<io*>(addr + CFGLR) &= ~(GPIO_CFGLR_CNF4);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_MODE4;
            } else if constexpr (num == 5) {
                *reinterpret_cast<io*>(addr + CFGLR) &= ~(GPIO_CFGLR_CNF5);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_MODE5;
            } else if constexpr (num == 6) {
                *reinterpret_cast<io*>(addr + CFGLR) &= ~(GPIO_CFGLR_CNF6);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_MODE6;
            } else if constexpr (num == 7) {
                *reinterpret_cast<io*>(addr + CFGLR) &= ~(GPIO_CFGLR_CNF7);
                *reinterpret_cast<io*>(addr + CFGLR) |= GPIO_CFGLR_MODE7;
            } else if constexpr (num == 8) {
                *reinterpret_cast<io*>(addr + CFGHR) &= ~(GPIO_CFGHR_CNF8);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_MODE8;
            } else if constexpr (num == 9) {
                *reinterpret_cast<io*>(addr + CFGHR) &= ~(GPIO_CFGHR_CNF9);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_MODE9;
            } else if constexpr (num == 10) {
                *reinterpret_cast<io*>(addr + CFGHR) &= ~(GPIO_CFGHR_CNF10);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_MODE10;
            } else if constexpr (num == 11) {
                *reinterpret_cast<io*>(addr + CFGHR) &= ~(GPIO_CFGHR_CNF11);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_MODE11;
            } else if constexpr (num == 12) {
                *reinterpret_cast<io*>(addr + CFGHR) &= ~(GPIO_CFGHR_CNF12);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_MODE12;
            } else if constexpr (num == 13) {
                *reinterpret_cast<io*>(addr + CFGHR) &= ~(GPIO_CFGHR_CNF13);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_MODE13;
            } else if constexpr (num == 14) {
                *reinterpret_cast<io*>(addr + CFGHR) &= ~(GPIO_CFGHR_CNF14);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_MODE14;
            } else if constexpr (num == 15) {
                *reinterpret_cast<io*>(addr + CFGHR) &= ~(GPIO_CFGHR_CNF15);
                *reinterpret_cast<io*>(addr + CFGHR) |= GPIO_CFGHR_MODE15;
            }
        } else if constexpr (mode == OPEN_DRAIN) {
            if constexpr (num == 0) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0);
                *reinterpret_cast<io*>(addr + CFGLR) &= ~GPIO_CFGLR_CNF0_1;
            } else if constexpr (num == 1) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1);
                *reinterpret_cast<io*>(addr + CFGLR) &= ~GPIO_CFGLR_CNF1_1;
            } else if constexpr (num == 2) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2);
                *reinterpret_cast<io*>(addr + CFGLR) &= ~GPIO_CFGLR_CNF2_1;
            } else if constexpr (num == 3) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3);
                *reinterpret_cast<io*>(addr + CFGLR) &= ~GPIO_CFGLR_CNF3_1;
            } else if constexpr (num == 4) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4);
                *reinterpret_cast<io*>(addr + CFGLR) &= ~GPIO_CFGLR_CNF4_1;
            } else if constexpr (num == 5) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE5 | GPIO_CFGLR_CNF5);
                *reinterpret_cast<io*>(addr + CFGLR) &= ~GPIO_CFGLR_CNF5_1;
            } else if constexpr (num == 6) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE6 | GPIO_CFGLR_CNF6);
                *reinterpret_cast<io*>(addr + CFGLR) &= ~GPIO_CFGLR_CNF6_1;
            } else if constexpr (num == 7) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7);
                *reinterpret_cast<io*>(addr + CFGLR) &= ~GPIO_CFGLR_CNF7_1;
            } else if constexpr (num == 8) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
                *reinterpret_cast<io*>(addr + CFGHR) &= ~GPIO_CFGHR_CNF8_1;
            } else if constexpr (num == 9) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE9 | GPIO_CFGHR_CNF9);
                *reinterpret_cast<io*>(addr + CFGHR) &= ~GPIO_CFGHR_CNF9_1;
            } else if constexpr (num == 10) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE10 | GPIO_CFGHR_CNF10);
                *reinterpret_cast<io*>(addr + CFGHR) &= ~GPIO_CFGHR_CNF10_1;
            } else if constexpr (num == 11) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE11 | GPIO_CFGHR_CNF11);
                *reinterpret_cast<io*>(addr + CFGHR) &= ~GPIO_CFGHR_CNF11_1;
            } else if constexpr (num == 12) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE12 | GPIO_CFGHR_CNF12);
                *reinterpret_cast<io*>(addr + CFGHR) &= ~GPIO_CFGHR_CNF12_1;
            } else if constexpr (num == 13) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE13 | GPIO_CFGHR_CNF13);
                *reinterpret_cast<io*>(addr + CFGHR) &= ~GPIO_CFGHR_CNF13_1;
            } else if constexpr (num == 14) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE14 | GPIO_CFGHR_CNF14);
                *reinterpret_cast<io*>(addr + CFGHR) &= ~GPIO_CFGHR_CNF14_1;
            } else if constexpr (num == 15) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE15 | GPIO_CFGHR_CNF15);
                *reinterpret_cast<io*>(addr + CFGHR) &= ~GPIO_CFGHR_CNF15_1;
            }
        } else if constexpr (mode == ALTERNATE_PUSH_PULL) {
            if constexpr (num == 0) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0);
                *reinterpret_cast<io*>(addr + CFGLR) &= ~GPIO_CFGLR_CNF0_0;
            } else if constexpr (num == 1) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1);
                *reinterpret_cast<io*>(addr + CFGLR) &= ~GPIO_CFGLR_CNF1_0;
            } else if constexpr (num == 2) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2);
                *reinterpret_cast<io*>(addr + CFGLR) &= ~GPIO_CFGLR_CNF2_0;
            } else if constexpr (num == 3) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3);
                *reinterpret_cast<io*>(addr + CFGLR) &= ~GPIO_CFGLR_CNF3_0;
            } else if constexpr (num == 4) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4);
                *reinterpret_cast<io*>(addr + CFGLR) &= ~GPIO_CFGLR_CNF4_0;
            } else if constexpr (num == 5) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE5 | GPIO_CFGLR_CNF5);
                *reinterpret_cast<io*>(addr + CFGLR) &= ~GPIO_CFGLR_CNF5_0;
            } else if constexpr (num == 6) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE6 | GPIO_CFGLR_CNF6);
                *reinterpret_cast<io*>(addr + CFGLR) &= ~GPIO_CFGLR_CNF6_0;
            } else if constexpr (num == 7) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7);
                *reinterpret_cast<io*>(addr + CFGLR) &= ~GPIO_CFGLR_CNF7_0;
            } else if constexpr (num == 8) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
                *reinterpret_cast<io*>(addr + CFGHR) &= ~GPIO_CFGHR_CNF8_0;
            } else if constexpr (num == 9) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE9 | GPIO_CFGHR_CNF9);
                *reinterpret_cast<io*>(addr + CFGHR) &= ~GPIO_CFGHR_CNF9_0;
            } else if constexpr (num == 10) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE10 | GPIO_CFGHR_CNF10);
                *reinterpret_cast<io*>(addr + CFGHR) &= ~GPIO_CFGHR_CNF10_0;
            } else if constexpr (num == 11) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE11 | GPIO_CFGHR_CNF11);
                *reinterpret_cast<io*>(addr + CFGHR) &= ~GPIO_CFGHR_CNF11_0;
            } else if constexpr (num == 12) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE12 | GPIO_CFGHR_CNF12);
                *reinterpret_cast<io*>(addr + CFGHR) &= ~GPIO_CFGHR_CNF12_0;
            } else if constexpr (num == 13) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE13 | GPIO_CFGHR_CNF13);
                *reinterpret_cast<io*>(addr + CFGHR) &= ~GPIO_CFGHR_CNF13_0;
            } else if constexpr (num == 14) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE14 | GPIO_CFGHR_CNF14);
                *reinterpret_cast<io*>(addr + CFGHR) &= ~GPIO_CFGHR_CNF14_0;
            } else if constexpr (num == 15) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE15 | GPIO_CFGHR_CNF15);
                *reinterpret_cast<io*>(addr + CFGHR) &= ~GPIO_CFGHR_CNF15_0;
            }
        } else if constexpr (mode == ALTERNATE_OPEN_DRAIN) {
            if constexpr (num == 0) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0);
            } else if constexpr (num == 1) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1);
            } else if constexpr (num == 2) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2);
            } else if constexpr (num == 3) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3);
            } else if constexpr (num == 4) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4);
            } else if constexpr (num == 5) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE5 | GPIO_CFGLR_CNF5);
            } else if constexpr (num == 6) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE6 | GPIO_CFGLR_CNF6);
            } else if constexpr (num == 7) {
                *reinterpret_cast<io*>(addr + CFGLR) |=
                    (GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7);
            } else if constexpr (num == 8) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE8 | GPIO_CFGHR_CNF8);
            } else if constexpr (num == 9) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE9 | GPIO_CFGHR_CNF9);
            } else if constexpr (num == 10) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE10 | GPIO_CFGHR_CNF10);
            } else if constexpr (num == 11) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE11 | GPIO_CFGHR_CNF11);
            } else if constexpr (num == 12) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE12 | GPIO_CFGHR_CNF12);
            } else if constexpr (num == 13) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE13 | GPIO_CFGHR_CNF13);
            } else if constexpr (num == 14) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE14 | GPIO_CFGHR_CNF14);
            } else if constexpr (num == 15) {
                *reinterpret_cast<io*>(addr + CFGHR) |=
                    (GPIO_CFGHR_MODE15 | GPIO_CFGHR_CNF15);
            }
        }
    }
    static constexpr void setHigh() {
        if constexpr (num == 0) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BS0;
        } else if constexpr (num == 1) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BS1;
        } else if constexpr (num == 2) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BS2;
        } else if constexpr (num == 3) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BS3;
        } else if constexpr (num == 4) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BS4;
        } else if constexpr (num == 5) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BS5;
        } else if constexpr (num == 6) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BS6;
        } else if constexpr (num == 7) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BS7;
        } else if constexpr (num == 8) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BS8;
        } else if constexpr (num == 9) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BS9;
        } else if constexpr (num == 10) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BS10;
        } else if constexpr (num == 11) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BS11;
        } else if constexpr (num == 12) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BS12;
        } else if constexpr (num == 13) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BS13;
        } else if constexpr (num == 14) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BS14;
        } else if constexpr (num == 15) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BS15;
        }
    }
    static constexpr void setLow() {
        if constexpr (num == 0) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BR0;
        } else if constexpr (num == 1) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BR1;
        } else if constexpr (num == 2) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BR2;
        } else if constexpr (num == 3) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BR3;
        } else if constexpr (num == 4) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BR4;
        } else if constexpr (num == 5) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BR5;
        } else if constexpr (num == 6) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BR6;
        } else if constexpr (num == 7) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BR7;
        } else if constexpr (num == 8) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BR8;
        } else if constexpr (num == 9) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BR9;
        } else if constexpr (num == 10) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BR10;
        } else if constexpr (num == 11) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BR11;
        } else if constexpr (num == 12) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BR12;
        } else if constexpr (num == 13) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BR13;
        } else if constexpr (num == 14) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BR14;
        } else if constexpr (num == 15) {
            *reinterpret_cast<io*>(addr + BSHR) = GPIO_BSHR_BR15;
        }
    }
    static constexpr void toggle() {
        if constexpr (num == 0) {
            *reinterpret_cast<io*>(addr + OUTDR) ^= GPIO_OUTDR_ODR0;
        } else if constexpr (num == 1) {
            *reinterpret_cast<io*>(addr + OUTDR) ^= GPIO_OUTDR_ODR1;
        } else if constexpr (num == 2) {
            *reinterpret_cast<io*>(addr + OUTDR) ^= GPIO_OUTDR_ODR2;
        } else if constexpr (num == 3) {
            *reinterpret_cast<io*>(addr + OUTDR) ^= GPIO_OUTDR_ODR3;
        } else if constexpr (num == 4) {
            *reinterpret_cast<io*>(addr + OUTDR) ^= GPIO_OUTDR_ODR4;
        } else if constexpr (num == 5) {
            *reinterpret_cast<io*>(addr + OUTDR) ^= GPIO_OUTDR_ODR5;
        } else if constexpr (num == 6) {
            *reinterpret_cast<io*>(addr + OUTDR) ^= GPIO_OUTDR_ODR6;
        } else if constexpr (num == 7) {
            *reinterpret_cast<io*>(addr + OUTDR) ^= GPIO_OUTDR_ODR7;
        } else if constexpr (num == 8) {
            *reinterpret_cast<io*>(addr + OUTDR) ^= GPIO_OUTDR_ODR8;
        } else if constexpr (num == 9) {
            *reinterpret_cast<io*>(addr + OUTDR) ^= GPIO_OUTDR_ODR9;
        } else if constexpr (num == 10) {
            *reinterpret_cast<io*>(addr + OUTDR) ^= GPIO_OUTDR_ODR10;
        } else if constexpr (num == 11) {
            *reinterpret_cast<io*>(addr + OUTDR) ^= GPIO_OUTDR_ODR11;
        } else if constexpr (num == 12) {
            *reinterpret_cast<io*>(addr + OUTDR) ^= GPIO_OUTDR_ODR12;
        } else if constexpr (num == 13) {
            *reinterpret_cast<io*>(addr + OUTDR) ^= GPIO_OUTDR_ODR13;
        } else if constexpr (num == 14) {
            *reinterpret_cast<io*>(addr + OUTDR) ^= GPIO_OUTDR_ODR14;
        } else if constexpr (num == 15) {
            *reinterpret_cast<io*>(addr + OUTDR) ^= GPIO_OUTDR_ODR15;
        }
    }
    static constexpr bool getState() {
        if constexpr (num == 0) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR0;
        } else if constexpr (num == 1) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR1;
        } else if constexpr (num == 2) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR2;
        } else if constexpr (num == 3) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR3;
        } else if constexpr (num == 4) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR4;
        } else if constexpr (num == 5) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR5;
        } else if constexpr (num == 6) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR6;
        } else if constexpr (num == 7) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR7;
        } else if constexpr (num == 8) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR8;
        } else if constexpr (num == 9) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR9;
        } else if constexpr (num == 10) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR10;
        } else if constexpr (num == 11) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR11;
        } else if constexpr (num == 12) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR12;
        } else if constexpr (num == 13) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR13;
        } else if constexpr (num == 14) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR14;
        } else if constexpr (num == 15) {
            return *reinterpret_cast<io*>(addr + INDR) & GPIO_INDR_IDR15;
        }
    }

    // metod to set port output speed
    // usualy is max speed (so no need speed in template parameter)
    static constexpr void setSpeed(PortSpeed speed) {
        if constexpr (num == 0) {
            *reinterpret_cast<io*>(addr + CFGLR) &= ~((~speed) & 0x3);
        } else if constexpr (num == 1) {
            *reinterpret_cast<io*>(addr + CFGLR) &= ~(((~speed) & 0x3) << 4);
        } else if constexpr (num == 2) {
            *reinterpret_cast<io*>(addr + CFGLR) &= ~(((~speed) & 0x3) << 8);
        } else if constexpr (num == 3) {
            *reinterpret_cast<io*>(addr + CFGLR) &= ~(((~speed) & 0x3) << 12);
        } else if constexpr (num == 4) {
            *reinterpret_cast<io*>(addr + CFGLR) &= ~(((~speed) & 0x3) << 16);
        } else if constexpr (num == 5) {
            *reinterpret_cast<io*>(addr + CFGLR) &= ~(((~speed) & 0x3) << 20);
        } else if constexpr (num == 6) {
            *reinterpret_cast<io*>(addr + CFGLR) &= ~(((~speed) & 0x3) << 24);
        } else if constexpr (num == 7) {
            *reinterpret_cast<io*>(addr + CFGLR) &= ~(((~speed) & 0x3) << 28);
        } else if constexpr (num == 8) {
            *reinterpret_cast<io*>(addr + CFGHR) &= ~((~speed) & 0x3);
        } else if constexpr (num == 9) {
            *reinterpret_cast<io*>(addr + CFGHR) &= ~(((~speed) & 0x3) << 4);
        } else if constexpr (num == 10) {
            *reinterpret_cast<io*>(addr + CFGHR) &= ~(((~speed) & 0x3) << 8);
        } else if constexpr (num == 11) {
            *reinterpret_cast<io*>(addr + CFGHR) &= ~(((~speed) & 0x3) << 12);
        } else if constexpr (num == 12) {
            *reinterpret_cast<io*>(addr + CFGHR) &= ~(((~speed) & 0x3) << 16);
        } else if constexpr (num == 13) {
            *reinterpret_cast<io*>(addr + CFGHR) &= ~(((~speed) & 0x3) << 20);
        } else if constexpr (num == 14) {
            *reinterpret_cast<io*>(addr + CFGHR) &= ~(((~speed) & 0x3) << 24);
        } else if constexpr (num == 15) {
            *reinterpret_cast<io*>(addr + CFGHR) &= ~(((~speed) & 0x3) << 28);
        }
    }

  private:
    static constexpr uint32_t whichGPIO() {
        if (PA == portName) {
            return GPIOA_BASE;
        } else if constexpr (PB == portName) {
            return GPIOB_BASE;
        } else if constexpr (PC == portName) {
            return GPIOC_BASE;
        } else if constexpr (PD == portName) {
            return GPIOD_BASE;
        } else if constexpr (PE == portName) {
            return GPIOE_BASE;
        }
    }
    static constexpr uint32_t addr = whichGPIO();
    static constexpr void setRCC() {
        if constexpr (addr == GPIOA_BASE) {
            RCC->APB2PCENR |= RCC_IOPAEN;
        } else if constexpr (addr == GPIOB_BASE) {
            RCC->APB2PCENR |= RCC_IOPBEN;
        } else if constexpr (addr == GPIOC_BASE) {
            RCC->APB2PCENR |= RCC_IOPCEN;
        } else if constexpr (addr == GPIOD_BASE) {
            RCC->APB2PCENR |= RCC_IOPDEN;
        } else if constexpr (addr == GPIOE_BASE) {
            RCC->APB2PCENR |= RCC_IOPEEN;
        }
    }
};

//

// template <PortName portName, uint8_t num, InitModeOut mode> class OUT {
//  public:
//    OUT() {
//        setRCC();
//        setMode();
//    }
//    static constexpr void setMode() {
//        switch (mode) {
//        case PUSH_PULL:
//            *reinterpret_cast<io*>(addr + CFGHR * IsLowOrHigh) &=
//                ~(0xF << (num - (8 - num % 8) * (IsLowOrHigh)) * 4);
//            *reinterpret_cast<io*>(addr + CFGHR * IsLowOrHigh) |=
//                (3 << (num - (8 - num % 8) * IsLowOrHigh) * 4);
//            break;
//        case OPEN_DRAIN:
//            *reinterpret_cast<io*>(addr + CFGHR * IsLowOrHigh) &=
//                ~(0xF << (num - (8 - num % 8) * (IsLowOrHigh)) * 4);
//            *reinterpret_cast<io*>(addr + CFGHR * IsLowOrHigh) |=
//                (7 << (num - (8 - num % 8) * IsLowOrHigh) * 4);
//            break;
//        case ALTERNATE_PUSH_PULL:
//            *reinterpret_cast<io*>(addr + CFGHR * IsLowOrHigh) &=
//                ~(0xF << (num - (8 - num % 8) * (IsLowOrHigh)) * 4);
//            *reinterpret_cast<io*>(addr + CFGHR * IsLowOrHigh) |=
//                (0xB << (num - (8 - num % 8) * IsLowOrHigh) * 4);
//            break;
//        case ALTERNATE_OPEN_DRAIN:
//            *reinterpret_cast<io*>(addr + CFGHR * IsLowOrHigh) |=
//                (0xF << (num - (8 - num % 8) * IsLowOrHigh) * 4);
//            break;
//        }
//    }
//    static constexpr void setHigh() { *reinterpret_cast<io*>(addr + BSHR) =
//    (0x1 << num); } static constexpr void setLow() {
//        *reinterpret_cast<io*>(addr + BSHR) = (0x1 << (num + 16));
//    }
//    static constexpr void toggle() { *reinterpret_cast<io*>(addr + OUTDR) ^=
//    (0x1 << num); } static constexpr bool getState() {
//        return (*reinterpret_cast<io*>(addr + INDR) & (0x1 << num));
//    }
//
//  private:
//    static constexpr bool isLowOrHigh() { return (num < 8) ? false : true; }
//    static constexpr bool IsLowOrHigh = isLowOrHigh();
//    static constexpr uint32_t GPIO_ADDR() {
//        // return (Regs::GPIOA_BASE_C + portName*0x400);
//        if (PA == portName) {
//            return GPIOA_BASE;
//        } else if (PB == portName) {
//            return GPIOB_BASE;
//        } else if (PC == portName) {
//            return GPIOC_BASE;
//        } else if (PD == portName) {
//            return GPIOD_BASE;
//        } else
//            return GPIOE_BASE;
//    }
//    static constexpr uint32_t addr = GPIO_ADDR();
//    static constexpr void setRCC() {
//        if (portName == PA) {
//            RCC->APB2PCENR |= RCC_IOPAEN;
//        } else if (portName == PB) {
//            RCC->APB2PCENR |= RCC_IOPBEN;
//        } else if (portName == PC) {
//            RCC->APB2PCENR |= RCC_IOPCEN;
//        } else if (portName == PD) {
//            RCC->APB2PCENR |= RCC_IOPDEN;
//        } else {
//            RCC->APB2PCENR |= RCC_IOPEEN;
//        }
//    }
//};

} // namespace Gpios

#endif // GPIOS_H
