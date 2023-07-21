#include "main.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"
#include "voltage.h"


extern volatile uint16_t dma_ch1_buff[DMA_CH1_BUF_SIZE];

void delay_ticks(volatile uint32_t ticks);
void timeOut_50ms() {
    //voltage regulator
    voltage_handler();
}

int main(void)
{
    RCC->CFGR0 &= ~RCC_HPRE; // SYS_CLK = 24MHz
    RCC->CFGR0 &= ~RCC_SW;   // HSI = 24MHz
    gpio_init();
    //tim1_init();
    tim2_init();
    setTimerMsCalback(timeOut_50ms);
    timerMsStart(35);

    uart_init();
    dma_ch1_adc1_init();
    dma_ch1_enable();
    adc_init();

    // TODO: complimentary PWM
    pwm_init();
    pwm_start();

    __enable_irq();
    dma_ch1_buff[0] = 0x55;
    uint16_t adc_Bat = 0;
    uint32_t adc_Bat_sum = 0;
    uint16_t adc_5V = 0;
    uint32_t adc_5V_sum = 0;
    while (1) {
        // step_handler();
        if (uartByte == '>') {
            uartByte = 0;
            // TODO: count average
            for (int i = 0; i < DMA_CH1_BUF_SIZE; i += 2) {
                adc_Bat_sum += (dma_ch1_buff[i]);
                adc_5V_sum += (dma_ch1_buff[i + 1]);
            }
            adc_Bat = adc_Bat_sum / (DMA_CH1_BUF_SIZE / 2);
            adc_5V = adc_5V_sum / (DMA_CH1_BUF_SIZE / 2);
            uart_sendbyte(adc_Bat >> 2);
            uart_sendbyte(adc_5V >> 2);
            // uart_sendbyte('1');
            adc_Bat = 0;
            adc_Bat_sum = 0;
            adc_5V = 0;
            adc_5V_sum = 0;
        }
    }
}

void delay_ticks(volatile uint32_t ticks)
{
    while (ticks--) {}
}
