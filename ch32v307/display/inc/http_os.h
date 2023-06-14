#ifndef HTTP_OS_H
#define HTTP_OS_H

#include "adc.h"
#include "eth.h"
#include "frwrapper.h"
#include "main.h"
#include "w25q.h"

class HttpOS : public FR_OS {
    enum ParseState : uint8_t
    {
        NOT,
        GET_HTML,
        GET_ICO,
        GET_CSS,
        GET_JS,
        GET_CONTENT,
        GET_LED_ON,
        GET_LED_OFF
    };

  public:
    HttpOS(const char* recv_buf, uint16_t stackSize = 256,
           BaseType_t priority = 0, void* pvParameters = nullptr);
    ~HttpOS();
    void runTask(void* pvParameters);

  private:
    ParseState parse(const char* buf, uint16_t len);
    char* recv_buf = nullptr;
    static constexpr uint16_t RECV_BUF_SIZE = 4096;
};

#endif // HTTP_OS_H
