#include "protocol.h"

namespace Protocol {
ProtocolInterface currentInterface = ProtocolInterface::UDP;
}

void Protocol::setCurrentInterface(ProtocolInterface interface) {
    currentInterface = interface;
}
Protocol::ProtocolInterface Protocol::getCurrentProtocolInterface() {
    return currentInterface;
}

void Protocol::parseFrame(uint8_t* data, uint8_t len) {
    // TODO: check CS
    uint16_t crc_received = data[len - 1] + (data[len - 2] << 8);
    uint16_t crc_counted = Crc16(data, len - 2);
    if (crc_received != crc_counted) {
        uint8_t errorBuf[6] = {'>', 6, SetState_type::SetReboot, 0, 0};
        uint16_t errorBufCrc = Crc16(errorBuf, 4);
        errorBuf[4] = errorBufCrc >> 8;
        errorBuf[5] = errorBufCrc;
        Uart::pThis->sendBuf(errorBuf, sizeof(errorBuf));
        // Udp::pThis->sendToPC(errorBuf, sizeof(errorBuf));
        Udp::pThis->sendToControl(errorBuf, sizeof(errorBuf));
        return;
    }
    switch (data[2]) {
    case SetState_type::SetReboot:
        // HAL_Delay(100);
        FirmwareUpdate::reboot();
        break;
    case SetState_type::SetUpdateFirmware:
        FirmwareUpdate::setFirmwareUpdateFlag(true);
        break;
    }
}

uint16_t Protocol::Crc16(uint8_t* data, uint8_t length) {
    unsigned short reg_crc =
        0xFFFF; // Load a 16–bit register with FFFF hex (all 1’s).
    while (length--) {
        unsigned char data1 = *data;
        data++;
        reg_crc ^= data1;
        // Exclusive OR the first 8–bit byte of the message with the low–order
        // byte of the 16–bit CRC register, putting the result in the CRC
        // register.
        for (int j = 0; j < 8; j++) {
            // Shift the CRC register one bit to the right (toward the LSB),
            // zero–filling the MSB. //"EXTRACT!!!- compare byte before XOR-а =>
            // в if-е" and examine the LSB.
            //(If the LSB was 1): Exclusive OR the CRC register with the
            // polynomial value 0xA001 (1010 0000 0000 0001).
            if (reg_crc & 0x0001) {
                reg_crc = (reg_crc >> 1) ^ 0xA001;
            } // LSB(b0)=1
            // (If the LSB was 0): Repeat Step 3 (another shift).
            else
                reg_crc = reg_crc >> 1;
        }
    }
    return reg_crc;
}

void Protocol::sendData(uint8_t* data, uint8_t len) {
    if (currentInterface == ProtocolInterface::UART) {
        Uart::pThis->sendBuf(data, len);
    } else {
        Udp::pThis->sendToControl(data, len);
        Udp::pThis->sendToPC(data, len);
    }
}
