#ifndef BUT_H
#define BUT_H

#include "gpios.h"

class But {
  public:
    But();
    static void interruptHandler();
    static inline bool getBut1() { return but1_in.getState(); }
    static inline bool getBut2() { return but1_in.getState(); }


    static bool IsBut1;
    static bool IsBut2;
    static uint32_t but1Counter;
    static uint32_t but2Counter;
    static constexpr auto BOUNCE_TIME_MS = 100;

    static Gpios::Out<Gpios::PC, 3> but1_out;
    static Gpios::In<Gpios::PC, 6, Gpios::PULL_DOWN> but1_in;
    static Gpios::Out<Gpios::PC, 2> but2_out;
    static Gpios::In<Gpios::PC, 7, Gpios::PULL_DOWN> but2_in;
};

#endif // BUT_H
