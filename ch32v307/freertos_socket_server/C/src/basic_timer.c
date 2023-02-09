#include "basic_timer.h"

// void (*BasicTimer6::fPtr)(void) = nullptr;

uint32_t BT6_counter = 0;
uint32_t BT6_counterMax = 0;
void (*BT6_fPtr)(void) = NULL;

void BT6_setCallback(void (*f)()) { BT6_fPtr = f; }
void BT6_init() {
    // TIM CLK=144MHz
    RCC->APB1PCENR |= RCC_TIM6EN;
    // TIM6->CTLR1 |= TIM_ARPE;    // enable autoreload
    TIM6->PSC = 144 - 1;        // TIM6 clk = 1 MHz
    TIM6->ATRLR = 1000 - 1;     // period = 1ms
    TIM6->DMAINTENR |= TIM_UIE; // update interrupt enabled
    // TIM6->CCER = 0;
    NVIC_EnableIRQ(TIM6_IRQn);
}
void BT6_start(uint32_t ms)
{
    BT6_counterMax = ms;
    TIM6->CTLR1 |= TIM_CEN;
}
void BT6_stop() { TIM6->CTLR1 &= ~TIM_CEN; }
void TIM6_IRQHandler() {
    if (TIM6->INTFR & TIM_UIF) {
        // TIM6->INTFR &= ~TIM_UIF;
        TIM6->INTFR = 0x0000;
        // NVIC_ClearPendingIRQ(TIM6_IRQn);
        if (BT6_counter >=
            BT6_counterMax) {
            if (BT6_fPtr != NULL) {
                BT6_fPtr();
            }
            BT6_counter = 0;
        } else {
            BT6_counter++;
        }
    }
}
