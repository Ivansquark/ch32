#ifndef UART_H
#define UART_H

#include "main.h"
#include "protocol.h"

class Uart {
  public:
    Uart();
    void sendByte(uint8_t byte);
    void sendStr(const char* str);
    void sendBuf(uint8_t* data, uint8_t len);
    bool interruptFlag = false;
    uint8_t byteRead = 0;
    static Uart* pThis;

    static constexpr uint8_t TEMP_SIZE = 10;
    uint8_t tempBuff[TEMP_SIZE] = {0};
    uint8_t tempBuffCounter = 0;

  private:
    void init();
    Uart(const Uart&) = delete;
    Uart(Uart&&) = delete;
};

extern "C" __attribute__((interrupt)) void USART3_IRQHandler(void);

#endif // UART_H
