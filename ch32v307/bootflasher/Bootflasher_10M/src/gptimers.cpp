#include "gptimers.h"

bool GpTimer::timFlag_1s = 0;
bool GpTimer::timFlag_1ms = 0;
bool GpTimer::timFlag_100us = 0;

GpTimer::GpTimer(uint8_t timNum) { init(timNum); }

void GpTimer::init(uint8_t tim) {
    switch (tim) {
    case 2: {
        //---------     ADC1 timer  -------------------
        RCC->APB1PCENR |= RCC_TIM2EN;
        TIM2->PSC = 144 - 1;        // TIM6 clk = 1 MHz
        TIM2->ATRLR = 100 - 1;      // period = 100 us
        TIM2->DMAINTENR |= TIM_UIE; // update interrupt enabled
        TIM2->CTLR1 |= TIM_CEN;
        NVIC_EnableIRQ(TIM2_IRQn);

        //TIM2->ARR = 100 - 1;        // 100 us
        //TIM2->DIER |= TIM_DIER_UIE; // interrupt at overload
        //TIM2->CR2 &= ~TIM_CR2_MMS;
        //TIM2->CR2 |= TIM_CR2_MMS_1;
        //TIM2->CR1 |= TIM_CR1_CEN;
        NVIC_EnableIRQ(TIM2_IRQn); // irq enable
    } break;
    case 3: {
        //----------    ms global timer    --------------------
        RCC->APB1PCENR |= RCC_TIM3EN;
        TIM3->PSC = 144 - 1;        // TIM6 clk = 1 MHz
        TIM3->ATRLR = 1000 - 1;     // period = 1 ms
        TIM3->DMAINTENR |= TIM_UIE; // update interrupt enabled
        TIM3->CTLR1 |= TIM_CEN;
        NVIC_EnableIRQ(TIM3_IRQn); // irq enable
    } break;
    case 4: {
        //----------    sec timer for frequency meter    --------
        RCC->APB1PCENR |= RCC_TIM4EN;
        TIM4->PSC = 36000 - 1;        // TIM6 clk = 1 MHz
        TIM4->ATRLR = 8000 - 1;     // period = 1 s
        TIM4->DMAINTENR |= TIM_UIE; // update interrupt enabled
        TIM4->CTLR1 |= TIM_CEN;
        NVIC_EnableIRQ(TIM4_IRQn); // irq enable
    } break;
    default:
        break;
    }
}

void TIM2_IRQHandler() {
    if (TIM2->INTFR & TIM_UIF) {
        // TIM2->INTFR &= ~TIM_UIF;
        TIM2->INTFR = 0x0000;
        GpTimer::timFlag_100us = true;
        NVIC_ClearPendingIRQ(TIM2_IRQn);
    }
}

void TIM3_IRQHandler(void) {
    if (TIM3->INTFR & TIM_UIF) {
        // TIM2->INTFR &= ~TIM_UIF;
        TIM3->INTFR = 0x0000;
        InterruptManager::call(TIM3_IRQn);
        GpTimer::timFlag_1ms = true;
        GpTimer::timFlag_100us = true;
        NVIC_ClearPendingIRQ(TIM3_IRQn);
    }
}
// TIM4 in freq_by_exti.cpp
// extern "C" void TIM4_IRQHandler(void) {
//	TIM3->SR &=~ TIM_SR_UIF;
//	NVIC_ClearPendingIRQ(TIM4_IRQn);
//}
