#ifndef BASIC_TIMER_H
#define BASIC_TIMER_H

#include "main.h"

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
    inline void stop() { TIM6->CTLR1 &= ~TIM_CEN; }

    void (*Callback)(void);
    void setCallback(void (*f)());
    uint32_t counter = 0;
    uint32_t counterMax = 0;
    bool IsStart = false;
    void timer_start(uint32_t ms);
    void timer_stop();

    void (*ST7735callback)(void);
    void setST7735Callback(void (*fPtr)());
    uint32_t ST7735counter = 0;
    uint32_t ST7735counterMax = 0;
    bool IsST7735start = false;
    void ST7755_timer_start(uint32_t ms);
    void ST7755_timer_stop();


  private:
    BasicTimer6();
    void init();
};
#endif // BASIC_TIMER_H
