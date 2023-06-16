#include "irq.h"

void InterruptManager::defaultHandler(void) {
    asm volatile("nop" ::);
    //while (1) { asm volatile("nop" ::); }
}

void InterruptManager::addHandler(void (*fPtr)(), IRQn_Type irq) {
    arrInterrupts[irq] = fPtr;
}

void InterruptManager::removeHandler(IRQn_Type irq) {
    arrInterrupts[irq] = defaultHandler;
}
void InterruptManager::call(IRQn_Type irq) { arrInterrupts[irq](); }

void (*InterruptManager::arrInterrupts[82])(void) = {
    &InterruptManager::defaultHandler};
