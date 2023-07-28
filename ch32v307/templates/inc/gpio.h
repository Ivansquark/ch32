#ifndef GPIO_H
#define GPIO_H

#include "main.h"

namespace Gpio {

namespace In {
void init();
inline bool isA0() { return (GPIOA->INDR & GPIO_INDR_IDR0); }
inline bool isBlue() { return !(GPIOA->INDR & GPIO_INDR_IDR4); }
inline bool isRed() { return !(GPIOA->INDR & GPIO_INDR_IDR15); }
} // namespace In

namespace Out {
void init();
inline void setRed() { GPIOA->BSHR |= GPIO_BSHR_BS15; }
inline void resetRed() { GPIOA->BSHR |= GPIO_BSHR_BR15; }
inline void toggleRed() { GPIOA->OUTDR ^= GPIO_OUTDR_ODR15; }
inline void setBlue() { GPIOB->BSHR |= GPIO_BSHR_BS4; }
inline void resetBlue() { GPIOB->BSHR |= GPIO_BSHR_BR4; }
inline void toggleBlue() { GPIOB->OUTDR ^= GPIO_OUTDR_ODR4; }

} // namespace Out

} // namespace Gpio

#endif // GPIO_H
