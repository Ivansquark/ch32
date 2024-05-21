#ifndef DMA_H
#define DMA_H

#include "main.h"
#include <string.h>

class Dma2 {
  public:
    Dma2();
    static bool IsWriting;
    uint8_t data6[16] = {0};
    uint8_t data8[16] = {0};
    bool write6(const uint8_t* arr, uint16_t len);
    bool write8(const uint8_t* arr, uint16_t len);

  private:
};

#endif // DMA_H
