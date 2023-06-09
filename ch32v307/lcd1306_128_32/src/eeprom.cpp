#include "eeprom.h"

Eeprom::Eeprom() { init(); }

uint8_t Eeprom::readByte(uint16_t address) {
    volatile uint16_t timeout = 0xFFFF;
    I2C_AcknowledgeConfig(I2C2, FunctionalState::ENABLE);
    while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY) != RESET) {}
    I2C_GenerateSTART(I2C2, FunctionalState::ENABLE);
    while (timeout-- && !I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)) {}

    I2C_Send7bitAddress(I2C2, ADDRESS_W, I2C_Direction_Transmitter);
    timeout = 0xFFFF;
    while (timeout-- &&
           !I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}
    I2C_SendData(I2C2, address >> 8);
    timeout = 0xFFFF;
    while (timeout-- && !I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}
    I2C_SendData(I2C2, address);
    timeout = 0xFFFF;
    while(timeout-- && !I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){}
    // restart
    I2C_GenerateSTART(I2C2, FunctionalState::ENABLE);
    timeout = 0xFFFF;
    while(timeout-- && !I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)){}
    I2C_Send7bitAddress(I2C2, ADDRESS_R, I2C_Direction_Receiver);
    timeout = 0xFFFF;
    while(timeout-- && !I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){}
    timeout = 0xFFFF;
    while (!(I2C2->STAR1 & I2C_STAR1_RXNE) && timeout--) {
        I2C_AcknowledgeConfig(I2C2, DISABLE);
    }
    uint8_t data = I2C_ReceiveData(I2C2);
    I2C_GenerateSTOP(I2C2, FunctionalState::ENABLE);
    return data;
}
void Eeprom::writeByte(uint16_t address, uint8_t byte) {
    volatile uint16_t timeout = 0xFFFF;
    I2C_AcknowledgeConfig(I2C2, FunctionalState::ENABLE);
    while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY) != RESET) {}
    I2C_GenerateSTART(I2C2, FunctionalState::ENABLE);
    // while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)){}
    while (!(I2C2->STAR1 & I2C_STAR1_SB) && timeout--) {}
    (void)I2C2->STAR1;

    I2C_Send7bitAddress(I2C2, ADDRESS_W, I2C_Direction_Transmitter);
    timeout = 0xFFFF;
    while (timeout-- &&
           !I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}
    I2C_SendData(I2C2, address >> 8);
    timeout = 0xFFFF;
    while (timeout-- &&
           !I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}
    I2C_SendData(I2C2, address);
    timeout = 0xFFFF;
    while (timeout-- &&
           !I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}
    I2C_SendData(I2C2, byte);
    timeout = 0xFFFF;
    while (timeout-- &&
           !I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}
    I2C_GenerateSTOP(I2C2, FunctionalState::ENABLE);
}
void Eeprom::init() {
    // GPIO init pB10-SCL pB11-SDA 1:1:1:1 alt open_drain max speed
    RCC->APB2PCENR |= RCC_IOPBEN;
    GPIOB->CFGHR |= GPIO_CFGHR_MODE10 | GPIO_CFGHR_MODE11;
    GPIOB->CFGHR |= GPIO_CFGHR_CNF10 | GPIO_CFGHR_CNF11;

    RCC->APB1PCENR |= RCC_I2C2EN;
    I2C_InitTypeDef I2C_struct{.I2C_ClockSpeed = 400000,
                               .I2C_Mode = I2C_Mode_I2C,
                               .I2C_DutyCycle = I2C_DutyCycle_2,
                               .I2C_OwnAddress1 = 0,
                               .I2C_Ack = I2C_Ack_Enable,
                               .I2C_AcknowledgedAddress =
                                   I2C_AcknowledgedAddress_7bit};
    I2C_Init(I2C2, &I2C_struct);
    I2C_Cmd(I2C2, ENABLE);

    I2C_AcknowledgeConfig(I2C2, ENABLE);
}
