
#include "main.h"

#include "adc.h"
#include "basic_timer.h"
#include "gpio.h"
#include "led.h"
#include "rcc.h"
#include "tirq.h"

#include <algorithm>
#include <array>
#include <stdlib.h>
#include <vector>

/* Global Variable */
uint32_t SystemCoreClock = 144000000;
//------------- objects in static memory (.data section) ----------------------
Rcc rcc(24);
// Adc adc1;
//-----------------------------------------------------------------------------
// void timeout();

void bubble(uint8_t* arr, int size);
void sort();

template<typename T>
class LedIrq {
    static void interruptHandler() {
        
    }
};

int main(void) {
    //Gpios::In<Gpios::PA, 1, Gpios::InitModeIn::ANALOG> a1;
    //Gpios::In<Gpios::PA, 13, Gpios::InitModeIn::ANALOG> a13;
    //Gpios::In<Gpios::PA, 14, Gpios::InitModeIn::ANALOG> a14;
    // volatile bool x = Gpios::get_in2();
    // BasicTimer6::Instance().setCallback(timeout);
    //BasicTimer6::Instance().start(500);
    // Gpio::Out::init();    
    //const Led leds;
    //ObserverForInterrupt<TIM6_IRQn, Led> timIrq;
    //__enable_irq();
    //Gpios::initLedGreen();

    // on Os code size as like pure C functions
    //Gpios::Out<Gpios::PA, 15> a15;
    //a15.setSpeed(Gpios::PortSpeed::LOW);
    while (1) {
        //leds.green_turnOn();
        //auto fptr1 = [&]() { a15.setLow(); };
        //fptr1();
        //Gpios::setLedGreen();
        //for (volatile int i = 0; i < 1000000; i++) {}
        //leds.green_turnOff();
        // void (Led::*fptr)() = &Led::green_turnOff;
        //(leds.*fptr)();
        //auto fptr = [&]() { a15.setHigh(); };
        //fptr();
        //Gpios::resetLedGreen();
        //for (volatile int i = 0; i < 1000000; i++) {}
    }
}

void bubble(uint8_t* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = i; j >= 0; j--) {
            if (arr[j + 1] < arr[j]) {
                uint8_t temp = arr[j + 1];
                arr[j + 1] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

void sort() {
    std::array<uint8_t, 10> arr{0, 0, 11, 14, 1, 2, 4, 3, 6, 7};
    arr[0] = 1;
    uint8_t arr1[10] = {0, 0, 11, 14, 1, 2, 4, 3, 6, 7};
    arr1[0] = std::move(arr[0]);
    // on Os code size 2172 C stdlib
    // qsort(arr1, 10, sizeof(uint8_t), [](const void* i, const void* j) -> int
    // {
    //    return (int)(*(uint8_t*)i - *(uint8_t*)j);
    //});
    // on Os code size 1972 C++ stdlib
    // std::sort(arr.begin(), arr.end());
    // on Os code size 1408
    bubble(arr.data(), 10);
    // arr1[0] = arr[0];
}
void timeout() {
    // callback function from hardware timer
}
