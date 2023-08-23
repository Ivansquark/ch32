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

extern "C" void TIM6_IRQHandler() {
    uint32_t x = 0;
    x++;
    if (TIM6->INTFR & TIM_UIF) {
        // TIM6->INTFR &= ~TIM_UIF;
        TIM6->INTFR = 0x0000;
        // NVIC_ClearPendingIRQ(TIM6_IRQn);

        if (BasicTimer6::Instance().counter >=
            BasicTimer6::Instance().counterMax) {
            InterruptManager::call(TIM6_IRQn);
            // if (BasicTimer6::Instance().fPtr != nullptr) {
            // BasicTimer6::Instance().fPtr();
            //}
            BasicTimer6::Instance().counter = 0;
        } else {
            BasicTimer6::Instance().counter++;
        }
    }
}
