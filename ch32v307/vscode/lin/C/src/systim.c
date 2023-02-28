#include "systim.h"

static volatile uint8_t IsTimeout = 0;
static volatile uint32_t counter = 0;

void SysTim_init() {
    SysTick->CTLR |= (1 << 2); // 1: HCLK selected as time base
    SysTick->CTLR |= (1 << 3); // 1: Start counting from zero again when
    SysTick->CTLR |= (1 << 1); // 1: interrupt enable
    SysTick->CMP = SYSTIM_RELOAD_VAL;
    NVIC_EnableIRQ(SysTicK_IRQn);
    SysTim_start();
}

void SysTim_start() {
    SysTick->CTLR |= 1; // tim enable
}
void SysTim_stop() {
    SysTick->CTLR &= ~1; // tim disable
}

uint8_t SysTim_getIsTimeout() { return IsTimeout; }
void SysTim_setIsTimeout(uint8_t state) { IsTimeout = state; }
uint32_t SysTim_getCounter() { return counter; }
void SysTim_setCounter(uint32_t val) { counter = val; }

void SysTim_delay_10us(uint32_t times) {
    counter = 0;
    while (counter < times) {}
}

void SysTick_Handler(void) {
    IsTimeout = 1;
    counter++;
    SysTick->SR = 0;
}
