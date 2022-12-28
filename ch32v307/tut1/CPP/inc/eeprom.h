#ifndef EEPROM_H
#define EEPROM_H

#include "ch32v30x_i2c.h"
#include "main.h"
//@ brief ft24c64
class Eeprom {
  public:
    Eeprom();

    uint8_t readByte(uint16_t address);
    void writeByte(uint16_t address, uint8_t byte);
        
  private:
    void init();
    static constexpr uint8_t ADDRESS_R = 0xA1;
    static constexpr uint8_t ADDRESS_W = 0xA0;
};

#endif // EEPROM_H
