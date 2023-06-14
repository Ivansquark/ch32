#include "systim.h"

namespace SysTim {
volatile bool IsTimeout = false;
volatile uint32_t counter = 0;
} // namespace SysTim

void SysTim::init(uint64_t reload_val) {
    SysTick->CTLR |= (1 << 2); // 1: HCLK selected as time base
    SysTick->CTLR |= (1 << 3); // 1: Start counting from zero again when
    SysTick->CTLR |= (1 << 1); // 1: interrupt enable
    SysTick->CMP = reload_val;
    NVIC_EnableIRQ(SysTicK_IRQn);
    start();
}

void SysTim::start() {
    SysTick->CTLR |= 1; // tim enable
}
void SysTim::stop() {
    SysTick->CTLR &= ~1; // tim disable
}

bool SysTim::getIsTimeout() { return IsTimeout; }
void SysTim::setIsTimeout(bool state) { IsTimeout = state; }
uint32_t SysTim::getCounter() { return counter; }
void SysTim::setCounter(uint32_t val) { counter = val; }

void SysTim::delay_10us(uint32_t times) {
    counter = 0;
    while (counter < times) {}
}

void SysTick_Handler(void) {
    SysTim::IsTimeout = true;
    SysTim::counter++;
    SysTick->SR = 0;
}
