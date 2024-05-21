#ifndef INPUTS_H
#define INPUTS_H

#include "gpios.h"

class Inputs {
  public:
    static void interruptHandler();

    static inline bool getButOff() { return IsButOff; }
    static inline bool getButOn() { return IsButOn; }
    static inline bool getUpKonc() { return IsUpKonc; }
    static inline bool getDownKonc() { return IsDownKonc; }
    static inline bool getClampUp() { return IsClampUp; }
    static inline bool getClampDown() { return IsClampDown; }
    static inline bool getCCKonc() { return IsCCKonc; }
    static inline bool getEin0() { return !ein0.getState(); }
    static inline bool getEin1() { return !ein1.getState(); }
    static inline bool getBookyOk() { return !bookyOk.getState(); }
    static inline bool getAl() { return !al.getState(); }
    static inline bool getCu() { return !cu.getState(); }
    static inline bool getTable() { return !table.getState(); }
    static inline bool getFbit1() { return !fbit1.getState(); }
    static inline bool getFbit2() { return !fbit2.getState(); }
    static inline bool getFbit3() { return !fbit3.getState(); }
    static inline bool getPrep() { return !prep.getState(); }
    static inline bool getReadyRpu() { return !ready_rpu.getState(); }
    static inline bool getExposure() { return !exposure.getState(); }
    static inline bool getHV_on() { return !hv_on.getState(); }
    static inline bool getXray_on() { return !xray_on.getState(); }
    static inline bool getHVC_fault() { return !hvc_fault.getState(); }
    static inline bool getExp() { return IsButExp; }
    static inline bool getReady() { return IsButReady; }
    static inline bool getAlarm() { return !alarm.getState(); }

  private:
    static Gpios::In<Gpios::PA, 12> butOff;
    static Gpios::In<Gpios::PA, 15> butOn;
    static Gpios::In<Gpios::PB, 11> up_konc;
    static Gpios::In<Gpios::PB, 12> down_konc;
    static Gpios::In<Gpios::PD, 14> clamp_up;
    static Gpios::In<Gpios::PD, 13> clamp_down;
    static Gpios::In<Gpios::PC, 5> cc_konc;
    static Gpios::In<Gpios::PE, 5> ein0;
    static Gpios::In<Gpios::PE, 6> ein1;
    static Gpios::In<Gpios::PA, 4> bookyOk;
    static Gpios::In<Gpios::PD, 10> al;
    static Gpios::In<Gpios::PD, 11> cu;
    static Gpios::In<Gpios::PD, 12> table;
    static Gpios::In<Gpios::PD, 6> fbit1;
    static Gpios::In<Gpios::PD, 7> fbit2;
    static Gpios::In<Gpios::PB, 3> fbit3;
    static Gpios::In<Gpios::PB, 5> prep;
    static Gpios::In<Gpios::PB, 9> ready_rpu;
    static Gpios::In<Gpios::PE, 1> exposure;
    static Gpios::In<Gpios::PE, 2> hv_on;
    static Gpios::In<Gpios::PE, 3> xray_on;
    static Gpios::In<Gpios::PD, 5> hvc_fault;
    static Gpios::In<Gpios::PC, 10> butExp;
    static Gpios::In<Gpios::PC, 11> butReady;
    static Gpios::In<Gpios::PC, 12> alarm;

    static inline bool isButOff() { return !butOff.getState(); }
    static inline bool isButOn() { return !butOn.getState(); }
    static inline bool isUpKonc() { return !up_konc.getState(); }
    static inline bool isDownKonc() { return !down_konc.getState(); }
    static inline bool isClampUp() { return !clamp_up.getState(); }
    static inline bool isClampDown() { return !clamp_down.getState(); }
    static inline bool isCCKonc() { return !cc_konc.getState(); }
    static inline bool isButExp() { return !butExp.getState(); }
    static inline bool isButReady() { return !butReady.getState(); }
    static bool IsButOn;
    static bool IsButOff;
    static bool IsUpKonc;
    static bool IsDownKonc;
    static bool IsClampUp;
    static bool IsClampDown;
    static bool IsClampUpKonc;
    static bool IsClampDownKonc;
    static bool IsCCKonc;
    static bool IsButExp;
    static bool IsButReady;

    static uint32_t butOnBounceCounter;
    static uint32_t butOffBounceCounter;
    static uint32_t upKoncBounceCounter;
    static uint32_t downKoncBounceCounter;
    static uint32_t clampUpBounceCounter;
    static uint32_t clampDownBounceCounter;
    static uint32_t ccKoncCounter;
    static uint32_t butExpBounceCounter;
    static uint32_t butReadyBounceCounter;
    static constexpr auto BOUNCE_TIME_MS = 100;
};

#endif // INPUTS_H
