#ifndef MODBUS_H
#define MODBUS_H

#include <stdint.h>
namespace Modbus {
uint16_t crc16(uint8_t* data, uint16_t len);

bool checkCS(uint8_t* arr, int len);

enum SetRegs : uint16_t {
    SetPositionLift = 200,
    SetPositionRotation = 201,
    SetPositionPress = 202,
    SetSpeedLift = 203,
    SetSpeedRotation = 204,
    SetSpeedPress = 205,
    SetAccelerationLift = 206,
    SetAccelerationRotation = 207,
    SetAccelerationPress = 208,
    SetMomentPress = 209,
    SetWordControl = 210 // 0x2000//209
};

enum GetRegs : uint16_t {
    GetSpeedLift = 300,
    GetSpeedRotation = 301,
    GetSpeedPress = 302,
    GetSettedMoment = 303,
    //--------- read next 5 regs in one packet -------------
    GetCurrentMoment = 304,
    GetPositionLift = 305,
    GetPositionRotation = 306,
    GetPositionPress = 307,
    GetWordStatus = 308,
    //------------------------------------------------------
    GetErrorCode = 309,
};

enum ControlWord : uint16_t {
    CONTROL_NONE = 0x0000,
    START_LIFT = 0x0001,
    START_ROTATION = 0x0002,
    START_PRESS_PEDAL_DOWN = 0x0004,
    START_PRESS_PEDAL_UP = 0x0008,
    START_PRESS_ENCODER_DOWN = 0x0010,
    START_PRESS_ENCODER_UP = 0x0020,
    STOP_ALL = 0x0040,
    STOP_LIFT = 0x0080,
    STOP_ROTATION = 0x0100,
    STOP_PRESS = 0x0200,
    START_PRESS_AXIS_CALIBRATION = 0x0400,
    APPLY_SPEED_ACCELERATION_LIFT = 0x0800,
    APPLY_SPEED_ACCELERATION_ROTATION = 0x1000,
    APPLY_SPEED_ACCELERATION_PRESS = 0x2000
};
enum StateWord : uint16_t {
    STATE_NONE = 0x0000,
    ERROR_LIFT = 0x0001,
    ERROR_ROTATION = 0x0002,
    ERROR_PRESS = 0x0004,
    PULS = 0x0008,
    IS_LIFT = 0x0010,
    IS_ROTATION = 0x0020,
    IS_PRESS = 0x0040,
    IS_AXIS_CALIBRATION = 0x0080,
};
} // namespace Modbus

#endif // MODBUS_H
