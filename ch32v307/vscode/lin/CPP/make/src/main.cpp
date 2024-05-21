#include "main.h"

#include "basic_timer.h"
#include "buzzer.h"
#include "dma.h"
#include "eeprom.h"
#include "eth.h"
#include "led.h"
#include "my_tcp.h"
#include "my_udp.h"
#include "rcc.h"
#include "state_machine.h"
#include "tirq.h"

// #include <stdlib.h>

/* Global Variable */
uint32_t SystemCoreClock = 144000000;
//------------- objects in static memory (.data section) ----------------------
Inputs inputs;
Outputs outputs;
Rcc rcc(24);

Eeprom eeprom;
Eth eth(192, 168, 1, 5, 24);
Dma2 dma;
StateMachine stateMachine;
Buzzer buzz;
//-----------------------------------------------------------------------------

int main(void) {
    // eeprom.writeLiftPosition(0xffff);
    ObserverForInterrupt<TIM6_IRQn, StateMachine, Inputs> observer_in_1ms;
    ObserverForInterrupt<TIM5_IRQn> observer_in_100us;
    BasicTimer6::Instance().start(1);
    BasicTimer5::Instance().start(1);

    while (1) {
        eth.rx_handler();
        stateMachine.handler();
    }
}
