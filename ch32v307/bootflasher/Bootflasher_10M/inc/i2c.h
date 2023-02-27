#ifndef I2C_H_
#define I2C_H_

#include "ch32v30x_i2c.h"
#include "main.h"

static constexpr uint8_t InstructionByte = 0x00;
// AD5245 0 1 0 1 1 AD1 AD0 0(W)
static constexpr uint8_t AddrPotForsage_45 = 0x58; // (01011000)
static constexpr uint8_t AddrPotPodkal_45 = 0x5A;  // (01011010)

// AD5247_5 0 1 0 1 1 AD1 AD0 0(W)
static constexpr uint8_t AddrPotFreqU_47 = 0x5C; // (01011100)
// AD5248 0 1 0 1 1 AD1 AD0 0(W)
static constexpr uint8_t AddrPotDose_FreqI_48 = 0x58; // (01011000)
// static constexpr uint8_t AddrPotFreqU_48 = 0x5A; // (01011010)
static constexpr uint8_t AddrPotRes1 = 0x5C; // (01011100)
static constexpr uint8_t AddrPotRes2 = 0x5E; // (01011110)

class Pot1 {
  public:
    Pot1();
    void writePotFreqI(uint8_t FI) { writeByte(AddrPotDose_FreqI_48, FI); }
    void writePotPodkal(uint8_t podkal) { writeByte(AddrPotPodkal_45, podkal); }
    void writePotForsage(uint8_t fors) { writeByte(AddrPotForsage_45, fors); }

  private:
    void writeByte(uint8_t address, uint8_t byte);
};

class Pot3 {
  public:
    Pot3();
    void writePotDose(uint8_t dose) { writeByte(AddrPotDose_FreqI_48, dose); }
    void writePotFreqU(uint8_t FU) { writeByte(AddrPotFreqU_47, FU); }

  private:
    void writeByte(uint8_t address, uint8_t byte);
};

class Eeprom {
  public:
    Eeprom();
    static Eeprom *pThis;

    Pot1 pot1;
    Pot3 pot3;

    uint8_t U = 0;
    uint8_t I = 0;
    uint8_t FreqU = 0;
    uint8_t FreqI = 0;
    uint8_t Dose = 0;
    uint8_t Forsage = 0;
    uint8_t Podkal = 0;
    uint8_t Umax = 0;
    uint8_t Imax = 0;
    bool Mode = 0; // 0 - manual, 1 - auto
    uint8_t LastErr = 0;
    uint8_t ReadErr = 0;
    bool Is_T = false;
    uint8_t T_on = 0;
    uint8_t T_off = 0;

    void writeU(uint8_t kV) {
        U = kV;
        writeByte(USTADDRESS_KV, kV);
    }
    void writeI(uint8_t mA) {
        I = mA;
        writeByte(USTADDRESS_MA, mA);
    }
    void writeFreqU(uint8_t f) {
        FreqU = f;
        writeByte(USTADDRESS_FU, f);
        pot3.writePotFreqU(f);
    }
    void writeFreqI(uint8_t f) {
        FreqI = f;
        writeByte(USTADDRESS_FI, f);
        pot1.writePotFreqI(f);
    }
    void writeDose(uint8_t d) {
        Dose = d;
        writeByte(USTADDRESS_DOSE, d);
        pot3.writePotDose(d);
    }
    void writeFors(uint8_t f) {
        Forsage = f;
        writeByte(USTADDRESS_FORS, f);
        pot1.writePotForsage(f);
    }
    void writePodkal(uint8_t p) {
        Podkal = p;
        writeByte(USTADDRESS_PODKAL, p);
        pot1.writePotPodkal(p);
    }
    void writeUmax(uint8_t kV) {
        Umax = kV;
        writeByte(USTADDRESS_KVMAX, kV);
    }
    void writeImax(uint8_t mA) {
        Imax = mA;
        writeByte(USTADDRESS_MAMAX, mA);
    }
    void writeMode(uint8_t state) {
        Mode = state;
        writeByte(USTADDRESS_MODE, state);
    }
    void writeLastErr(uint8_t lastErrCode) {
        LastErr = lastErrCode;
        writeByte(USTADDRESS_ERR, lastErrCode);
    }
    void writeReadErr(uint8_t readErrCode) {
        ReadErr = readErrCode;
        writeByte(USTADDRESS_READERR, readErrCode);
    }
    void writeIs_T(uint8_t temp) {
        Is_T = temp;
        writeByte(USTADDRESS_T, temp);
    }
    void writeT_on(uint8_t temp) {
        T_on = temp;
        writeByte(USTADDRESS_TON, temp);
    }
    void writeT_off(uint8_t temp) {
        T_off = temp;
        writeByte(USTADDRESS_TOFF, temp);
    }

    uint8_t readU() {
        U = readByte(USTADDRESS_KV);
        return U;
    }
    uint8_t readI() {
        I = readByte(USTADDRESS_MA);
        return I;
    }
    uint8_t readFreqU() {
        FreqU = readByte(USTADDRESS_FU);
        return FreqU;
    }
    uint8_t readFreqI() {
        FreqI = readByte(USTADDRESS_FI);
        return FreqI;
    }
    uint8_t readDose() {
        Dose = readByte(USTADDRESS_DOSE);
        return Dose;
    }
    uint8_t readFors() {
        Forsage = readByte(USTADDRESS_FORS);
        return Forsage;
    }
    uint8_t readPodkal() {
        Podkal = readByte(USTADDRESS_PODKAL);
        return Podkal;
    }
    uint8_t readUmax() {
        Umax = readByte(USTADDRESS_KVMAX);
        return Umax;
    }
    uint8_t readImax() {
        Imax = readByte(USTADDRESS_MAMAX);
        return Imax;
    }
    uint8_t readMode() {
        Mode = readByte(USTADDRESS_MODE);
        return Mode;
    }
    uint8_t readLastErr() {
        LastErr = readByte(USTADDRESS_ERR);
        return LastErr;
    }
    uint8_t readReadErr() {
        ReadErr = readByte(USTADDRESS_READERR);
        return ReadErr;
    }
    uint8_t readIs_T() {
        Is_T = readByte(USTADDRESS_T);
        return Is_T;
    }
    uint8_t readT_on() {
        T_on = readByte(USTADDRESS_TON);
        return T_on;
    }
    uint8_t readT_off() {
        T_off = readByte(USTADDRESS_TOFF);
        return T_off;
    }

  private:
    volatile uint8_t arr[32];
    uint16_t arrSize = 0;
    //const uint8_t slaveWrite = 0xA0;
    //const uint8_t slaveRead = 0xA1;
    static constexpr uint8_t ADDRESS_R = 0xA1;
    static constexpr uint8_t ADDRESS_W = 0xA0;

    void writeByte(uint8_t address, uint8_t byte);
    uint8_t readByte(uint8_t address);

    void init();

    static constexpr uint8_t USTADDRESS_KV = 0x01;
    static constexpr uint8_t USTADDRESS_MA = 0x02;
    static constexpr uint8_t USTADDRESS_T = 0x03;      // use T Fan in TANK
    static constexpr uint8_t USTADDRESS_FU = 0x04;     // VFreqValue
    static constexpr uint8_t USTADDRESS_FI = 0x05;     // IFreqValue
    static constexpr uint8_t USTADDRESS_DOSE = 0x06;   // DoseValue
    static constexpr uint8_t USTADDRESS_FORS = 0x07;   // ForsajValue
    static constexpr uint8_t USTADDRESS_PODKAL = 0x08; // PodkalValue
    static constexpr uint8_t USTADDRESS_TON = 0x09;    // T_on_Value
    static constexpr uint8_t USTADDRESS_TOFF = 0x0A;   // T_off_Value
    static constexpr uint8_t USTADDRESS_KVMAX = 0x0B;
    static constexpr uint8_t USTADDRESS_MAMAX = 0x0C;
    static constexpr uint8_t USTADDRESS_MODE = 0x0D;    // Auto Manual
    static constexpr uint8_t USTADDRESS_ERR = 0x0E;     // Last Error
    static constexpr uint8_t USTADDRESS_READERR = 0x0F; // ReadError On/Off

    // static constexpr uint8_t RESOLUTION_9BIT = 0x1F;
    // static constexpr uint8_t RESOLUTION_10BIT = 0x3F;
    // static constexpr uint8_t RESOLUTION_11BIT = 0x5F;
    // static constexpr uint8_t RESOLUTION_12BIT = 0x7F;
    // static constexpr uint8_t SKIP_ROM = 0;
    // static constexpr uint8_t NOSKIP_ROM = 1;
};

#endif // I2C_H_
