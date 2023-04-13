#include "basic_timer.h"

// void (*BasicTimer6::fPtr)(void) = nullptr;

BasicTimer6::BasicTimer6() { init(); }
void (BasicTimer6::*Callback)(void) = nullptr;
void (BasicTimer6::*ST7735callback)(void) = nullptr;
void BasicTimer6::setCallback(void (*f)()) { Callback = f; }
void BasicTimer6::setST7735Callback(void (*f)()) { ST7735callback = f; }

void BasicTimer6::init() {
    // TIM CLK=144MHz
    RCC->APB1PCENR |= RCC_TIM3EN;
    // TIM6->CTLR1 |= TIM_ARPE;    // enable autoreload
    TIM3->PSC = 24 - 1;        // TIM6 clk = 1 MHz
    TIM3->ATRLR = 1000 - 1;     // period = 1ms
    TIM3->DMAINTENR |= TIM_UIE; // update interrupt enabled
    // TIM3->CCER = 0;
    NVIC_EnableIRQ(TIM3_IRQn);
    TIM3->CTLR1 |= TIM_CEN;
}

void BasicTimer6::timer_start(uint32_t ms) {
    IsStart = true;
    counter = 0;
    counterMax = ms;
}
void BasicTimer6::timer_stop() {
    IsStart = false;
    counter = 0;
    counterMax = 0;
}
void BasicTimer6::ST7755_timer_start(uint32_t ms) {
    IsST7735start = true;
    ST7735counter = 0;
    ST7735counterMax = ms;
}
void BasicTimer6::ST7755_timer_stop() {
    IsST7735start = false;
    ST7735counter = 0;
    ST7735counterMax = 0;
}


extern "C"
__attribute__((interrupt))
void TIM3_IRQHandler() {
    uint32_t x = 0;
    x++;
    if (TIM3->INTFR & TIM_UIF) {
        // TIM3->INTFR &= ~TIM_UIF;
        TIM3->INTFR = 0x0000;
        // NVIC_ClearPendingIRQ(TIM6_IRQn);
        if (BasicTimer6::Instance().IsStart) {
            if (BasicTimer6::Instance().counter >=
                BasicTimer6::Instance().counterMax) {
                if (BasicTimer6::Instance().Callback != nullptr) {
                    BasicTimer6::Instance().Callback();
                }
                BasicTimer6::Instance().counter = 0;
            } else {
                BasicTimer6::Instance().counter++;
            }
        }
        if (BasicTimer6::Instance().IsST7735start) {
            if (BasicTimer6::Instance().ST7735counter >=
                BasicTimer6::Instance().ST7735counterMax) {
                if (BasicTimer6::Instance().ST7735callback != nullptr) {
                    BasicTimer6::Instance().ST7735callback();
                }
                BasicTimer6::Instance().ST7735counter = 0;
            } else {
                BasicTimer6::Instance().ST7735counter++;
            }
        }
    }
}
