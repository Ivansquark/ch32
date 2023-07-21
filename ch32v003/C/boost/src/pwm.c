#include "pwm.h"

uint8_t percent = 0;

void pwm_start() { TIM1->CTLR1 |= TIM_CEN; }
void pwm_stop() { TIM1->CTLR1 &= ~TIM_CEN; }

void pwm_ch1_start(uint8_t duty) { TIM1->CH1CVR = duty; }
void pwm_ch1_stop() { TIM1->CH1CVR = TIM1->ATRLR + 1; }
void pwm_ch2_start(uint8_t duty) { TIM1->CH2CVR = duty; }
void pwm_ch2_stop() { TIM1->CH2CVR = TIM1->ATRLR + 1; }

void pwm_change_duty(uint8_t percents)
{
    TIM1->CH2CVR = (percents * AUTO_RELOAD_VALUE_FOR_50kHz) / 100;
}

void pwm_init()
{
    // SYSCLK = 24 MHz
    // pwm ch1 out
    RCC->APB2PCENR |= RCC_IOPCEN;
    RCC->APB2PCENR |= RCC_IOPDEN;

    // PC7 T1CH2 1:0:1:1 - Multiplexed function push-pull output mode
    GPIOC->CFGLR |= GPIO_CFGLR_MODE7;
    GPIOC->CFGLR &= ~GPIO_CFGLR_CNF7;
    GPIOC->CFGLR |= GPIO_CFGLR_CNF7_1;
    // PD2 T1CH2N 1:0:1:1 - Multiplexed function push-pull output mode
    //GPIOD->CFGLR |= GPIO_CFGLR_MODE2;
    //GPIOD->CFGLR &= ~GPIO_CFGLR_CNF2;
    //GPIOD->CFGLR |= GPIO_CFGLR_CNF2_1;

    RCC->APB2PCENR |= RCC_AFIOEN;
    // 0:1 remap TIM1 channel2 uart5 on C7, d2
    AFIO->PCFR1 |= (3 << 6);
    // TIM1 init
    RCC->APB2PCENR |= RCC_TIM1EN;
    // Enables the Automatic Reload Value Register
    // TIM1->CTLR1 |= TIM_ARPE;
    // TIM1->CHCTLR1 &= ~TIM_OC1M;
    // TIM1->CHCTLR1 |= TIM_OC1M_2 | TIM_OC1M_1;
    // OC2M=1:1:0 => PWM mode 1
    TIM1->CHCTLR1 &= ~TIM_OC2M;
    TIM1->CHCTLR1 |= TIM_OC2M_2 | TIM_OC2M_1;
    // TIM1->CCER |= TIM_CC1E | TIM_CC1NE | TIM_CC2E | TIM_CC2NE;
    //TIM1->CCER |= TIM_CC2NE;
    TIM1->CCER |= TIM_CC2E;

    TIM1->PSC = 1 - 1;                             // 1 => 24 MHz
    percent = 70;
    TIM1->ATRLR = AUTO_RELOAD_VALUE_FOR_50kHz - 1; // 50 kHz
    // TIM1->CH1CVR = TIM1->ATRLR / 2;
    TIM1->CH2CVR = (percent * AUTO_RELOAD_VALUE_FOR_50kHz) / 100;
    //TIM1->CH2CVR = TIM1->ATRLR / 2;

    // dead time Tdts = Tclk = 1/24000000 = 41 ns
    // DTG[7:5]=0xx=>DT=DTG[7:0]*Tdtg, Tdtg=TDTS=41ns; => 0 - 6 us (256*41ns)
    // DTG[7:5]=10x => DT=(64+DTG[5:0])*Tdtg, Tdtg=2×TDTS=81 ns; => 5 - 10 us
    // DTG[7:5]=110 => DT=(32+DTG[4:0])*Tdtg, Tdtg=8×TDTS=328 ns; => 41 - 20 us
    TIM1->BDTR = (0x000 << 4) | 10; // 0.4 us dead time
    TIM1->BDTR |= TIM_MOE;
}
