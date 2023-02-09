#ifndef EEPROM_H
#define EEPROM_H

#include "ch32v30x_i2c.h"
#include "main.h"
//@ brief ft24c64

#define ADDRESS_R  0xA0
#define ADDRESS_W  0xA1

uint8_t Eeprom_readByte(uint16_t address);
void Eeprom_writeByte(uint16_t address, uint8_t byte);

void Eeprom_init();

#endif // EEPROM_H
