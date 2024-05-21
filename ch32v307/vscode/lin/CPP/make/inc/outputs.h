#ifndef OUTPUTS_H
#define OUTPUTS_H

#include "main.h"

class Outputs {
  public:
    Outputs();
    static Outputs* pThis;

    inline static void motor_on() { Outputs::pThis->Motor_PW.setHigh(); }
    inline static void motor_off() { Outputs::pThis->Motor_PW.setLow(); }
    inline static void PW_on() { Outputs::pThis->PW.setHigh(); }
    inline static void PW_off() { Outputs::pThis->PW.setLow(); }
    inline static void REZERV1_on() { Outputs::pThis->REZERV1.setHigh(); }
    inline static void REZERV1_off() { Outputs::pThis->REZERV1.setLow(); }
    inline static void lightOn() { Outputs::pThis->LightOnOff.setLow(); }
    inline static void lightOff() { Outputs::pThis->LightOnOff.setHigh(); }
    inline static void bookSplayOn() { Outputs::pThis->BookSplay.setHigh(); }
    inline static void bookSplayOff() { Outputs::pThis->BookSplay.setLow(); }
    inline static void bookStartOn() { Outputs::pThis->BookStart.setHigh(); }
    inline static void bookStartOff() { Outputs::pThis->BookStart.setLow(); }

  private:
    Gpios::Out<Gpios::PC, 10, Gpios::InitModeOut::PUSH_PULL> Motor_PW;
    Gpios::Out<Gpios::PA, 15, Gpios::InitModeOut::PUSH_PULL> PW;
    Gpios::Out<Gpios::PC, 11, Gpios::InitModeOut::PUSH_PULL> REZERV1;
    Gpios::Out<Gpios::PA, 6, Gpios::InitModeOut::PUSH_PULL> LightOnOff;
    Gpios::Out<Gpios::PA, 2, Gpios::InitModeOut::PUSH_PULL> BookSplay;
    Gpios::Out<Gpios::PE, 5, Gpios::InitModeOut::PUSH_PULL> BookStart;
};

#endif // OUTPUTS_H
