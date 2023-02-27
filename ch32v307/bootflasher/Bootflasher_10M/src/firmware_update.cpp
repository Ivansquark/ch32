#include "firmware_update.h"

namespace FirmwareUpdate {
UpdateStates currentUpdateState;
uint16_t packet_counter;
uint16_t CS;
uint32_t FullFirmwareLength;
uint8_t currentSectorNum;
uint32_t oneSectorBufferCounter;

static bool OK_or_Error_global;

bool firmwareUpdateFlag = false;
} // namespace FirmwareUpdate

void FirmwareUpdate::firmwareUpdate_init(void) {
    currentUpdateState = REPLY_MODE_TO_PC;
    packet_counter = 0;
    CS = 0;
    FullFirmwareLength = 0;
    currentSectorNum = 0;
    oneSectorBufferCounter = 0;
    OK_or_Error_global = true;
}

void FirmwareUpdate::setFirmwareUpdateFlag(bool state) {
    firmwareUpdateFlag = state;
}

bool FirmwareUpdate::getFirmwareUpdateFlag() { return firmwareUpdateFlag; }

void FirmwareUpdate::reboot() {
    //______________ Software RESET _______________________
    NVIC_SystemReset();
}

void FirmwareUpdate::setBootFlagAndReboot(void) {
    Flash::pThis->writeBootFlag();
    reboot();
}

bool FirmwareUpdate::firmwareUpdateHandler(void) {
    bool OK_or_ERROR = true;
    switch (currentUpdateState) {
    case REPLY_MODE_TO_PC:
        currentUpdateState = WAIT_FOR_START;
        break;
    case WAIT_FOR_START:
        break;
    case WAIT_FOR_END:
        // TODO: if end then send to PC shutdown and then setBootFlagAndReboot()
        break;
    case REBOOT:
        break;
    }
    OK_or_ERROR = OK_or_Error_global;
    return OK_or_ERROR;
}

void FirmwareUpdate::parsePayload(uint8_t* buf, volatile uint16_t len) {
    bool OK_or_ERROR = true;
    if (buf[0] == START) {
        // TODO: write to tcp "start"
        uint8_t sendBuf[1] = {0};
        sendBuf[0] = START;
        Tcp::pThis->lenSendTCP = sizeof(sendBuf);
        memcpy((void*)(Tcp::pThis->frame), (const void*)sendBuf,
               sizeof(sendBuf));
        tcp_write(Tcp::pThis->tpcbPtr, (const void*)(Tcp::pThis->frame),
                  Tcp::pThis->lenSendTCP, 1);
        currentUpdateState = WAIT_FOR_END;
        // clear variables
        packet_counter = 0;
        CS = 0;
        FullFirmwareLength = 0;
        currentSectorNum = 0;
        oneSectorBufferCounter = 0;
    } else if (buf[0] == TO_BOARD) {
        uint16_t tempPacketNumber = (buf[1] << 8) | buf[2];
        volatile uint16_t tempLen = len - 3;
        uint8_t* frameBuf = &buf[3];
        uint8_t sendBuf[3] = {0};
        // receive 512 data bytes, write to flash
        if (packet_counter == (tempPacketNumber)) {
            Flash::pThis->clearRamBuffer();
            if (tempLen < 512) {
                // last packet
                if (tempLen <= 256) {
                    memcpy((Flash::pThis->ramPage), frameBuf, tempLen);
                    Flash::pThis->programPage(
                        Flash::pThis->FLASH_BANK2_FIRMWARE +
                            packet_counter * 512,
                        Flash::pThis->ramPage, Flash::pThis->PAGE_SIZE);

                } else {
                    memcpy((Flash::pThis->ramPage), frameBuf, 256);
                    Flash::pThis->programPage(
                        Flash::pThis->FLASH_BANK2_FIRMWARE +
                            packet_counter * 512,
                        Flash::pThis->ramPage, Flash::pThis->PAGE_SIZE);
                    Flash::pThis->clearRamBuffer();
                    memcpy((Flash::pThis->ramPage), frameBuf + 256,
                           256);
                    Flash::pThis->programPage(
                        Flash::pThis->FLASH_BANK2_FIRMWARE +
                            packet_counter * 512 + 256,
                        Flash::pThis->ramPage, Flash::pThis->PAGE_SIZE);
                }
            } else if (tempLen == 512) {
                memcpy((Flash::pThis->ramPage), frameBuf, tempLen / 2);
                Flash::pThis->programPage(
                    Flash::pThis->FLASH_BANK2_FIRMWARE + packet_counter * 512,
                    Flash::pThis->ramPage, Flash::pThis->PAGE_SIZE);
                memcpy((Flash::pThis->ramPage), frameBuf + 256, tempLen / 2);
                Flash::pThis->programPage(Flash::pThis->FLASH_BANK2_FIRMWARE +
                                              packet_counter * 512 + 256,
                                          Flash::pThis->ramPage,
                                          Flash::pThis->PAGE_SIZE);
            } else {
                // TODO: error
            }

            oneSectorBufferCounter += 512;
            FullFirmwareLength += tempLen;
            sendBuf[0] = TO_PC;
            sendBuf[1] = packet_counter >> 8;
            sendBuf[2] = packet_counter;
            Tcp::pThis->lenSendTCP = sizeof(sendBuf);
            memcpy((void*)(Tcp::pThis->frame), (const void*)sendBuf,
                   sizeof(sendBuf));
            tcp_write(Tcp::pThis->tpcbPtr, (const void*)(Tcp::pThis->frame),
                      Tcp::pThis->lenSendTCP, 1);
            packet_counter++;
        } else {
            // TODO:: error
        }
    } else if (buf[0] == END) {
        uint8_t sendBuf[1] = {0};
        uint16_t receivedCS = ((buf[1] << 8) | (buf[2]));
        // flash last sector
        {
            currentSectorNum++;
            oneSectorBufferCounter = 0;
            // Flash sector
            // OK_or_ERROR = Flash::pThis->writeBufBank2(FullFirmwareLength);
            Flash::pThis->clearRamBuffer();
        }
        // check ControlSum writed in  flash
        CS = Tcp::pThis->checkSum(
            (uint8_t*)(long)Flash::pThis->FLASH_BANK2_FIRMWARE,
            FullFirmwareLength);
        if (CS != receivedCS) {
            CS = 0;
            // send "CSNO"
            sendBuf[0] = CS_NOT_OK;
            Tcp::pThis->lenSendTCP = sizeof(sendBuf);
            memcpy((void*)(Tcp::pThis->frame), (const void*)sendBuf,
                   sizeof(sendBuf));
            tcp_write(Tcp::pThis->tpcbPtr, (const void*)(Tcp::pThis->frame),
                      Tcp::pThis->lenSendTCP, 1);
        } else {
            sendBuf[0] = CS_OK;
            Tcp::pThis->lenSendTCP = sizeof(sendBuf);
            memcpy((void*)(Tcp::pThis->frame), (const void*)sendBuf,
                   sizeof(sendBuf));
            tcp_write(Tcp::pThis->tpcbPtr, (const void*)(Tcp::pThis->frame),
                      Tcp::pThis->lenSendTCP, 1);
        }
    } else if (buf[0] == BOARD_REBOOT) {
        // PC allready go to shutdown, => must wait 40 sec and set boot flag and
        // reboot
        setBootFlagAndReboot();
    }
    OK_or_Error_global = OK_or_ERROR;
}
