#ifndef UDP_OS_H
#define UDP_OS_H

#include "eth.h"
#include "frwrapper.h"
#include "main.h"

class UdpOS : public FR_OS {
  public:
    UdpOS(uint8_t* recv_buf = nullptr);
    void runTask(void* pvParameters);

  private:
    uint8_t* recv_buf = nullptr;
    static constexpr uint16_t RECV_BUF_SIZE = 256;
};

#endif // UDP_OS_H
