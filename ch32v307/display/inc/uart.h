#ifndef UART_H
#define UART_H

#include "main.h"
#include "frwrapper.h"
#include <stdio.h>

class Uart5 : public FR_OS {
  public:
    Uart5();
    static Uart5* pThis;
    void sendByte(uint8_t byte);
    void sendStr(const char* str);
    void sendBuf(uint8_t* data, uint8_t len);
    void runTask(void* pvParameters) override;
  private:
    void init();
    uint8_t TxBuff[256] = {0};
    uint8_t TxLen = 0;
};

class Uart1 {
  public:
    Uart1();
    static Uart1* pThis;
    void sendByte(uint8_t byte);
    void sendStr(const char* str);
    void sendBuf(uint8_t* data, uint8_t len);
    uint8_t rxBuf[256];
    uint8_t rxCounter = 0;
  private:
    void init();
};

class Uart3 {
  public:
    Uart3();
    static Uart3* pThis;
    void sendByte(uint8_t byte);
    void sendStr(const char* str);
    void sendBuf(uint8_t* data, uint8_t len);
    uint8_t rxBuf[256];
    uint8_t rxCounter = 0;
  private:
    void init();
};
#endif // UART_H
