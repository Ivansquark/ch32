#ifndef UART_H
#define UART_H

#include "gpios.h"
#include "main.h"
#include "string.h"

class Uart {
  public:
    Uart();
    static uint8_t uartByte;
    static char rxdata[32];
    static uint8_t counter;
    static char data[32];
    static bool IsReceived;
    static void sendStr(char* arr, int len);

  private:
    void init();
    static void sendbyte(uint8_t byte);
};

//__attribute__((interrupt("WCH-Interrupt-fast")))
extern "C" __attribute__((interrupt("WCH-Interrupt-fast"))) void
USART1_IRQHandler(void);

#endif // UART_H
