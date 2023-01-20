#include "main.h"
#include "gpio.h"

void delay_ticks(volatile uint32_t ticks);

int main(void)
{
    gpio_init();
    while (1) {
        C1_set();
        delay_ticks(1000000);
        C1_reset();
        delay_ticks(1000000);
    }
}

void delay_ticks(volatile uint32_t ticks)
{
    while (ticks--) {}
}
