#include "ds1822.h"
DS1822* DS1822::pThis = nullptr;

DS1822::DS1822(uint8_t num) {
    this->num = num;
    init(true);
}

void DS1822::up() {
    switch (num) {
    case 0:
        GPIOB->BSHR |= GPIO_BSHR_BS0;
        break;
    case 1:
        GPIOB->BSHR |= GPIO_BSHR_BS1;
        break;
    case 2:
        GPIOB->BSHR |= GPIO_BSHR_BS2;
        break;
    case 3:
        GPIOB->BSHR |= GPIO_BSHR_BS3;
        break;
    case 4:
        GPIOB->BSHR |= GPIO_BSHR_BS4;
        break;
    case 5:
        GPIOB->BSHR |= GPIO_BSHR_BS5;
        break;
    default:
        break;
    }
}
void DS1822::down() {
    switch (num) {
    case 0:
        GPIOB->BSHR |= GPIO_BSHR_BR0;
        break;
    case 1:
        GPIOB->BSHR |= GPIO_BSHR_BR1;
        break;
    case 2:
        GPIOB->BSHR |= GPIO_BSHR_BR2;
        break;
    case 3:
        GPIOB->BSHR |= GPIO_BSHR_BR3;
        break;
    case 4:
        GPIOB->BSHR |= GPIO_BSHR_BR4;
        break;
    case 5:
        GPIOB->BSHR |= GPIO_BSHR_BR5;
        break;
    default:
        break;
    }
}

bool DS1822::isUp() {
    bool isUp = false;
    switch (num) {
    case 0:
        isUp = GPIOB->INDR & GPIO_INDR_IDR0;
        break;
    case 1:
        isUp = GPIOB->INDR & GPIO_INDR_IDR1;
        break;
    case 2:
        isUp = GPIOB->INDR & GPIO_INDR_IDR2;
        break;
    case 3:
        isUp = GPIOB->INDR & GPIO_INDR_IDR3;
        break;
    case 4:
        isUp = GPIOB->INDR & GPIO_INDR_IDR4;
        break;
    case 5:
        isUp = GPIOB->INDR & GPIO_INDR_IDR5;
        break;
    default:
        break;
    }
    return isUp;
}

bool DS1822::init(bool mode) {
    RCC->APB2PCENR |= RCC_IOPBEN;
    switch (num) {
    case 0:
        // open drain
        GPIOB->CFGLR &= ~GPIO_CFGLR_CNF0;
        GPIOB->CFGLR |= GPIO_CFGLR_CNF0_0;
        GPIOB->CFGLR |= GPIO_CFGLR_MODE0;

        //GPIOB->CFGLR &= ~GPIO_CFGLR_CNF0;
        //GPIOB->CFGLR |= GPIO_CFGLR_MODE0;
        break;
    case 1:
        GPIOB->CFGLR &= ~GPIO_CFGLR_CNF1;
        GPIOB->CFGLR |= GPIO_CFGLR_CNF1_0;
        GPIOB->CFGLR |= GPIO_CFGLR_MODE1;
        break;
    case 2:
        GPIOB->CFGLR &= ~GPIO_CFGLR_CNF2;
        GPIOB->CFGLR |= GPIO_CFGLR_CNF2_0;
        GPIOB->CFGLR |= GPIO_CFGLR_MODE2;
        break;
    case 3:
        GPIOB->CFGLR &= ~GPIO_CFGLR_CNF3;
        GPIOB->CFGLR |= GPIO_CFGLR_CNF3_0;
        GPIOB->CFGLR |= GPIO_CFGLR_MODE3;
        break;
    case 4:
        GPIOB->CFGLR &= ~GPIO_CFGLR_CNF4;
        GPIOB->CFGLR |= GPIO_CFGLR_CNF4_0;
        GPIOB->CFGLR |= GPIO_CFGLR_MODE4;
        break;
    case 5:
        GPIOB->CFGLR &= ~GPIO_CFGLR_CNF5;
        GPIOB->CFGLR |= GPIO_CFGLR_CNF5_0;
        GPIOB->CFGLR |= GPIO_CFGLR_MODE5;
        break;
    default:
        break;
    }

    if (reset()) return 1;
    if (mode == SKIP_ROM) {
        writeByte(0xCC);
        writeByte(0x4E);
        writeByte(0x64);
        writeByte(0x9E);
        writeByte(RESOLUTION_12BIT);
    }
    return 0;
}

bool DS1822::reset() {
    down();
    delay_us(485);
    up();
    delay_us(65);
    bool status = isUp();
    delay_us(600);
    return status;
}
void DS1822::writeBit(uint8_t bit) {
    down();
    delay_us(bit ? 5 : 60);
    up();
    delay_us(bit ? 60 : 5);
    delay_us(1);
}
void DS1822::writeByte(uint8_t byte) {
    for (volatile int i = 0; i < 8; i++) {
        writeBit(byte >> i & 1);
        delay_us(5);
    }
}
bool DS1822::readBit() {
    down();
    delay_us(2);
    up();
    delay_us(20);
    bool bit = isUp();
    delay_us(43);
    return bit;
}
uint8_t DS1822::readByte() {
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++) {
        byte += readBit() << i;
        delay_us(1);
    }
    return byte;
}
void DS1822::measureTempCmd(uint8_t mode) {
    reset();
    if (mode == SKIP_ROM) { writeByte(0xCC); }
    writeByte(0x44);
}
void DS1822::readStrat(uint8_t mode, uint8_t* data) {
    reset();
    if (mode == SKIP_ROM) { writeByte(0xCC); }
    writeByte(0xBE);
    for (int i = 0; i < 9; i++) { data[i] = readByte(); }
}
float DS1822::convert(uint16_t data) {
    float t;
    t = (float)((data & 0x07FF) >> 4);
    t += (float)(data & 0x000F) / 16.0f;
    return t;
}
uint8_t DS1822::getSign(uint16_t data) {
    if (data & (1 << 11))
        return 1;
    else
        return 0;
}
void DS1822::measureT() {
    if (!firstMeasureCmdFlag) {
        firstMeasureCmdFlag = true;
    } else {
        measureTempCmd(SKIP_ROM);
        readStrat(SKIP_ROM, data);

        uint8_t crc = crc_dallas(data, 8);
        if (crc != data[8]) return;

        uint16_t raw_temper = ((uint16_t)data[1] << 8) | data[0];
        signMinus = getSign(raw_temper);
        temp = convert(raw_temper);
    }
}

void DS1822::delay_us(volatile uint16_t us) {
    volatile int cycles = us * 144 / 9; // for O0
    // volatile int cycles = us * 144/5; // for O1
    // volatile int cycles = us * 1440/65; // for Os
    while (cycles--) {}
}

uint8_t DS1822::crc_dallas(uint8_t* data, uint8_t len) {
    uint8_t crc = 0x0;
    while (len--) {
        uint8_t inbyte = *(data++);
        for (int i = 0; i < 8; i++) {
            unsigned char mix = (crc ^ inbyte) & 0x01;
            crc = crc >> 1;
            // 0x8C = 0b10001100  (reverse from x^8+x^5+x4+x^0)
            if (mix) crc = crc ^ 0x8C;
            inbyte = inbyte >> 1;
        }
    }
    return crc;
}
