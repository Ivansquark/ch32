#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "main.h"
#include "eth.h"
#include "uart.h"
#include "i2c.h"
#include "adc.h"
#include "freq_by_exti.h"
#include "firmware_update.h"
#include "my_udp.h"

namespace Protocol {

enum ProtocolInterface : bool {
    UDP,
    UART
};

enum SetState_type : uint8_t
{
    SetReboot,
    SetUpdateFirmware
};
// request  ('<'), (length = 5), (SetState_type), (CS - 2 bytes)
// reply    ('>'), (length = 6), (SetState_type), (1 byte status), (CS - 2 bytes)

void setCurrentInterface(ProtocolInterface interface);
ProtocolInterface getCurrentProtocolInterface();

void parseFrame(uint8_t* data, uint8_t len);
void sendData(uint8_t* data, uint8_t len);
uint16_t Crc16(uint8_t *data, uint8_t len);

} // namespace Protocol

#endif // PROTOCOL_H
