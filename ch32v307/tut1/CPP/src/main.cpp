#include "main.h"

#include "basic_timer.h"
#include "eeprom.h"
#include "gpio.h"
#include "rcc.h"
#include "systim.h"
#include "w25q.h"

Rcc rcc(8);
Eeprom eeprom;

void timeout();
bool IsTimeout;

void delay(volatile uint32_t val);
int main(void) {
    W25q flash;
    //flash.reset();
    delay(1000000);
    //flash.WAKEUP();
    delay(10000);
    uint16_t id = 0;
    //do {
    //    id = 0;
    //    //id = flash.readSR();
    //    id = flash.readID();
    //}
    //while (id != flash.W25Q32);
    const char* str = "Hello wq25 flash";
    uint8_t tempArr[20] = {0};
    flash.write((uint8_t*)str, 0, 15);
    //flash.erase_Sector(0);
    //flash.write(tempArr, 0, sizeof(tempArr));
    flash.read(tempArr, 0x0, sizeof(tempArr));
    SysTim::init(144000000); // 1 s
    __enable_irq();
    BasicTimer6::Instance().setCallback(timeout);
    BasicTimer6::Instance().start(500);
    IsTimeout = false;
    bool timeoutSate = false;
    Gpio::Out::init();
    while (1) {
        // if (IsTimeout) {
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
