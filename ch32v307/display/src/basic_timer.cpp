#include "basic_timer.h"

BasicTimer6::BasicTimer6() { init(); }

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

extern "C" void TIM6_IRQHandler() {
    if (TIM6->INTFR & TIM_UIF) {
        // TIM6->INTFR &= ~TIM_UIF;
        TIM6->INTFR = 0x0000;
        InterruptManager::call(TIM6_IRQn);
        //NVIC_ClearPendingIRQ(TIM6_IRQn);
    }
}
