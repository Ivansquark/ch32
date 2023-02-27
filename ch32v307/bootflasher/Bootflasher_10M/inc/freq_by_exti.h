#ifndef FREQ_BY_EXTI
#define FREQ_BY_EXTI

#include "main.h"
#include "gptimers.h"

class FrequencyMeter {
    public:
        FrequencyMeter();
        static FrequencyMeter* pThis;
        GpTimer tim_sec{4};
        
        uint32_t freqU = 0;
        uint32_t freqI = 0;
        uint32_t counterFreqU = 0;
        uint32_t counterFreqI = 0;
    private:
        void init();
};


extern "C" __attribute__((interrupt)) void TIM4_IRQHandler(void);
extern "C" __attribute__((interrupt)) void EXTI1_IRQHandler(void);
extern "C" __attribute__((interrupt)) void EXTI15_10_IRQHandler(void);

#endif //FREQ_BY_EXTI
