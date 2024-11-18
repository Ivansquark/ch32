#include "timer.h"

static uint8_t IsTimerMs = 0;
static uint32_t timerMsCount = 0;
static uint32_t timerMsCounter = 0;
static void (*timerMsCallbackPtr)() = NULL;
void dummyTimerMsCallback() {}

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
    // TIM1->SWEVGR |= TIM_UG; //update generation
    TIM1->BDTR |= TIM_MOE; // main output enable
    TIM1->CTLR1 |= TIM_CEN;
}
void tim2_init()
{
    RCC->APB1PCENR |= RCC_TIM2EN;
    TIM2->PSC = 1 - 1;          // 24MHz
    TIM2->ATRLR = 24000 - 1;    // 1 ms
    TIM2->DMAINTENR |= TIM_UIE; // update interrupt enable
    TIM2->CTLR1 |= TIM_ARPE;
    //TIM2->SWEVGR |= TIM_UG;     // update generation
    //
    NVIC_EnableIRQ(TIM2_IRQn);
    TIM2->CTLR1 |= TIM_CEN;

    // callbacks timers init
    //timerMsCallbackPtr = dummyTimerMsCallback;
}
void timerMsStart(uint32_t ms)
{
    IsTimerMs = 1;
    timerMsCount = ms;
    timerMsCounter = 0;
}
void timerMsStop()
{
    IsTimerMs = 0;
    timerMsCounter = 0;
}
void setTimerMsCalback(void (*fPtr)()) { timerMsCallbackPtr = fPtr; }


// interrupt
extern "C"
__attribute__((interrupt("WCH-Interrupt-fast")))
void TIM2_IRQHandler()
{
    TIM2->INTFR &= ~TIM_UIF;
    //TIM2->INTFR &= ~TIM_UIF;
    //InterruptManager::call(TIM2_IRQn);
}
