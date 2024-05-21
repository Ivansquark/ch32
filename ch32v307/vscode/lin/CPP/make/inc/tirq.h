#ifndef TIRQ_H
#define TIRQ_H

#include "main.h"
// static observer
extern void (*vectors[])();

class InterruptManager {
  public:
    InterruptManager() {
        // set default handler to DefaultHandler
    }
    // set callback
    static void addHandler(void (*fPtr)(), IRQn irq) { arrIrq[irq] = fPtr; }
    static void removeHandler(IRQn_Type irq) { arrIrq[irq] = defaultHandler; }
    static void call(IRQn irq) { arrIrq[irq](); }

  private:
    // TODO: try to set struct to Vector
    // array of function pointers
    // static void (*ptr[104])(void);
    static void (*arrIrq[104])(void);
    static void defaultHandler(void) { asm volatile("nop" ::); }
};

template <IRQn_Type irq, typename... Subscribed>
class ObserverForInterrupt {
  public:
    ObserverForInterrupt() {
        InterruptManager::addHandler(onInterruptHandler, irq);
    }

  private:
    static void onInterruptHandler() {
        // folding expressions unpacked
        // operator comma: check first and execute second
        (Subscribed::interruptHandler(), ...);
    }
};

#endif // TIRQ_H
