#ifndef SYSTIM_H
#define SYSTIM_H

#include "main.h"

namespace SysTim {
void init(uint64_t reload_val);
void start();
void stop();
bool getIsTimeout();
void setIsTimeout(bool state);
} // namespace SysTim

extern "C" __attribute__((interrupt)) void SysTick_Handler(void);

#endif // SYSTIM
