#ifndef DS1822_H
#define DS1822_H

#include "main.h"

class DS1822 {
  public:
    DS1822(uint8_t num = 0);
    static DS1822* pThis;

    bool signMinus = false;
    float temp = 0;

    void measureT();

  private:
    uint8_t data[9] = {0};
    bool firstMeasureCmdFlag = false;
    bool init(bool mode);
    void up();
    void down();
    bool isUp();
    bool reset();
    void writeBit(uint8_t bit);
    void writeByte(uint8_t byte);
    bool readBit();
    uint8_t readByte();
    void measureTempCmd(uint8_t mode);
    void readStrat(uint8_t mode, uint8_t* data);
    uint8_t getSign(uint16_t data);
    float convert(uint16_t data);
    void delay_us(volatile uint16_t us);

    uint8_t crc_dallas(uint8_t* data, uint8_t len);

    static constexpr uint8_t RESOLUTION_9BIT = 0x1F;
    static constexpr uint8_t RESOLUTION_10BIT = 0x3F;
    static constexpr uint8_t RESOLUTION_11BIT = 0x5F;
    static constexpr uint8_t RESOLUTION_12BIT = 0x7F;
    static constexpr bool SKIP_ROM = 0;
    static constexpr bool NOSKIP_ROM = 1;

    uint8_t num = 0;
};

#endif // DS1822_H
