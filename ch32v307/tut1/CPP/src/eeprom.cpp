#include "eeprom.h"

Eeprom::Eeprom() { init(); }

uint8_t Eeprom::readByte(uint16_t address) {
    uint16_t timeout = 0xFFFF;
    I2C_AcknowledgeConfig(I2C2, FunctionalState::ENABLE);
    I2C_GenerateSTART(I2C2, FunctionalState::ENABLE);
    while (!(I2C2->STAR1 & I2C_FLAG_SB) && timeout--) {}
    (void)I2C1->STAR1;

    I2C_Send7bitAddress(I2C2, ADDRESS_W, I2C_Direction_Transmitter);
    timeout = 0xFFFF;
    while (!(I2C2->STAR1 & I2C_FLAG_ADDR) && timeout--) {}
    (void)I2C1->STAR1;
    (void)I2C1->STAR1;
    I2C_SendData(I2C2, address >> 8);
    timeout = 0xFFFF;
    while (!(I2C2->STAR1 & I2C_FLAG_TXE) && timeout--) {}
    //restart
    I2C_GenerateSTART(I2C2, FunctionalState::ENABLE);
    timeout = 0xFFFF;
    while (!(I2C2->STAR1 & I2C_FLAG_SB) && timeout--) {}
    (void)I2C1->STAR1;
    I2C_SendData(I2C2, address);
    timeout = 0xFFFF;
    while (!(I2C2->STAR1 & I2C_FLAG_TXE) && timeout--) {}
    I2C_Send7bitAddress(I2C2, ADDRESS_R, I2C_Direction_Transmitter);
    timeout = 0xFFFF;
    while (!(I2C2->STAR1 & I2C_FLAG_ADDR) && timeout--) {}
    (void)I2C1->STAR1;
    (void)I2C1->STAR1;
    timeout = 0xFFFF;
    while (!(I2C1->STAR1 & I2C_STAR1_RXNE) && timeout--){}
    uint8_t data = I2C_ReceiveData(I2C2);
    I2C_GenerateSTOP(I2C2, FunctionalState::ENABLE);
    return data;
}
void Eeprom::writeByte(uint16_t address, uint8_t byte) {
    uint16_t timeout = 0xFFFF;
    I2C_AcknowledgeConfig(I2C2, FunctionalState::ENABLE);
    I2C_GenerateSTART(I2C2, FunctionalState::ENABLE);
    while (!(I2C2->STAR1 & I2C_FLAG_SB) && timeout--) {}
    (void)I2C1->STAR1;

    I2C_Send7bitAddress(I2C2, ADDRESS_W, I2C_Direction_Transmitter);
    timeout = 0xFFFF;
    while (!(I2C2->STAR1 & I2C_FLAG_ADDR) && timeout--) {}
    (void)I2C1->STAR1;
    (void)I2C1->STAR1;
    I2C_SendData(I2C2, address >> 8);
    timeout = 0xFFFF;
    while (!(I2C2->STAR1 & I2C_FLAG_TXE) && timeout--) {}
    I2C_SendData(I2C2, address);
    timeout = 0xFFFF;
    while (!(I2C2->STAR1 & I2C_FLAG_TXE) && timeout--) {}
    I2C_SendData(I2C2, byte);
    timeout = 0xFFFF;
    while (!(I2C2->STAR1 & I2C_FLAG_BTF) && timeout--) {}
    I2C_GenerateSTOP(I2C2, FunctionalState::ENABLE);
}
void Eeprom::init() {
    // GPIO init pB10-SCL pB11-SDA 1:1:1:1 alt open_drain max speed
    RCC->APB2PCENR |= RCC_IOPBEN;
    GPIOB->CFGHR |= GPIO_CFGHR_MODE10 | GPIO_CFGHR_MODE11;
    GPIOB->CFGHR |= GPIO_CFGHR_CNF10 | GPIO_CFGHR_CNF11;
    
    RCC->APB1PCENR |= RCC_I2C2EN;
    I2C_InitTypeDef I2C_struct{.I2C_ClockSpeed = 100000,
                               .I2C_Mode = I2C_Mode_I2C,
                               .I2C_DutyCycle = I2C_DutyCycle_2,
                               .I2C_Ack = I2C_Ack_Enable,
                               .I2C_AcknowledgedAddress =
                                   I2C_AcknowledgedAddress_7bit};
    I2C_Init(I2C2, &I2C_struct);
}
