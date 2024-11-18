#ifndef TRCC_H
#define TRCC_H

#include "ch32v00x.h"

namespace TRcc {

enum Osc {
    HSI,
    HSE
};

template <Osc osc, int freq_out, int HSE_freq>
concept CheckRcc = requires() {
    requires((osc == HSI && freq_out == 24) || (osc == HSI && freq_out == 48) ||
             ((osc == HSE && (freq_out >= 4) && (freq_out <= 48) &&
               (HSE_freq >= 4) && (HSE_freq <= 48) &&
               ((freq_out == HSE_freq) || freq_out == 2 * HSE_freq))));
};

/*@ brief
 * freq_out get from constexpr value that can see all program
 */
template <Osc osc, int freq_out = 0, int HSE_freq = 4>
    requires CheckRcc<osc, freq_out, HSE_freq>
class RccSys {
  public:
    RccSys() {
        RCC->CFGR0 &= ~RCC_HPRE;
        RCC->CFGR0 &= ~RCC_SW;
        if constexpr (osc == HSI) {
            if constexpr (freq_out == 24) {
            } else if constexpr (freq_out == 48) {
                RCC->CFGR0 &= ~RCC_PLLSRC;
                RCC->CFGR0 |= RCC_SW_1;
                RCC->CTLR |= RCC_PLLON;
                while(!(RCC->CTLR & RCC_PLLRDY)) {}
            }
        } else if constexpr (osc == HSE) {
                RCC->CTLR |= RCC_HSEON;
                while(!(RCC->CTLR & RCC_HSERDY)) {}
            if constexpr (freq_out == HSE_freq) {
                RCC->CFGR0 |= RCC_SW_0;
                RCC->CFGR0 &= ~RCC_SW_1;
            } else if constexpr (freq_out == 2 * HSE_freq) {
                RCC->CFGR0 |= RCC_PLLSRC;
                RCC->CFGR0 &= ~RCC_SW_0;
                RCC->CFGR0 |= RCC_SW_1;
                RCC->CTLR |= RCC_PLLON;
                while(!(RCC->CTLR & RCC_PLLRDY)) {}
            }
        }
    }
};
} // namespace TRcc

#endif // TRCC_H
