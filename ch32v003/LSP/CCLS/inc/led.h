#ifndef LED_H
#define LED_H

#include "gpios.h"

class Led {
public:
    static inline void on() {pin.setHigh();}
    static inline void off() {pin.setLow();}
private:

static Gpios::Out<Gpios::PC, 4> pin;
};


#endif //LED_H
