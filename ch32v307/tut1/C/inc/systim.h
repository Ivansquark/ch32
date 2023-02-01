#ifndef SYSTIM_H
#define SYSTIM_H

#include "main.h"

#define SYSTIM_RELOAD_VAL 1440

// 10 us irq
void SysTim_init();
void SysTim_start();
void SysTim_stop();
uint8_t SysTim_getIsTimeout();
void SysTim_setIsTimeout(uint8_t state);

uint32_t SysTim_getCounter();
void SysTim_setCounter(uint32_t val);

void SysTim_delay_10us(uint32_t times);

__attribute__((interrupt))
void SysTick_Handler(void);

#endif // SYSTIM
