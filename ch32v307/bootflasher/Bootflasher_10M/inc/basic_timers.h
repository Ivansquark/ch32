#ifndef BASIC_TIMERS_H
#define BASIC_TIMERS_H

#include "main.h"

namespace BasicTimers {

void init();

void timerWaitXrayOn_start(uint16_t ms);
void timerWaitXrayOn_stop();
void setCallbackOnTimerWaitXRay_timeout(void (*fPtr)());

} // namespace BasicTimers

extern "C"
    //__attribute__((interrupt("WCH-Interrupt-fast")))
    __attribute__((interrupt)) void
    TIM6_IRQHandler();
#endif // BASIC_TIMERS_H
