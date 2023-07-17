#ifndef PWM_H
#define PWM_H

#include "main.h"


// TIM1 pwm init
void pwm_init();
void pwm_start();

void pwm_ch1_start(uint8_t duty);
void pwm_ch1_stop();
void pwm_ch2_start(uint8_t duty);
void pwm_ch2_stop();

#endif //PWM_H
