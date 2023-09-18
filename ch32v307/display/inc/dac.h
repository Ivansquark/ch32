#ifndef DAC_H
#define DAC_H

#include "main.h"

class Dac {
  public:
    Dac();
    static Dac* pThis;
    inline void setBacklight(uint8_t val) {
        currentPercent = val;
        DAC->R12BDHR1 = val * coef100;
    }
    inline void increaseBacklight() {
        if(currentPercent < 99) ++currentPercent;
        DAC->R12BDHR1 = currentPercent * coef100;
    }
    inline void decreaseBacklight() {
        if (currentPercent > 1) --currentPercent;
        DAC->R12BDHR1 = currentPercent * coef100;
    }
    inline void clearU() { DAC->R12BDHR2 = 0; }
    inline void clearI() { DAC->R12BDHR1 = 0; }

  private:
    void init(void);
    uint8_t currentPercent = 0;
    static constexpr uint8_t coef100 = 4095 / 100;
};

#endif // DAC_H
