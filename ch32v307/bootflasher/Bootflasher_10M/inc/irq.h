#ifndef IRQ_H
#define IRQ_H

#include "main.h"

class InterruptManager {
    public:
        static void addHandler(void (*fPtr)(void), IRQn_Type irq);
        static void removeHandler(IRQn_Type irq);
        static void defaultHandler();
        static void call(IRQn_Type irq);
        static void (*arrInterrupts[82])(void);
};
class Interruptable{
    public:
        Interruptable(){}
        virtual ~Interruptable(){}
        virtual void interruptHandler() = 0;
};

template<IRQn_Type irq>
class InterruptManagerForObjects {
    public:
        void setVector() {
            InterruptManager::addHandler(interruptsHandler,irq);
        }
        static void addClass(Interruptable* obj) {
            for(int i = 0; i< SIZE_OF_ARR_OF_CLASSES; i++) {
                if(arrayOfInterruptClasses[i] == nullptr) {
                    arrayOfInterruptClasses[i] = obj;
                    return;
                }
            }
        }
        static void interruptsHandler() {
            for(int i = 0; i< SIZE_OF_ARR_OF_CLASSES; i++) {
                if(arrayOfInterruptClasses[i] == nullptr) {
                    return;
                } else {
                    arrayOfInterruptClasses[i]->interruptHandler();
                }
            }
        }
        static constexpr uint8_t SIZE_OF_ARR_OF_CLASSES = 2;
        static Interruptable* arrayOfInterruptClasses[SIZE_OF_ARR_OF_CLASSES];
};
template<IRQn_Type irq>
Interruptable* InterruptManagerForObjects<irq>::arrayOfInterruptClasses[SIZE_OF_ARR_OF_CLASSES] = {nullptr};
#endif //IRQ_H
