#ifndef MY_HTTP_H
#define MY_HTTP_H

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
    enum ParseState : uint8_t
    {
        NOT,
        NOT_FULL,
        GET_HTML,
        GET_ICO,
        GET_CSS,
        GET_JS
    };
    ParseState currentParseState = ParseState::NOT;
    ParseState parse(const uint8_t* data, uint16_t len);
    
    bool isHttpRequest();
    void httpHandler();

    //struct Tcp::server_struct* es = NULL;
    tcp_pcb* pcb = NULL;

  private:
    void init();

    //uint8_t buff[1024 * 2] = {0};
};

#endif // MY_HTTP_H
