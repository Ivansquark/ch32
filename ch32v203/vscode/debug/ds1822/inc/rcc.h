#ifndef RCC_H
#define RCC_H

#include "main.h"

extern uint32_t SystemCoreClock;
class Rcc {
  public:
      Rcc(uint8_t quarz);
  private:
    void init(uint8_t quarz);
};

#endif // RCC_H
