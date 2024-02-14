#include "actuator.h"

Actuator::Actuator() { init(); }

void Actuator::handler() {
    switch (currentState) {
    case DOWN:
        // TODO: wait for up signal
        break;
    case GO_UP:
        // wait for adc in some position
        down_stop();
        up_start();
        if(current_percent >= stop_percent) {
            currentState = UP;
        }
        break;
    case UP:
        // wait for down signal or signal to adc position
        break;
    case GO_TO_NEW_UP:
        if(current_percent > stop_percent + 2) {
            // go down
            up_stop();
            down_start();
        } else if (current_percent < stop_percent - 2) {
            // go up
            down_stop();
            up_start();
        } else {
            stop();
            currentState = UP;
        }
        break;
    case GO_DOWN:
        //TODO: wait for limit switch or adc
        break;
    default:
        break;
    }
    //TODO moving average
    current_percent = 100 * adc.getAdc() / 4095;
}

void Actuator::init() {
    // GPIO init:
    // E13-start, E14-stop
    RCC->APB2PCENR |= RCC_IOPEEN;
    GPIOE->CFGHR |= GPIO_CFGHR_MODE13 | GPIO_CFGHR_MODE14;
    GPIOE->CFGHR &= ~(GPIO_CFGHR_CNF13 | GPIO_CFGHR_CNF14);
}
