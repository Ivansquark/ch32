#ifndef RS485_H
#define RS485_H

#include "main.h"

class RS485 {
  public:
    RS485();
    static RS485* pThis;

    //---------------  constants  ---------------------------------------------
    static constexpr uint8_t SIZE_RECEIVE = 5;
    static constexpr uint8_t SIZE_TRANSMIT = 5;

    //----------------  receive  ----------------------------------------------
    uint8_t arr[SIZE_RECEIVE] = {0};
    uint8_t counterBytes = 0;
    uint32_t counter_ms = 0;

    //---------------   transmit  ---------------------------------------------
    void writeBytes(const uint8_t* arr, uint8_t len);

    //---------------  driver  ------------------------------------------------
    inline void setRead() {
        // TODO set pin low
    }
    inline void setWrite() {
        // TODO set pin high
    }

    void setIsNeedRead(bool state) { IsNeedRead = state; };
    bool getIsNeedRead() { return IsNeedRead; };

  private:
    void init();
    bool IsReadNow = false;
    bool IsNeedRead = true;

    void writeByte(uint8_t byte);
};

#endif // RS485_H
