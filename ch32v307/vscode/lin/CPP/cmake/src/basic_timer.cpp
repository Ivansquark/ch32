#include "basic_timer.h"

// void (*BasicTimer6::fPtr)(void) = nullptr;

BasicTimer6::BasicTimer6() { init(); }

void BasicTimer6::setCallback(void (*f)()) { fPtr = f; }
void BasicTimer6::init() {
    // TIM CLK=144MHz
    RCC->APB1PCENR |= RCC_TIM6EN;
    // TIM6->CTLR1 |= TIM_ARPE;    // enable autoreload
    TIM6->PSC = 144 - 1;        // TIM6 clk = 1 MHz
    TIM6->ATRLR = 1000 - 1;     // period = 1ms
    TIM6->DMAINTENR |= TIM_UIE; // update interrupt enabled
    // TIM6->CCER = 0;
    NVIC_EnableIRQ(TIM6_IRQn);
}

extern "C" 
//__attribute__((interrupt("WCH-Interrupt-fast")))
__attribute__((interrupt))
void TIM6_IRQHandler() {
    if (TIM6->INTFR & TIM_UIF) {
        // TIM6->INTFR &= ~TIM_UIF;
        TIM6->INTFR = 0x0000;
        InterruptManager::call(TIM6_IRQn);
    }
}

BasicTimer5::BasicTimer5() { init(); }

void BasicTimer5::setCallback(void (*f)()) { fPtr = f; }
void BasicTimer5::init() {
    // TIM CLK=144MHz
    RCC->APB1PCENR |= RCC_TIM5EN;
    // TIM5->CTLR1 |= TIM_ARPE;    // enable autoreload
    TIM5->PSC = 144 - 1;        // TIM5 clk = 1 MHz
    TIM5->ATRLR = 100 - 1;      // period = 100 us
    TIM5->DMAINTENR |= TIM_UIE; // update interrupt enabled
    // TIM5->CCER = 0;
    NVIC_EnableIRQ(TIM5_IRQn);
}

extern "C" __attribute__((interrupt)) void TIM5_IRQHandler() {
    if (TIM5->INTFR & TIM_UIF) {
        // TIM5->INTFR &= ~TIM_UIF;
        TIM5->INTFR = 0x0000;
        InterruptManager::call(TIM5_IRQn);
    }
}
