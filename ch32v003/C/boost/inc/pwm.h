#ifndef PWM_H
#define PWM_H

#include "main.h"

#define AUTO_RELOAD_VALUE_FOR_50kHz 240 //960 //240//480

// TIM1 pwm init
void pwm_init();
void pwm_start();

void pwm_change_duty(uint8_t percents);

void pwm_ch1_start(uint8_t duty);
void pwm_ch1_stop();
void pwm_ch2_start(uint8_t duty);
void pwm_ch2_stop();
extern uint8_t percent;

#endif //PWM_H
