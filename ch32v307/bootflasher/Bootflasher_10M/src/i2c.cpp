#include "i2c.h"

Pot1::Pot1() {}

void Pot1::writeByte([[maybe_unused]] uint8_t address,
                     [[maybe_unused]] uint8_t byte) {
    // volatile uint16_t timeout = 0xFFFF;
    // I2C1->CR1 |= I2C_CR1_ACK;
    // I2C1->CR1 |= I2C_CR1_START;
    //// wait for start bit
    // while (!(I2C1->SR1 & I2C_SR1_SB) && (timeout--)) {
    //}
    //(void)I2C1->SR1;    // read register to clear start flag
    // I2C3->DR = address; // send byte with device addres and last bit on WR
    // timeout = 0xFFFF;
    // while (!(I2C1->SR1 & I2C_SR1_ADDR) && (timeout--)) {
    //}
    //(void)I2C1->SR1;
    //(void)I2C1->SR2;
    // I2C1->DR = InstructionByte; // send address
    // timeout = 0xFFFF;
    // while (!(I2C1->SR1 & I2C_SR1_TXE) && (timeout--)) {
    //}
    //// I2C1->DR = address & 0xFF; //
    // timeout = 0xFFFF;
    // while (!(I2C1->SR1 & I2C_SR1_TXE) && (timeout--)) {
    //}
    // I2C1->DR = byte;
    // timeout = 0xFFFF;
    // while (!(I2C1->SR1 & I2C_SR1_BTF) && (timeout--)) {
    //}
    // I2C1->CR1 |= I2C_CR1_STOP;
    // HAL_Delay(100);
}

Pot3::Pot3() {}

void Pot3::writeByte([[maybe_unused]] uint8_t address,
                     [[maybe_unused]] uint8_t byte) {
    /*
    volatile uint16_t timeout = 0xFFFF;
    I2C3->CR1 |= I2C_CR1_ACK;
    I2C3->CR1 |= I2C_CR1_START;
    // wait for start bit
    while (!(I2C3->SR1 & I2C_SR1_SB) && (timeout--)) {
    }
    (void)I2C1->SR1;    // read register to clear start flag
    I2C3->DR = address; // send byte with device addres and last bit on WR
    timeout = 0xFFFF;
    while (!(I2C3->SR1 & I2C_SR1_ADDR) && (timeout--)) {
    }
    (void)I2C3->SR1;
    (void)I2C3->SR2;
    I2C1->DR = InstructionByte; // send address
    timeout = 0xFFFF;
    while (!(I2C3->SR1 & I2C_SR1_TXE) && (timeout--)) {
    }
    // I2C1->DR = address & 0xFF; //
    timeout = 0xFFFF;
    while (!(I2C3->SR1 & I2C_SR1_TXE) && (timeout--)) {
    }
    I2C3->DR = byte;
    timeout = 0xFFFF;
    while (!(I2C3->SR1 & I2C_SR1_BTF) && (timeout--)) {
    }
    I2C3->CR1 |= I2C_CR1_STOP;
    HAL_Delay(100);
    */
}

Eeprom* Eeprom::pThis = nullptr;

Eeprom::Eeprom() {
    pThis = this;
    init();
    readU();
    readI();
    readFreqU();
    readFreqI();
    readDose();
    readFors();
    readPodkal();
    readUmax();
    readImax();
    readMode();
    readLastErr();
    readReadErr();
    readIs_T();
    readT_on();
    readT_off();
}

void Eeprom::writeByte(uint8_t address, uint8_t byte) {
    volatile uint16_t timeout = 0xFFFF;
    I2C_AcknowledgeConfig(I2C1, FunctionalState::ENABLE);
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET) {}
    I2C_GenerateSTART(I2C1, FunctionalState::ENABLE);
    // while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)){}
    while (!(I2C1->STAR1 & I2C_STAR1_SB) && timeout--) {}
    (void)I2C1->STAR1;

    I2C_Send7bitAddress(I2C1, ADDRESS_W, I2C_Direction_Transmitter);
    // timeout = 0xFFFF;
    // while (timeout-- &&
    //       !I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    //       {}
    // I2C_SendData(I2C2, address >> 8);
    timeout = 0xFFFF;
    while (timeout-- &&
           !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}
    I2C_SendData(I2C1, address);
    timeout = 0xFFFF;
    while (timeout-- &&
           !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}
    I2C_SendData(I2C1, byte);
    timeout = 0xFFFF;
    while (timeout-- &&
           !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}
    I2C_GenerateSTOP(I2C1, FunctionalState::ENABLE);
    //delay to write in eeprom
    for (volatile int i = 0; i < 144000 * 10; i++) {}
    /*
    volatile uint16_t timeout = 0xFFFF;
    I2C3->CR1 |= I2C_CR1_ACK;
    I2C3->CR1 |= I2C_CR1_START;
    // wait for start bit
    while (!(I2C3->SR1 & I2C_SR1_SB) && (timeout--)) {
    }
    (void)I2C3->SR1;       // read register to clear start flag
    I2C3->DR = slaveWrite; // send byte with device addres and last bit on WR
    timeout = 0xFFFF;
    while (!(I2C3->SR1 & I2C_SR1_ADDR) && (timeout--)) {
    }
    (void)I2C3->SR1;
    (void)I2C3->SR2;
    I2C3->DR = address; // send address register
    timeout = 0xFFFF;
    while (!(I2C3->SR1 & I2C_SR1_TXE) && (timeout--)) {
    }
    // I2C1->DR = address & 0xFF; //
    // timeout = 0xFFFF;
    // while (!(I2C1->SR1 & I2C_SR1_TXE) && (timeout--)) {
    // }
    I2C3->DR = byte;
    timeout = 0xFFFF;
    while (!(I2C3->SR1 & I2C_SR1_BTF) && (timeout--)) {
    }
    I2C3->CR1 |= I2C_CR1_STOP;
    // timeout = 0xFFFF;
    // while (!(I2C1->SR1 & I2C_SR1_STOPF) && (timeout--)) {
    // }
    HAL_Delay(100);
    */
}
uint8_t Eeprom::readByte(uint8_t address) {
    volatile uint16_t timeout = 0xFFFF;
    I2C_AcknowledgeConfig(I2C1, FunctionalState::ENABLE);
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET) {}
    I2C_GenerateSTART(I2C1, FunctionalState::ENABLE);
    while (timeout-- && !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}

    I2C_Send7bitAddress(I2C1, ADDRESS_W, I2C_Direction_Transmitter);
    timeout = 0xFFFF;
    while (timeout-- &&
           !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}
    // send address
    // I2C_SendData(I2C1, address >> 8);
    // timeout = 0xFFFF;
    // while (timeout-- && !I2C_CheckEvent(I2C1,
    // I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}
    I2C_SendData(I2C1, address);
    timeout = 0xFFFF;
    while (timeout-- &&
           !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}
    // restart
    I2C_GenerateSTART(I2C1, FunctionalState::ENABLE);
    timeout = 0xFFFF;
    while (timeout-- && !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}
    I2C_Send7bitAddress(I2C1, ADDRESS_R, I2C_Direction_Receiver);
    timeout = 0xFFFF;
    while (timeout-- &&
           !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {}
    timeout = 0xFFFF;
    while (!(I2C1->STAR1 & I2C_STAR1_RXNE) && timeout--) {
        I2C_AcknowledgeConfig(I2C1, DISABLE);
    }
    uint8_t data = I2C_ReceiveData(I2C1);
    I2C_GenerateSTOP(I2C1, FunctionalState::ENABLE);
    return data;
    /*
    uint8_t x = 0;
    volatile uint16_t timeout = 0xFFFF;
    I2C3->CR1 |= I2C_CR1_ACK;
    I2C3->CR1 |= I2C_CR1_START;
    while (!(I2C3->SR1 & I2C_SR1_SB)) {
    }
    //while (!(I2C3->SR1 & I2C_SR1_SB) && (timeout--)) {
    //}
    (void)I2C3->SR1;
    I2C3->DR = slaveWrite;
    timeout = 0xFFFF;
    while (!(I2C3->SR1 & I2C_SR1_ADDR) && (timeout--)) {
    }
    (void)I2C3->SR1;
    (void)I2C3->SR2;
    // I2C1->DR = address >> 8;
    // while (!(I2C1->SR1 & I2C_SR1_TXE && (timeout--))) {
    //}
    I2C3->DR = address & 0xFF;
    timeout = 0xFFFF;
    while (!(I2C3->SR1 & I2C_SR1_TXE) && (timeout--)) {
    }
    I2C3->CR1 |= I2C_CR1_START;
    timeout = 0xFFFF;
    while (!(I2C3->SR1 & I2C_SR1_SB) && (timeout--)) {
    }
    I2C3->DR = slaveRead;
    timeout = 0xFFFF;
    while (!(I2C3->SR1 & I2C_SR1_ADDR) && (timeout--)) {
    }
    (void)I2C3->SR1;
    (void)I2C3->SR2;
    I2C3->CR1 &= ~I2C_CR1_ACK; // no acknowlege!!!;
    timeout = 0xFFFF;
    while (!(I2C3->SR1 & I2C_SR1_RXNE) && (timeout--)) {
    }
    x = I2C3->DR;
    I2C3->CR1 |= I2C_CR1_STOP;
    HAL_Delay(10);
    return x;
    */
}
void Eeprom::init() {
    // i2C1 I-potenciometers PODKAL, FORSAGE, FreqI (100kHz)
    /*
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER |= GPIO_MODER_MODER8_1;
    GPIOB->MODER &= ~GPIO_MODER_MODER8_0; // 10: PB-6 alternate func
    GPIOB->MODER |= GPIO_MODER_MODER9_1;
    GPIOB->MODER &= ~GPIO_MODER_MODER9_0; // 10: PB-7 alternate func
    GPIOB->OTYPER |= GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9;
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR8_0 | GPIO_PUPDR_PUPDR8_1);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR9_0 | GPIO_PUPDR_PUPDR9_1);
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9; // 11:
    high speed GPIOB->AFR[1] |= (4 << 4) | (4 << 0); //

    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    I2C1->CR1 |= I2C_CR1_ACK; // ACK в ответе       генерировать условие ACK
    после приёма байтов I2C1->CR2 &= ~I2C_CR2_FREQ; // сбрасываем; I2C1->CR2 |=
    42;            //  Peripheral clock frequency - устанавливаем частоту
    // I2C1->CR2|=I2C_CR2_ITEVTEN; // включаем прерывания по событию
    // TPCLK1 = 24ns
    I2C1->CCR &= ~I2C_CCR_CCR;  //обнуляем CCR - значения контроля частоты
    I2C1->CCR &= ~I2C_CCR_FS;   // 0: Sm mode I2C     1: Fm mode I2C
    I2C1->CCR &= ~I2C_CCR_DUTY; // only for fast mode 0: Fm mode tlow/thigh = 2
    1: Fm mode tlow/thigh = 16/9 (see CCR)
    // SM: THIGH=CCR*TPCLK1=TLOW ::: THIGH+TLOW=10000ns =>
    //т.к. THIGH+TLOW=1/100kHz => THIGH+TLOW=10us=10000ns
    // CCR*(2*TPCLK1)=10000ns  => CCR=10000ns/(2*24ns) => CCR=208
    I2C1->CCR |= 208;
    // TRISE: Tr=1000ns (max rise time) => TRISE=(Tr/TPCLK1)+1=43;
    I2C1->TRISE = 41;        //максимальное время => заносим немного меньшее
    I2C1->CR1 |= I2C_CR1_PE; //включаем

    // I2C1->CR2 |= 24; //  Peripheral clock frequency
    //// TPCLK1 = 42ns
    // I2C1->CCR &= ~I2C_CCR_CCR;
    // I2C1->CCR &= ~I2C_CCR_FS;   // 0: Sm mode I2C     1: Fm mode I2C
    // I2C1->CCR &= ~I2C_CCR_DUTY; // only for fast mode 0: Fm mode tlow/thigh =
    2 1: Fm mode tlow/thigh = 16/9 (see
    // CCR)
    //// SM: THIGH=CCR*TPCLK1=TLOW ::: THIGH+TLOW=10000ns =>
    ////?.?. THIGH+TLOW=1/100kHz => THIGH+TLOW=10us=10000ns
    //// CCR*(2*TPCLK1)=10000ns  => CCR=10000ns/(2*42ns) =>
    //// CCR=120
    // I2C1->CCR |= 120;
    //// TRISE: Tr=1000ns (max rise time) => TRISE=(Tr/TPCLK1)+1=24;
    // I2C1->TRISE = 22;
    */

    // GPIO init pB6-SCL pB7-SDA 1:1:1:1 alt open_drain max speed
    RCC->APB2PCENR |= RCC_IOPBEN;
    GPIOB->CFGLR |= GPIO_CFGLR_MODE6 | GPIO_CFGLR_MODE7;
    GPIOB->CFGLR |= GPIO_CFGLR_CNF6 | GPIO_CFGLR_CNF7;

    RCC->APB1PCENR |= RCC_I2C1EN;
    I2C_InitTypeDef I2C_struct{.I2C_ClockSpeed = 100000,
                               .I2C_Mode = I2C_Mode_I2C,
                               .I2C_DutyCycle = I2C_DutyCycle_2,
                               .I2C_OwnAddress1 = 0,
                               .I2C_Ack = I2C_Ack_Enable,
                               .I2C_AcknowledgedAddress =
                                   I2C_AcknowledgedAddress_7bit};
    I2C_Init(I2C1, &I2C_struct);
    I2C_Cmd(I2C1, ENABLE);

    I2C_AcknowledgeConfig(I2C2, ENABLE);

    /*
    // I2C3 U-pot DOSE, FreqU + Eeprom (100kHz)
    // PA8-SCL
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER8_1;
    GPIOA->MODER &= ~GPIO_MODER_MODER8_0; // 10: PA-8 alternate func
    GPIOA->OTYPER |= GPIO_OTYPER_OT_8;
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR8_1;
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR8_0;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8; // 11: high speed
    GPIOA->AFR[1] |= (4 << 0);                //
    // GPIOA->MODER |= GPIO_MODER_MODER8_0;
    // GPIOA->MODER &= ~GPIO_MODER_MODER8_1; // 10: PA-8 alternate func
    // PC9-SDA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER |= GPIO_MODER_MODER9_1;
    GPIOC->MODER &= ~GPIO_MODER_MODER9_0; // 10: PC-9 alternate func
    GPIOC->OTYPER |= GPIO_OTYPER_OT_9;
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR9_0 | GPIO_PUPDR_PUPDR9_1);
    GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9; // 11: high speed
    GPIOC->AFR[1] |= (4 << 4);                //
    // GPIOC->MODER |= GPIO_MODER_MODER9_0;
    // GPIOC->MODER &= ~GPIO_MODER_MODER9_1; // 10: PC-9 alternate func

    RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
    I2C3->CR1 |= I2C_CR1_ACK; // ACK в ответе       генерировать условие ACK
    после приёма байтов I2C3->CR2 &= ~I2C_CR2_FREQ; // сбрасываем; I2C3->CR2 |=
    42;            //  Peripheral clock frequency - устанавливаем частоту
    // I2C1->CR2|=I2C_CR2_ITEVTEN; // включаем прерывания по событию
    // TPCLK1 = 24ns
    I2C3->CCR &= ~I2C_CCR_CCR;  //обнуляем CCR - значения контроля частоты
    I2C3->CCR &= ~I2C_CCR_FS;   // 0: Sm mode I2C     1: Fm mode I2C
    I2C3->CCR &= ~I2C_CCR_DUTY; // only for fast mode 0: Fm mode tlow/thigh = 2
    1: Fm mode tlow/thigh = 16/9 (see CCR)
    // SM: THIGH=CCR*TPCLK1=TLOW ::: THIGH+TLOW=10000ns =>
    //т.к. THIGH+TLOW=1/100kHz => THIGH+TLOW=10us=10000ns
    // CCR*(2*TPCLK1)=10000ns  => CCR=10000ns/(2*24ns) => CCR=208
    I2C3->CCR |= 208;
    // TRISE: Tr=1000ns (max rise time) => TRISE=(Tr/TPCLK1)+1=43;
    I2C3->TRISE = 41;        //максимальное время => заносим немного меньшее
    I2C3->CR1 |= I2C_CR1_PE; //включаем
    */
}
