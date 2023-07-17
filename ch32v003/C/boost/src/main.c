#include "main.h"
#include "adc.h"
#include "dma.h"
#include "gpio.h"
#include "step.h"
#include "timer.h"
#include "uart.h"

void delay_ticks(volatile uint32_t ticks);

extern volatile uint16_t dma_ch1_buff[DMA_CH1_BUF_SIZE];
uint8_t sendCounter = 0;
uint32_t blinkCounter = 0;

void timeOut_100ms()
{
    uint16_t val = dma_ch1_buff[0];
    uart_sendbyte(val >> 8);
    uart_sendbyte(val);
}

int main(void)
{
    RCC->CFGR0 &= ~RCC_HPRE; // SYS_CLK = 24MHz
    RCC->CFGR0 &= ~RCC_SW;   // HSI = 24MHz
    gpio_init();
    //TODO: complimentary PWM
    //tim1_init();
    // tim2_init();
    // setTimerMsCalback(timeOut_100ms);
    // timerMsStart(1000);
    uart_init();
    dma_ch1_adc1_init();
    dma_ch1_enable();
    adc_init();
    step_init();
    __enable_irq();
    dma_ch1_buff[0] = 0x55;
    uint8_t adc_Bat = 0;
    uint32_t adc_Bat_sum = 0;
    uint8_t adc_5V = 0;
    uint32_t adc_5V_sum = 0;
    while (1) {
        step_handler();
        if (uartByte == '>') {
            uartByte = 0;
            // TODO: count average
            for (int i = 0; i < DMA_CH1_BUF_SIZE; i += 2) {
                adc_Bat_sum += (dma_ch1_buff[i] >> 2);
                adc_5V_sum += (dma_ch1_buff[i + 1] >> 2);
            }
            adc_Bat = adc_Bat_sum / (DMA_CH1_BUF_SIZE / 2);
            adc_5V = adc_5V_sum / (DMA_CH1_BUF_SIZE / 2);
            uart_sendbyte(adc_Bat);
            uart_sendbyte(adc_5V);
            adc_Bat = 0;
            adc_Bat_sum = 0;
            adc_5V = 0;
            adc_5V_sum = 0;
        }
        // if (!Is_C3()) {
        //    C2_reset();
        //} else {
        //    C2_set();
        //    uint32_t remainder = blinkCounter % 10000;
        //    if (!remainder) {
        //        // uart_sendbyte(dma_ch1_buff[0]);
        //    }
        //}
        // if (uartByte == 0x55) {
        //    uart_sendbyte(sendCounter++);
        //}
        // if (blinkCounter == 500000) {
        //    C1_set();
        //} else if (blinkCounter == 1000000) {
        //    C1_reset();
        //    blinkCounter = 0;
        //}
        // blinkCounter++;
    }
}

void delay_ticks(volatile uint32_t ticks)
{
    while (ticks--) {}
}
