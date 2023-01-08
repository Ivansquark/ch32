#ifndef SYSTIM_H
#define SYSTIM_H

#include "main.h"

namespace SysTim {
// 10 us irq
void init(uint64_t reload_val = 1440);
void start();
void stop();
bool getIsTimeout();
void setIsTimeout(bool state);

uint32_t getCounter();
void setCounter(uint32_t val);

void delay_10us(uint32_t times);
} // namespace SysTim

extern "C" 
__attribute__((interrupt))
void SysTick_Handler(void);

#endif // SYSTIM
