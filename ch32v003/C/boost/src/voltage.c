#include "voltage.h"

void voltage_handler()
{
    // once in 100 ms
    // TODO get average value of pwm voltage and regulate it to 6V
    uint16_t adc_5V = 0;
    uint32_t adc_5V_sum = 0;
    for (int i = 0; i < DMA_CH1_BUF_SIZE; i += 2) {
        adc_5V_sum += (dma_ch1_buff[i + 1]);
    }
    adc_5V = adc_5V_sum / (DMA_CH1_BUF_SIZE / 2);

    if (adc_5V < (VALUE_FOR_7V - MAX_DEVIATION_FROM_7V)) {
        if (percent < 80) {
            pwm_change_duty(percent++);
        }
    } else if (adc_5V > (VALUE_FOR_7V + MAX_DEVIATION_FROM_7V)) {
        if (percent > 20) {
            pwm_change_duty(percent--);
        }
    }
}
