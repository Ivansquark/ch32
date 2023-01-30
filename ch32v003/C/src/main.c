#include "main.h"
#include "adc.h"
#include "dma.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"

void delay_ticks(volatile uint32_t ticks);

extern volatile uint16_t dma_ch1_buff[DMA_CH1_BUF_SIZE];
uint8_t sendCounter = 0;
uint32_t blinkCounter = 0;
int main(void)
{
    RCC->CFGR0 &= ~RCC_HPRE; // SYS_CLK = 24MHz
    gpio_init();
    tim1_init();
    uart_init();
    dma_ch1_adc1_init();
    dma_ch1_enable();
    adc_init();
    __enable_irq();
    while (1) {
        if (!Is_C3()) {
            C2_reset();
        } else {
            C2_set();
            uint32_t remainder = blinkCounter % 10000;
            if (!remainder) {
                uart_sendbyte(1);
            }
        }
        if (uartByte == 0x55) {
            uart_sendbyte(sendCounter++);
        }
        if (blinkCounter == 500000) {
            C1_set();
        } else if (blinkCounter == 1000000) {
            C1_reset();
            blinkCounter = 0;
        }
        blinkCounter++;
    }
}

void delay_ticks(volatile uint32_t ticks)
{
    while (ticks--) {}
}
