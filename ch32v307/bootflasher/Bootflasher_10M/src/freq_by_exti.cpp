#include "freq_by_exti.h"

FrequencyMeter* FrequencyMeter::pThis = nullptr;

FrequencyMeter::FrequencyMeter() {
    pThis = this;
    init();
}

void FrequencyMeter::init() {
    // freqU-PD1, freqI-PE11
    RCC->APB2PCENR |= RCC_IOPDEN | RCC_IOPEEN;
    GPIOD->CFGLR &= ~GPIO_CFGLR_CNF1;
    GPIOD->CFGLR |= GPIO_CFGLR_CNF1_0;
    GPIOD->CFGLR &= ~GPIO_CFGLR_MODE1;
    AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI1_PD; 
    GPIOE->CFGHR &= ~GPIO_CFGHR_CNF11;
    GPIOE->CFGHR |= GPIO_CFGHR_CNF11_0;
    GPIOE->CFGHR &= ~GPIO_CFGHR_MODE11;
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI11_PE; 
    EXTI->INTENR |= EXTI_INTENR_MR1 | EXTI_INTENR_MR11;
    EXTI->FTENR |= EXTI_FTENR_TR1 | EXTI_FTENR_TR11;
    // SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI9_PA |
    // SYSCFG_EXTICR3_EXTI10_PA); EXTI->IMR |= (EXTI_IMR_MR9 | EXTI_IMR_IM10);
    // EXTI->FTSR |= (EXTI_FTSR_TR9 | EXTI_FTSR_TR10);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    NVIC_EnableIRQ(EXTI1_IRQn);
}

/////////////////////////////////////////////////////
//-------- 1 sec timer ---------------------
void TIM4_IRQHandler(void) {
    TIM4->CTLR1 &= ~TIM_CEN;
    TIM4->INTFR = 0x0000;
    NVIC_ClearPendingIRQ(TIM4_IRQn);
    FrequencyMeter::pThis->freqI = FrequencyMeter::pThis->counterFreqI;
    FrequencyMeter::pThis->freqU = FrequencyMeter::pThis->counterFreqU;
    FrequencyMeter::pThis->counterFreqI = 0;
    FrequencyMeter::pThis->counterFreqU = 0;
    TIM4->CTLR1 |= TIM_CEN;
    GpTimer::timFlag_1s = true;
}

void EXTI1_IRQHandler(void) {
    //! Reset interrupt flag
    EXTI->INTFR = EXTI_INTF_INTF1; // reset interrupt flag
    NVIC_ClearPendingIRQ(EXTI1_IRQn);
    FrequencyMeter::pThis->counterFreqU++;
}
void EXTI15_10_IRQHandler(void) {
    //! Reset interrupt flag
    EXTI->INTFR = EXTI_INTF_INTF11; // reset interrupt flag
    NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    FrequencyMeter::pThis->counterFreqI++;
}
