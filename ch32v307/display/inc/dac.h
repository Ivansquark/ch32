#ifndef DAC_H
#define DAC_H

#include "main.h"

class Dac {
  public:
    Dac();
    static Dac* pThis;
    inline void setBacklight(uint8_t val) { DAC->R12BDHR1 = val * coef100; }
    inline void clearU() { DAC->R12BDHR2 = 0; }
    inline void clearI() { DAC->R12BDHR1 = 0; }
    void sin(uint32_t width);

  private:
    void init(void);
    static constexpr uint8_t coef100 = 4095/100;
};

#endif // DAC_H
