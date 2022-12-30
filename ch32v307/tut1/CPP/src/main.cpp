#include "main.h"

#include "basic_timer.h"
#include "eeprom.h"
#include "gpio.h"
#include "rcc.h"
#include "systim.h"

Rcc rcc(8);
Eeprom eeprom;

void timeout();
bool IsTimeout;

void delay(volatile uint32_t val);
int main(void) {
    // NVIC_PriorityGroupConfig(NVIC_PriorityGrou:wqp_2);
    // GPIO_Toggle_INIT();
    SysTim::init(144000000); // 1 s
    __enable_irq();
    BasicTimer6::Instance().setCallback(timeout);
    BasicTimer6::Instance().start(500);
    IsTimeout = false;
    bool timeoutSate = false;
    Gpio::Out::init();
    while (1) {
        //if (IsTimeout) {
        //    IsTimeout = false;
        //    if (!timeoutSate) {
        //        timeoutSate = true;
        //        Gpio::Out::setRed();
        //        Gpio::Out::resetBlue();
        //    } else {
        //        timeoutSate = false;
        //        Gpio::Out::resetRed();
        //        Gpio::Out::setBlue();
        //    }
        //}
        if (SysTim::getIsTimeout()) {
            SysTim::setIsTimeout(false);
            if (!timeoutSate) {
                timeoutSate = true;
                Gpio::Out::setRed();
                Gpio::Out::resetBlue();
            } else {
                timeoutSate = false;
                Gpio::Out::resetRed();
                Gpio::Out::setBlue();
            }
        }
    }
}

void timeout() { IsTimeout = true; }

void delay(volatile uint32_t val) {
    while (val--) {}
}
