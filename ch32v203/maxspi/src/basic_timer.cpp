#include "basic_timer.h"

// void (*BasicTimer6::fPtr)(void) = nullptr;

BasicTimer6::BasicTimer6() { init(); }

void BasicTimer6::setCallback(void (*f)()) { fPtr = f; }
void BasicTimer6::init() {
    // TIM CLK=144MHz
    RCC->APB1PCENR |= RCC_TIM3EN;
    // TIM6->CTLR1 |= TIM_ARPE;    // enable autoreload
    TIM3->PSC = 144 - 1;        // TIM6 clk = 1 MHz
    TIM3->ATRLR = 1000 - 1;     // period = 1ms
    TIM3->DMAINTENR |= TIM_UIE; // update interrupt enabled
    // TIM3->CCER = 0;
    NVIC_EnableIRQ(TIM3_IRQn);
}

extern "C" void TIM6_IRQHandler() {
    uint32_t x = 0;
    x++;
    if (TIM3->INTFR & TIM_UIF) {
        // TIM3->INTFR &= ~TIM_UIF;
        TIM3->INTFR = 0x0000;
        // NVIC_ClearPendingIRQ(TIM6_IRQn);
        if (BasicTimer6::Instance().counter >=
            BasicTimer6::Instance().counterMax) {
            if (BasicTimer6::Instance().fPtr != nullptr) {
                BasicTimer6::Instance().fPtr();
            }
            BasicTimer6::Instance().counter = 0;
        } else {
            BasicTimer6::Instance().counter++;
        }
    }
}
