#include "main.h"

#include "adc.h"
#include "basic_timer.h"
#include "gpio.h"
#include "rcc.h"
#include "systim.h"

Rcc rcc(8);

void timeout();
bool IsTimeout;

constexpr uint16_t SIZE = 256 * 12;
uint8_t tempArr[SIZE];

void delay(volatile uint32_t val);

int main(void) {
    __enable_irq();
    Gpio::Out::init();

    while (1) {
    }
}

void timeout() { IsTimeout = true; }

void delay(volatile uint32_t val) {
    while (val--) {}
}

