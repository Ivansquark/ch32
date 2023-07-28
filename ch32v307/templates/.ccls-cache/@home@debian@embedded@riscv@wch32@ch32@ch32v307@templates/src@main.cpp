#include "main.h"

#include "adc.h"
#include "basic_timer.h"
#include "gpio.h"
#include "led.h"
#include "rcc.h"

#include <array>

/* Global Variable */
uint32_t SystemCoreClock = 144000000;
//------------- objects in static memory (.data section) ----------------------
Rcc rcc(24);
// Adc adc1;
//-----------------------------------------------------------------------------
// void timeout();

int main(void) {
    // Gpios::In<Gpios::PA, 2, Gpios::InitModeIn::FLOATING> in2;
    // volatile bool x = in2.getState();
    // Gpios::init_in2();
    // volatile bool x = Gpios::get_in2();
    //__enable_irq();
    // BasicTimer6::Instance().setCallback(timeout);
    // BasicTimer6::Instance().start(500);
    // Gpio::Out::init();
    Led leds;
    // Gpios::initLedGreen();
    std::array<uint8_t, 2> arr{0,1};
    Gpios::Out<Gpios::PA, 15> a15;
    while (1) {
        leds.green_turnOn();
        auto fptr1 = [&]() { a15.setLow(); };
        fptr1();
        // Gpios::setLedGreen();
        for (volatile int i = 0; i < 1000000; i++) {}
        leds.green_turnOff();
        // void (Led::*fptr)() = &Led::green_turnOff;
        //(leds.*fptr)();
        auto fptr = [&]() { a15.setHigh(); };
        fptr();
        // Gpios::resetLedGreen();
        for (volatile int i = 0; i < 1000000; i++) {}
    }
}

void timeout() {
    // callback function from hardware timer
}
