#ifndef BASIC_TIMER_H
#define BASIC_TIMER_H

#include "main.h"
#include "tirq.h"

class BasicTimer6 {
  public:
    inline static BasicTimer6& Instance() {
        static BasicTimer6 tim6;
        return tim6;
    }
    inline void start(uint32_t ms) {
        counterMax = ms;
        TIM6->CTLR1 |= TIM_CEN;
    }
    static inline void stop() { TIM6->CTLR1 &= ~TIM_CEN; }

    void (*fPtr)(void) = nullptr;
    void setCallback(void (*fPtr)());

    uint32_t counter = 0;
    uint32_t counterMax = 0;

  private:
    BasicTimer6();
    static void init();
};
extern "C"
//__attribute__((interrupt("WCH-Interrupt-fast")))
__attribute__((interrupt))
void TIM6_IRQHandler();

class BasicTimer5 {
  public:
    inline static BasicTimer5& Instance() {
        static BasicTimer5 tim5;
        return tim5;
    }
    inline void start(uint32_t ms) {
        counterMax = ms;
        TIM5->CTLR1 |= TIM_CEN;
    }
    static inline void stop() { TIM5->CTLR1 &= ~TIM_CEN; }

    void (*fPtr)(void) = nullptr;
    void setCallback(void (*fPtr)());

    uint32_t counter = 0;
    uint32_t counterMax = 0;

  private:
    BasicTimer5();
    static void init();
};

#endif // BASIC_TIMER_H
