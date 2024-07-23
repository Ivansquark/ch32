#ifndef TIRQ_H
#define TIRQ_H

#include "main.h"


//! static observer for interrupts handling 
class InterruptManager {
  public:
    // set callback
    static void addHandler(void (*fPtr)(), IRQn irq) { arrIrq[irq] = fPtr; }
    static void removeHandler(IRQn_Type irq) { arrIrq[irq] = defaultHandler; }
    static void call(IRQn irq) { arrIrq[irq](); }

  private:
    // array of function pointers
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
        // operator comma: check and execute first then second then ...
        (Subscribed::interruptHandler(), ...);
    }
};

#endif // TIRQ_H
