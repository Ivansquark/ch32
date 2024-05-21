#include "buzzer.h"

Buzzer* Buzzer::pThis = nullptr;

Buzzer::Buzzer() {
    pThis = this;
    init();
}

void Buzzer::start() {
    bzz.setHigh();
}

void Buzzer::stop() {
    bzz.setLow();
}

void Buzzer::handler() {
    if(MustBuzz) {
        start();
        //TODO: set toggle by counter
    } else {
        stop();
    }
}

void Buzzer::init() {
    //if not generator play pwm music
}
