#ifndef TCP_OS_H
#define TCP_OS_H

#include "eth.h"
#include "frwrapper.h"
#include "main.h"
#include "http_os.h"

class TcpOS : public FR_OS {
  public:
    TcpOS(const char* recv_buf);
    void runTask(void* pvParameters) override;
    private:
    char* recv_buf = nullptr;
};

#endif // TCP_OS_H
