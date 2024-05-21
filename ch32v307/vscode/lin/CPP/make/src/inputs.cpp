#include "inputs.h"
Gpios::In<Gpios::PA, 12> Inputs::butOff;
Gpios::In<Gpios::PA, 15> Inputs::butOn;
Gpios::In<Gpios::PB, 11> Inputs::up_konc;
Gpios::In<Gpios::PB, 12> Inputs::down_konc;
Gpios::In<Gpios::PD, 14> Inputs::clamp_up;
Gpios::In<Gpios::PD, 13> Inputs::clamp_down;
Gpios::In<Gpios::PC, 5> Inputs::cc_konc;
Gpios::In<Gpios::PE, 5> Inputs::ein0;
Gpios::In<Gpios::PE, 6> Inputs::ein1;
Gpios::In<Gpios::PA, 4> Inputs::bookyOk;
Gpios::In<Gpios::PD, 10> Inputs::al;
Gpios::In<Gpios::PD, 11> Inputs::cu;
Gpios::In<Gpios::PD, 12> Inputs::table;
Gpios::In<Gpios::PD, 6> Inputs::fbit1;
Gpios::In<Gpios::PD, 7> Inputs::fbit2;
Gpios::In<Gpios::PB, 3> Inputs::fbit3;
Gpios::In<Gpios::PB, 5> Inputs::prep;
Gpios::In<Gpios::PB, 9> Inputs::ready_rpu;
Gpios::In<Gpios::PE, 1> Inputs::exposure;
Gpios::In<Gpios::PE, 2> Inputs::hv_on;
Gpios::In<Gpios::PE, 3> Inputs::xray_on;
Gpios::In<Gpios::PD, 5> Inputs::hvc_fault;
Gpios::In<Gpios::PC, 10> Inputs::butExp;
Gpios::In<Gpios::PC, 11> Inputs::butReady;
Gpios::In<Gpios::PC, 12> Inputs::alarm;

bool Inputs::IsButOn = false;
bool Inputs::IsButOff = false;
bool Inputs::IsUpKonc = false;
bool Inputs::IsDownKonc = false;
bool Inputs::IsClampUp = false;
bool Inputs::IsClampDown = false;
bool Inputs::IsCCKonc = false;
bool Inputs::IsButExp = false;
bool Inputs::IsButReady = false;

uint32_t Inputs::butOnBounceCounter = 0;
uint32_t Inputs::butOffBounceCounter = 0;
uint32_t Inputs::upKoncBounceCounter = 0;
uint32_t Inputs::downKoncBounceCounter = 0;
uint32_t Inputs::clampUpBounceCounter = 0;
uint32_t Inputs::clampDownBounceCounter = 0;
uint32_t Inputs::ccKoncCounter = 0;
uint32_t Inputs::butExpBounceCounter = 0;
uint32_t Inputs::butReadyBounceCounter = 0;

// ms timer interrupt
void Inputs::interruptHandler() {
    if (Inputs::isButOn()) {
        if (Inputs::butOnBounceCounter >= BOUNCE_TIME_MS) {
            Inputs::butOnBounceCounter = 0;
            Inputs::IsButOn = true;
        } else {
            Inputs::butOnBounceCounter++;
        }
    } else {
        Inputs::IsButOn = false;
        Inputs::butOnBounceCounter = 0;
    }
    if (Inputs::isButOff()) {
        if (Inputs::butOffBounceCounter >= BOUNCE_TIME_MS) {
            Inputs::butOffBounceCounter = 0;
            Inputs::IsButOff = true;
        } else {
            Inputs::butOffBounceCounter++;
        }
    } else {
        Inputs::IsButOff = false;
        Inputs::butOffBounceCounter = 0;
    }
    if (Inputs::isUpKonc()) {
        if (Inputs::upKoncBounceCounter >= BOUNCE_TIME_MS) {
            Inputs::upKoncBounceCounter = 0;
            Inputs::IsUpKonc = true;
        } else {
            Inputs::upKoncBounceCounter++;
        }
    } else {
        Inputs::IsUpKonc = false;
        Inputs::upKoncBounceCounter = 0;
    }
    if (Inputs::isDownKonc()) {
        if (Inputs::downKoncBounceCounter >= BOUNCE_TIME_MS) {
            Inputs::downKoncBounceCounter = 0;
            Inputs::IsDownKonc = true;
        } else {
            Inputs::downKoncBounceCounter++;
        }
    } else {
        Inputs::IsDownKonc = false;
        Inputs::downKoncBounceCounter = 0;
    }
    if (Inputs::isClampUp()) {
        if (Inputs::clampUpBounceCounter >= BOUNCE_TIME_MS) {
            Inputs::clampUpBounceCounter = 0;
            Inputs::IsClampUp = true;
        } else {
            Inputs::clampUpBounceCounter++;
        }
    } else {
        Inputs::IsClampUp = false;
        Inputs::clampUpBounceCounter = 0;
    }
    if (Inputs::isClampDown()) {
        if (Inputs::clampDownBounceCounter >= BOUNCE_TIME_MS) {
            Inputs::clampDownBounceCounter = 0;
            Inputs::IsClampDown = true;
        } else {
            Inputs::clampDownBounceCounter++;
        }
    } else {
        Inputs::IsClampDown = false;
        Inputs::clampDownBounceCounter = 0;
    }
    if (Inputs::isCCKonc()) {
        if (Inputs::ccKoncCounter >= BOUNCE_TIME_MS) {
            Inputs::ccKoncCounter = 0;
            Inputs::IsCCKonc = true;
        } else {
            Inputs::ccKoncCounter++;
        }
    } else {
        Inputs::IsCCKonc = false;
        Inputs::ccKoncCounter = 0;
    }
    if (Inputs::isButExp()) {
        if (Inputs::butExpBounceCounter >= BOUNCE_TIME_MS) {
            Inputs::butExpBounceCounter = 0;
            Inputs::IsButExp = true;
        } else {
            Inputs::butExpBounceCounter++;
        }
    } else {
        Inputs::IsButExp = false;
        Inputs::butExpBounceCounter = 0;
    }
    if (Inputs::isButReady()) {
        if (Inputs::butReadyBounceCounter >= BOUNCE_TIME_MS) {
            Inputs::butReadyBounceCounter = 0;
            Inputs::IsButReady = true;
        } else {
            Inputs::butReadyBounceCounter++;
        }
    } else {
        Inputs::IsButReady = false;
        Inputs::butReadyBounceCounter = 0;
    }
}
