#ifndef TIMER_H
#define TIMER_H

#include "main.h"
#include "stdint.h"
#include "stddef.h"

void tim1_init();

// 1ms h/w timer for s/w timers
void tim2_init();


// software timer
void timerMsStart(uint32_t ms);
void timerMsStop();
void setTimerMsCalback(void (*fPtr)());

#endif //TIMER_H
