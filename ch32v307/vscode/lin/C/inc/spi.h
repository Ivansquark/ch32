#ifndef SPI_H
#define SPI_H

#include "ch32v30x_spi.h"
#include "ch32v30x_rcc.h"
#include "main.h"

void spi2_init();
uint8_t spi2_readWriteByte(uint8_t byte);

#endif // SPI_H
