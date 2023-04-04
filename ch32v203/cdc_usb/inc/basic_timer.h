#ifndef BASIC_TIMER_H
#define BASIC_TIMER_H

#include "main.h"

class BasicTimer3 {
  public:
    inline static BasicTimer3& Instance() {
        static BasicTimer3 tim3;
        return tim3;
    }
    inline void start(uint32_t ms) {
        counterMax = ms;
        TIM3->CTLR1 |= TIM_CEN;
    }
    inline void stop() { TIM3->CTLR1 &= ~TIM_CEN; }

    void (*fPtr)(void) = nullptr;
    void setCallback(void (*fPtr)());

    uint32_t counter = 0;
    uint32_t counterMax = 0;

  private:
    BasicTimer3();
    void init();
};
extern "C"
//__attribute__((interrupt("WCH-Interrupt-fast")))
__attribute__((interrupt))
void TIM3_IRQHandler();
#endif // BASIC_TIMER_H
