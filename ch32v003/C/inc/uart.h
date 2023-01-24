#ifndef UART_H
#define UART_H

#include "main.h"

extern uint8_t uartByte;
void uart_init();
void uart_sendbyte(uint8_t byte);

#endif //UART_H
