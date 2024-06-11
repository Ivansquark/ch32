#include "but.h"

Gpios::Out<Gpios::PC, 3> But::but1_out;
Gpios::In<Gpios::PC, 6, Gpios::PULL_DOWN> But::but1_in;
Gpios::Out<Gpios::PC, 2> But::but2_out;
Gpios::In<Gpios::PC, 7, Gpios::PULL_DOWN> But::but2_in;
bool But::IsBut1 = false;
bool But::IsBut2 = false;
uint32_t But::but1Counter = 0;
uint32_t But::but2Counter = 0;

But::But() {
    but1_out.setHigh();
    but2_out.setHigh();
}

void But::interruptHandler() {
    if (But::getBut1()) {
        if (But::but1Counter >= BOUNCE_TIME_MS) {
            But::but1Counter = 0;
            But::IsBut1 = true;
        } else {
            But::but1Counter++;
        }
    } else {
        But::IsBut1 = false;
        But::but1Counter = 0;
    }
    if (But::getBut2()) {
        if (But::but2Counter >= BOUNCE_TIME_MS) {
            But::but2Counter = 0;
            But::IsBut2 = true;
        } else {
            But::but2Counter++;
        }
    } else {
        But::IsBut2 = false;
        But::but2Counter = 0;
    }
}
