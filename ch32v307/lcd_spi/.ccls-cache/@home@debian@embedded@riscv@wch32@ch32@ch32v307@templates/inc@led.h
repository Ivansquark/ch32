#ifndef LED_H
#define LED_H

#include "main.h"

class Led {
  public:
    inline Led(){}
    //static Led* pThis;
    inline void green_turnOn() {
        b4.setHigh();
    }
    inline void green_turnOff() const {
        b4.setLow();
    }
    //void red_turnOn();
    //void red_turnOff();
  private:
    //Gpios::OUT<Gpios::PortName::PB, 4, Gpios::InitModeOut::PUSH_PULL> b4;
    Gpios::Out<Gpios::PortName::PB, 4, Gpios::InitModeOut::PUSH_PULL> b4;
    //Gpios::Out<Gpios::PortName::PA, 15, Gpios::InitModeOut::PUSH_PULL> a15;
};

#endif // LED_H
