#ifndef GPTIMERS_H_
#define GPTIMERS_H_

#include "irq.h"
#include "main.h"

class GpTimer {
  public:
    GpTimer(uint8_t timNum = 2);
    static bool timFlag_1s;
    static bool timFlag_1ms;
    static bool timFlag_100us;

  private:
    void init(uint8_t tim);
};
extern "C" __attribute__((interrupt)) void TIM2_IRQHandler();
extern "C" __attribute__((interrupt)) void TIM3_IRQHandler();
#endif // GPTIMERS_H_
