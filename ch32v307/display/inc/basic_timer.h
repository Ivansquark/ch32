#ifndef BASIC_TIMER_H
#define BASIC_TIMER_H

#include "irq.h"
#include "main.h"

class BasicTimer6 {
  public:
    inline static BasicTimer6& Instance() {
        static BasicTimer6 tim6;
        return tim6;
    }
    inline void start() { TIM6->CTLR1 |= TIM_CEN; }
    inline void stop() { TIM6->CTLR1 &= ~TIM_CEN; }

  private:
    BasicTimer6();
    void init();
};
extern "C"
    __attribute__((interrupt("WCH-Interrupt-fast")))
    //__attribute__((interrupt))
    void
    TIM6_IRQHandler();
#endif // BASIC_TIMER_H
