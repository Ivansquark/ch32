#include "timer.h"

void tim1_init()
{
    // pwm PD2 TIM1 ch1 AF push/pull 1:0:1:1
    RCC->APB2PCENR |= RCC_IOPDEN;
    GPIOD->CFGLR &= ~GPIO_CFGLR_CNF2_0;
    GPIOD->CFGLR |= GPIO_CFGLR_CNF2_1;
    GPIOD->CFGLR |= GPIO_CFGLR_MODE2;

    RCC->APB2PCENR |= RCC_TIM1EN;
    TIM1->PSC = 1 - 1;     // 24MHz 
    TIM1->ATRLR = 429 - 1; // ~ 56 kHz
    TIM1->CH1CVR = TIM1->ATRLR / 2;

    // TIM1-> |= TIM_EGR_UG; //update generation
    TIM1->CCER |= TIM_CC1E; // enable signal on output pin OC2
    // 1:1:0 PWM_MODE 1 active while < CCR1
    TIM1->CHCTLR1 &= ~TIM_OC2M;
    TIM1->CHCTLR1 |= TIM_OC1M_2 | TIM_OC1M_1;
    //TIM1->SWEVGR |= TIM_UG; //update generation
    TIM1->BDTR |= TIM_MOE; //main output enable
    TIM1->CTLR1 |= TIM_CEN;
}
