#ifndef BUZZER_H
#define BUZZER_H

#include "gpios.h"
#include "main.h"

class Buzzer {
  public:
    Buzzer();
    static Buzzer* pThis;

    void start();
    void stop();

    void handler();
    void setMustBuzz(bool state) { MustBuzz = state; }
    bool setMustBuzz() const { return MustBuzz; }

  private:
    static void init();
    bool MustBuzz = false;
    Gpios::Out<Gpios::PE, 1, Gpios::InitModeOut::PUSH_PULL> bzz;
};

#endif // BUZZER_H
