#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "adc.h"
#include "main.h"

class Actuator {
  public:
    Actuator();
    Adc adc;

    enum State : uint8_t
    {
        DOWN,
        GO_UP,
        UP,
        GO_TO_NEW_UP,
        GO_DOWN
    };
    State currentState = State::DOWN;

    void handler();
    inline void up_start() {
        GPIOE->BSHR |= GPIO_BSHR_BS11;
        GPIOE->BSHR |= GPIO_BSHR_BS11;
    }
    inline void up_stop() {
        GPIOE->BSHR |= GPIO_BSHR_BR11;
        GPIOE->BSHR |= GPIO_BSHR_BR11;
    }

    inline void down_start() {
        GPIOE->BSHR |= GPIO_BSHR_BS12;
        GPIOE->BSHR |= GPIO_BSHR_BS12;
    }
    inline void down_stop() {
        GPIOE->BSHR |= GPIO_BSHR_BR12;
        GPIOE->BSHR |= GPIO_BSHR_BR12;
    }
    inline void stop() {
        up_stop();
        down_stop();
    }
    // adc max = 100% adc min = 0%;
    uint8_t current_percent = 0;
    uint8_t stop_percent = DEFAULT_PERCENT;
    inline void setStopPercent(uint8_t percent) { stop_percent = percent; }
    inline uint8_t getStopPercent() { return stop_percent; }

  private:
    void init();
    static constexpr uint8_t DEFAULT_PERCENT = 50;
    uint16_t adc_down_position = 0;
    uint16_t adc_up_position = 0;
};

#endif // ACTUATOR_H
