#include "eeprom.h"

volatile int16_t Eeprom::liftPosition = 0;
volatile int16_t Eeprom::rotationPosition = 0;
volatile int16_t Eeprom::pressPosition = 0;
volatile int16_t Eeprom::pressMoment = 0;

Eeprom::Eeprom() { init(); }

void Eeprom::writeLiftPosition(int16_t val) {
    writeShort(ADDRESS_LIFT_POSITION, val);
}
void Eeprom::writeRotationPosition(int16_t val) {
    writeShort(ADDRESS_ROTATION_POSITION, val);
}
void Eeprom::writePressPosition(int16_t val) {
    writeShort(ADDRESS_PRESS_POSITION, val);
}
void Eeprom::writePressMoment(int16_t val) {
    writeShort(ADDRESS_PRESS_MOMENT, val);
}

int16_t Eeprom::readLiftPosition() {
    return liftPosition = readShort(ADDRESS_LIFT_POSITION);
}
int16_t Eeprom::readRotationPosition() {
    return rotationPosition = readShort(ADDRESS_ROTATION_POSITION);
}
int16_t Eeprom::readPressPosition() {
    return pressPosition = readShort(ADDRESS_PRESS_POSITION);
}
int16_t Eeprom::readPressMoment() {
    return pressMoment = readShort(ADDRESS_PRESS_MOMENT);
}

void Eeprom::increaseRotationPosition() {
    if(rotationPosition < ROTATION_POSITION_MAX) {
        writeRotationPosition(++rotationPosition);
    }
}
void Eeprom::decreaseRotationPosition() {
    if(rotationPosition > 0) {
        writeRotationPosition(--rotationPosition);
    }

}
void Eeprom::increasePressMoment() {
    if(pressMoment < PRESS_MOMENT_MAX) {
        pressMoment += 10;
        writePressMoment(pressMoment);
    }
}
void Eeprom::decreasePressMoment() {
    if(pressMoment > 0) {
        pressMoment -= 10;
        writePressMoment(pressMoment);
    }
}
void Eeprom::readEeprom() {
    readLiftPosition();
    readRotationPosition();
    readPressPosition();
    readPressMoment();
}
void Eeprom::init() {
    RCC->APB1PCENR |= RCC_I2C1EN;
    I2C_InitTypeDef I2C_struct{.I2C_ClockSpeed = 500000,
                               .I2C_Mode = I2C_Mode_I2C,
                               .I2C_DutyCycle = I2C_DutyCycle_2,
                               .I2C_OwnAddress1 = 0,
                               .I2C_Ack = I2C_Ack_Enable,
                               .I2C_AcknowledgedAddress =
                                   I2C_AcknowledgedAddress_7bit};
    I2C_Init(I2C1, &I2C_struct);
    I2C_Cmd(I2C1, ENABLE);

    I2C_AcknowledgeConfig(I2C1, ENABLE);

    uint8_t IsClearEeprom = readByte(0);
#if 1
    if (IsClearEeprom == 0xFF) {
        writeLiftPosition(LIFT_POSITION_MAX);
        writeRotationPosition(45);
        writePressPosition(100);
        writePressMoment(70);
    } else {
        readEeprom();
    }
#endif
}
void Eeprom::writeShort(uint16_t addr, uint16_t val) {
    writeByte(addr, val >> 8);
    writeByte(addr + 1, val);
}

uint16_t Eeprom::readShort(uint16_t address) {
    uint16_t data = (readByte(address) << 8) + readByte(address + 1);
    return data;
}

void Eeprom::writeByte(uint16_t address, uint8_t byte) {
    volatile uint16_t timeout = 0xFFFF;
    I2C_AcknowledgeConfig(I2C1, FunctionalState::ENABLE);
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET) {}
    I2C_GenerateSTART(I2C1, FunctionalState::ENABLE);
    // while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)){}
    while (!(I2C1->STAR1 & I2C_STAR1_SB) && --timeout) {}
    (void)I2C1->STAR1;

    I2C_Send7bitAddress(I2C1, ADDRESS_W, I2C_Direction_Transmitter);
    timeout = 0xFFFF;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) &&
           --timeout) {}

    I2C_SendData(I2C1, address >> 8);
    timeout = 0xFFFF;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) &&
           --timeout) {}
    I2C_SendData(I2C1, address);
    timeout = 0xFFFF;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) &&
           --timeout) {}
    I2C_SendData(I2C1, byte);
    timeout = 0xFFFF;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) &&
           --timeout) {}
    I2C_GenerateSTOP(I2C1, FunctionalState::ENABLE);
    // delay to write in eeprom
    //for (volatile int i = 0; i < 144000 * 10; i++) {}
    for (volatile int i = 0; i < 144000; i++) {}
}
uint8_t Eeprom::readByte(uint16_t address) {
    volatile uint16_t timeout = 0xFFFF;
    I2C_AcknowledgeConfig(I2C1, FunctionalState::ENABLE);
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET) {}
    I2C_GenerateSTART(I2C1, FunctionalState::ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) && --timeout) {}

    I2C_Send7bitAddress(I2C1, ADDRESS_W, I2C_Direction_Transmitter);
    timeout = 0xFFFF;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) &&
           --timeout) {}

    // send address
    I2C_SendData(I2C1, address >> 8);
    timeout = 0xFFFF;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) &&
           --timeout) {}
    I2C_SendData(I2C1, address);
    timeout = 0xFFFF;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) &&
           --timeout) {}
    // restart
    I2C_GenerateSTART(I2C1, FunctionalState::ENABLE);
    timeout = 0xFFFF;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) && timeout--) {}
    I2C_Send7bitAddress(I2C1, ADDRESS_R, I2C_Direction_Receiver);
    timeout = 0xFFFF;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) &&
           --timeout) {}
    timeout = 0xFFFF;
    while (!(I2C1->STAR1 & I2C_STAR1_RXNE) && --timeout) {
        I2C_AcknowledgeConfig(I2C1, DISABLE);
    }
    uint8_t data = I2C_ReceiveData(I2C1);
    I2C_GenerateSTOP(I2C1, FunctionalState::ENABLE);
    return data;
}
