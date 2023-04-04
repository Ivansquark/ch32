#include "pwm.h"

void pwm_start() { TIM1->CTLR1 |= TIM_CEN; }
void pwm_stop() { TIM1->CTLR1 &= ~TIM_CEN; }

void pwm_ch1_start(uint8_t duty) { TIM1->CH1CVR = duty; }
void pwm_ch1_stop() { TIM1->CH1CVR = TIM1->ATRLR + 1; }
void pwm_ch2_start(uint8_t duty) { TIM1->CH2CVR = duty; }
void pwm_ch2_stop() { TIM1->CH2CVR = TIM1->ATRLR + 1; }

void pwm_init()
{
    // SYSCLK = 24 MHz
    // pwm ch1 out
    RCC->APB2PCENR |= RCC_IOPDEN;
    // PD0 T1CH1N 1:0:1:1 - Multiplexed function push-pull output mode
    GPIOD->CFGLR |= GPIO_CFGLR_MODE0;
    GPIOD->CFGLR &= ~GPIO_CFGLR_CNF0;
    GPIOD->CFGLR |= GPIO_CFGLR_CNF0_1;
    // PD2 T1CH1 1:0:1:1 - Multiplexed function push-pull output mode
    GPIOD->CFGLR |= GPIO_CFGLR_MODE2;
    GPIOD->CFGLR &= ~GPIO_CFGLR_CNF2;
    GPIOD->CFGLR |= GPIO_CFGLR_CNF2_1;

    // pwm ch2 out
    //RCC->APB2PCENR |= RCC_IOPAEN;
    // PA2 T1CH1N 1:0:1:1 - Multiplexed function push-pull output mode
    //GPIOA->CFGLR |= GPIO_CFGLR_MODE2;
    //GPIOA->CFGLR &= ~GPIO_CFGLR_CNF2;
    //GPIOA->CFGLR |= GPIO_CFGLR_CNF2_1;
    // PA1 T1CH1 1:0:1:1 - Multiplexed function push-pull output mode
    //GPIOA->CFGLR |= GPIO_CFGLR_MODE1;
    //GPIOA->CFGLR &= ~GPIO_CFGLR_CNF1;
    //GPIOA->CFGLR |= GPIO_CFGLR_CNF1_1;

    // TIM1 init
    RCC->APB2PCENR |= RCC_TIM1EN;

    TIM1->CTLR1 |= TIM_ARPE;
    TIM1->CHCTLR1 &= ~TIM_OC1M;
    TIM1->CHCTLR1 |= TIM_OC1M_2 | TIM_OC1M_1;
    TIM1->CHCTLR1 &= ~TIM_OC2M;
    TIM1->CHCTLR1 |= TIM_OC2M_2 | TIM_OC2M_1;
    TIM1->CCER |= TIM_CC1E | TIM_CC1NE | TIM_CC2E | TIM_CC2NE;

    TIM1->PSC = 1 - 1;     // 1 => 24 MHz
    TIM1->ATRLR = 12000 - 1; // 500 Hz
    TIM1->CH1CVR = TIM1->ATRLR / 2;
    TIM1->CH2CVR = TIM1->ATRLR / 2;
    
    // dead time Tdts = Tclk = 1/24000000 = 41 ns
    // DTG[7:5]=0xx=>DT=DTG[7:0]*Tdtg, Tdtg=TDTS=41ns; => 0 - 6 us (256*41ns)
    // DTG[7:5]=10x => DT=(64+DTG[5:0])*Tdtg, Tdtg=2×TDTS=81 ns; => 5 - 10 us
    // DTG[7:5]=110 => DT=(32+DTG[4:0])*Tdtg, Tdtg=8×TDTS=328 ns; => 41 - 20 us
    TIM1->BDTR = (0x000 << 4) | 10; // 0.4 us dead time
    TIM1->BDTR |= TIM_MOE;
}
