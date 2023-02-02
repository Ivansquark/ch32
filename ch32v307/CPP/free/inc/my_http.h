#ifndef MY_HTTP_H
#define MY_HTTP_H

#include "adc.h"
#include "gpio.h"
#include "main.h"
#include "my_tcp.h"
#include "string.h"
#include "w25q.h"

class Http {
  public:
    Http();
    W25q w25q;
    Tcp tcp;
    static Http* pThis;

    bool isHttpRequest();
    void httpHandler();

    // struct Tcp::server_struct* es = NULL;
    tcp_pcb* pcb = NULL;

  private:
    void init();
    uint16_t debugVal = 0;
    uint8_t esCounter = 0;
    // uint8_t buff[1024 * 2] = {0};
    uint32_t retryCounter = 0;
    static constexpr uint32_t MaxRetryCount = 0xFFFF;
};

#endif // MY_HTTP_H
