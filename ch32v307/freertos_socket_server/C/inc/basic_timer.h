#ifndef BASIC_TIMER_H
#define BASIC_TIMER_H

#include "main.h"

void BT6_start(uint32_t ms);
void BT6_stop();

extern void (*BT6_fPtr)(void);
void BT6_setCallback(void (*BT6_fPtr)());

void BT6_init();
//__attribute__((interrupt("WCH-Interrupt-fast")))
__attribute__((interrupt)) void TIM6_IRQHandler();
#endif // BASIC_TIMER_H
