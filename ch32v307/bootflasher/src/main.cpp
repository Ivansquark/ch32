#include "main.h"

#include "rcc.h"
#include "gpio.h"


int main(void) {
    Gpio::Out::init();
    while (1) {
        Gpio::Out::setBlue();
        for(volatile int i = 0; i < 100000; i++){}
        Gpio::Out::resetBlue();
        for(volatile int i = 0; i < 100000; i++){}
    }
}
