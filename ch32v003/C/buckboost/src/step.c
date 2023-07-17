#include "step.h"

#define DIR_FORWARD() ((GPIOD->BSHR) |= (GPIO_BSHR_BS3))
#define DIR_BACKWARD() ((GPIOD->BSHR) |= (GPIO_BSHR_BR3))

void step_init()
{
    pwm_init();
    // gpio init PD3 - DIR
    RCC->APB2PCENR |= RCC_IOPDEN;
    // PD0 T1CH1N 0:0:1:1 - push-pull output mode
    GPIOD->CFGLR |= GPIO_CFGLR_MODE3;
    GPIOD->CFGLR &= ~GPIO_CFGLR_CNF3;
    GPIOD->BSHR |= GPIO_BSHR_BR3;
    pwm_start();
}
void step_forward()
{
    DIR_FORWARD();
    pwm_ch1_start(TIM1->ATRLR / 2);
}
void step_backward()
{
    DIR_BACKWARD();
    pwm_ch1_start(TIM1->ATRLR / 2);
}
void step_stop() { pwm_ch1_stop(); }

void step_handler()
{
    uint16_t val = getMedianVal();
    if (val > 900) {
        step_forward();
    } else if (val < 200) {
        step_backward();
    } else {
        step_stop();
    }
}

void quickSort(uint16_t* A, int p, int len)
{
    int r;
    int x = A[p];
    int i = p;
    //-------- partition --------
    int j;
    for (j = p + 1; j < len; j++) {
        if (A[j] <= x) {
            i = i + 1;
            swap(A[i], A[j]);
        }
    }
    swap(A[i], A[p]);
    //----------------------------
    if (p < len) {
        r = i;
        quickSort(A, p, r);
        quickSort(A, r + 1, len);
    }
}
uint16_t getMedianVal()
{
    uint16_t buf[DMA_CH1_BUF_SIZE] = {0};
    memcpy(buf, (const void*)dma_ch1_buff, 64);
    quickSort((uint16_t*)buf, 0, DMA_CH1_BUF_SIZE);
    uint32_t sum_val = 0;
    for (int i = 0; i < 8; i++) {
        sum_val += buf[DMA_CH1_BUF_SIZE / 2 - 4 + i];
    }
    uint16_t ret_val = sum_val / 8;
    return ret_val;
}
