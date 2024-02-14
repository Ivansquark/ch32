#include "main.h"

#include "adc.h"
#include "basic_timer.h"
#include "ds1822.h"
#include "gpio.h"
#include "rcc.h"
#include "rs485.h"
#include "systim.h"
uint32_t SystemCoreClock = 0;
Rcc rcc(8);

void timeout();
bool IsTimeout;

constexpr uint16_t SIZE = 256 * 12;
uint8_t tempArr[SIZE];

void delay(volatile uint32_t val);

struct __attribute__((__packed__)) Tempertures {
    uint8_t num0 = 0;
    float temp0 = 0;
    uint8_t num1 = 1;
    float temp1 = 0;
    uint8_t num2 = 2;
    float temp2 = 0;
    uint8_t num3 = 3;
    float temp3 = 0;
    uint8_t num4 = 4;
    float temp4 = 0;
    uint8_t num5 = 5;
    float temp5 = 0;
    uint8_t num6 = 6;
    float temp6 = 0;
};

int main(void) {
    __enable_irq();
    Gpio::Out::init();
    /* USB20 device init */
    DS1822 temperature0(0);
    DS1822 temperature1(1);
    DS1822 temperature2(2);
    DS1822 temperature3(3);
    DS1822 temperature4(4);
    DS1822 temperature5(5);

    Tempertures temps;
    RS485 rs485;

    BasicTimer3::Instance().setCallback(timeout);

    int counter = 0;
    while (1) {
        temperature0.measureT();
        temperature1.measureT();
        temperature2.measureT();
        temperature3.measureT();
        temperature4.measureT();
        temperature5.measureT();
        temps.temp0 = temperature0.temp;
        temps.temp1 = temperature1.temp;
        temps.temp2 = temperature2.temp;
        temps.temp3 = temperature3.temp;
        temps.temp4 = temperature4.temp;
        temps.temp5 = temperature5.temp;
        // TODO: uart transmit
        if (counter >= 1000.000) {
            counter = 0;
            rs485.writeBytes((const uint8_t*)&temps.temp0, 4);
        } else {
            counter++;
        }
    }
}

void timeout() { IsTimeout = true; }

void delay(volatile uint32_t val) {
    while (val--) {}
}
