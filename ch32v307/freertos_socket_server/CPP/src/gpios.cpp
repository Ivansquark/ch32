#include "gpios.h"

namespace Gpios {

void init_in() {
    RCC->APB2PCENR |= RCC_IOPAEN;
    GPIOA->CFGLR &= ~(GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2);
}

} // namespace Gpios
