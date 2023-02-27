#ifndef FIRMWARE_UPDATE_H
#define FIRMWARE_UPDATE_H

#include "flash.h"
#include "my_tcp.h"

namespace FirmwareUpdate {

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

typedef enum updateStates
{
    REPLY_MODE_TO_PC,
    WAIT_FOR_START,
    WAIT_FOR_END,
    REBOOT
} UpdateStates;

void firmwareUpdate_init(void);

void setFirmwareUpdateFlag(bool state);
bool getFirmwareUpdateFlag();

bool firmwareUpdateHandler(void);
void reboot();
void setBootFlagAndReboot(void);
void parsePayload(uint8_t* buf, volatile uint16_t len);

} // namespace FirmwareUpdate

#endif // FIRMWARE_UPDATE_H
