#ifndef MY_HTTP_H
#define MY_HTTP_H

#include "main.h"

class Http {
  public:
    Http();

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

  private:
    void init();
};

#endif // MY_HTTP_H
