#include "basic_timer.h"

// void (*BasicTimer6::fPtr)(void) = nullptr;

BasicTimer3::BasicTimer3() { init(); }

void BasicTimer3::setCallback(void (*f)()) { fPtr = f; }
void BasicTimer3::init() {
    // TIM CLK=144MHz
    RCC->APB1PCENR |= RCC_TIM3EN;
    // TIM6->CTLR1 |= TIM_ARPE;    // enable autoreload
    TIM3->PSC = 144 - 1;        // TIM6 clk = 1 MHz
    TIM3->ATRLR = 1000 - 1;     // period = 1ms
    TIM3->DMAINTENR |= TIM_UIE; // update interrupt enabled
    // TIM3->CCER = 0;
    NVIC_EnableIRQ(TIM3_IRQn);
}

extern "C" void TIM3_IRQHandler() {
    uint32_t x = 0;
    x++;
    if (TIM3->INTFR & TIM_UIF) {
        // TIM3->INTFR &= ~TIM_UIF;
        TIM3->INTFR = 0x0000;
        // NVIC_ClearPendingIRQ(TIM6_IRQn);
        if (BasicTimer3::Instance().counter >=
            BasicTimer3::Instance().counterMax) {
            if (BasicTimer3::Instance().fPtr != nullptr) {
                BasicTimer3::Instance().fPtr();
            }
            BasicTimer3::Instance().counter = 0;
        } else {
            BasicTimer3::Instance().counter++;
        }
    }
}
