#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

namespace Protocol {
typedef enum
{
    START = 's',
    END = 'e',
    TO_BOARD = '>',     //
    TO_PC = '<',        //
    CS_OK = 'o',        //
    CS_NOT_OK = 'n',    //
    BOARD_REBOOT = 'r', //
} pl_updateFirmware;

enum ProtocolInterface : bool
{
    UDP,
    UART
};

enum SetState_type : uint8_t
{
    SetReboot,
    SetUpdateFirmware
};

uint16_t Crc16(uint8_t* data, uint16_t len);
void setCurrentInterface(ProtocolInterface interface);
ProtocolInterface getCurrentProtocolInterface();

} // namespace Protocol

#endif // PROTOCOL_H
