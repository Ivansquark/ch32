#ifndef DAC_H
#define DAC_H

#include "main.h"

class Dac {
  public:
    Dac();
    static Dac* pThis;
    inline void setU(uint16_t val) { DAC->R12BDHR1 = val << 4; }
    inline void setI(uint16_t val) { DAC->R12BDHR2 = val << 4; }
    inline void clearU() { DAC->DOR1 = 0; }
    inline void clearI() { DAC->DOR2 = 0; }
    void sin(uint32_t width);

  private:
    void init(void);
    uint16_t buf_sin[60] = {
        11,   45,   100,  177,  274,  391,  526,  678,  844,  1024, 1215, 1415,
        1622, 1834, 2048, 2262, 2474, 2681, 2881, 3072, 3252, 3418, 3570, 3705,
        3822, 3919, 3996, 4051, 4085, 4095, 4085, 4051, 3996, 3919, 3822, 3705,
        3570, 3418, 3252, 3072, 2881, 2681, 2474, 2262, 2048, 1834, 1622, 1415,
        1215, 1024, 844,  678,  526,  391,  274,  177,  100,  45,   11,   0};
};

#endif // DAC_H
