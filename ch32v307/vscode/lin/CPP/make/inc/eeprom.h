#ifndef EEPROM_H
#define EEPROM_H

#include "ch32v30x_i2c.h"
#include "gpios.h"
#include "main.h"

class Eeprom {
  public:
    Eeprom();
    static volatile int16_t liftPosition;
    static volatile int16_t rotationPosition;
    static volatile int16_t pressPosition;
    static volatile int16_t pressMoment;

    static void writeLiftPosition(int16_t val);
    static void writeRotationPosition(int16_t val);
    static void writePressPosition(int16_t val);
    static void writePressMoment(int16_t val);
    static int16_t readLiftPosition();
    static int16_t readRotationPosition();
    static int16_t readPressPosition();
    static int16_t readPressMoment();

    static void increaseRotationPosition();
    static void decreaseRotationPosition();
    static void increasePressMoment();
    static void decreasePressMoment();

  private:
    void init();
    void readEeprom();
    static constexpr uint8_t ADDRESS_R = 0xA1;
    static constexpr uint8_t ADDRESS_W = 0xA0;

    static constexpr uint16_t ADDRESS_LIFT_POSITION = 0x00;
    static constexpr uint16_t ADDRESS_ROTATION_POSITION = 0x02;
    static constexpr uint16_t ADDRESS_PRESS_POSITION = 0x04;
    static constexpr uint16_t ADDRESS_PRESS_MOMENT = 0x06;

    //----------------- constants in mm and N
    static constexpr uint16_t LIFT_POSITION_MAX = 2000;
    static constexpr uint16_t ROTATION_POSITION_MAX = 80;
    static constexpr uint16_t PRESS_POSITION_MAX = 200;
    static constexpr uint16_t PRESS_POSITION_MIN = 10;
    static constexpr uint16_t PRESS_MOMENT_MAX = 200;

    static void writeShort(uint16_t addr, uint16_t val);
    static uint16_t readShort(uint16_t addr);
    static void writeByte(uint16_t address, uint8_t byte);
    static uint8_t readByte(uint16_t address);

    Gpios::Out<Gpios::PB, 6, Gpios::InitModeOut::ALTERNATE_OPEN_DRAIN> scl;
    Gpios::Out<Gpios::PB, 7, Gpios::InitModeOut::ALTERNATE_OPEN_DRAIN> sda;
};

#endif // EEPROM_H
