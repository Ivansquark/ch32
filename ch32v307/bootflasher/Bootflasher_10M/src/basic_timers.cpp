#include "basic_timers.h"
namespace BasicTimers {

void (*fPtrWaitXRayOn)();
uint32_t timerWaitXrayOn_counter = 0;
bool timerWaitXrayOn_Start = false;

} // namespace BasicTimers
void BasicTimers::setCallbackOnTimerWaitXRay_timeout(void (*fPtr)()) {
    fPtrWaitXRayOn = fPtr;
}
void BasicTimers::timerWaitXrayOn_start(uint16_t ms) {
    timerWaitXrayOn_counter = ms;
    timerWaitXrayOn_Start = true;
}
void BasicTimers::timerWaitXrayOn_stop() { timerWaitXrayOn_Start = false; }

void BasicTimers::init() {
    // TIM CLK=144MHz
    RCC->APB1PCENR |= RCC_TIM6EN;
    // TIM6->CTLR1 |= TIM_ARPE;    // enable autoreload
    TIM6->PSC = 144 - 1;        // TIM6 clk = 1 MHz
    TIM6->ATRLR = 1000 - 1;     // period = 1ms
    TIM6->DMAINTENR |= TIM_UIE; // update interrupt enabled
    TIM6->CTLR1 |= TIM_CEN;
    NVIC_EnableIRQ(TIM6_IRQn);
}

extern "C" void TIM6_IRQHandler() {
    if (TIM6->INTFR & TIM_UIF) {
        // TIM6->INTFR &= ~TIM_UIF;
        TIM6->INTFR = 0x0000;
        if (BasicTimers::timerWaitXrayOn_Start) {
            if (!BasicTimers::timerWaitXrayOn_counter) {
                BasicTimers::fPtrWaitXRayOn();
            } else {
                BasicTimers::timerWaitXrayOn_counter--;
            }
        }
        // NVIC_ClearPendingIRQ(TIM6_IRQn);
    }
}
