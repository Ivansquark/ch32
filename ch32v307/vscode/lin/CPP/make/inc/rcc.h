#ifndef RCC_H
#define RCC_H

#include "main.h"

class Rcc {
  public:
    explicit Rcc(uint8_t quarz);

  private:
    static void init(uint8_t quarz);
};

#endif // RCC_H
