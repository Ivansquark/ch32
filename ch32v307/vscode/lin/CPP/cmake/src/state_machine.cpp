#include "state_machine.h"

StateMachine* StateMachine::pThis = nullptr;

StateMachine::StateMachine() {
    pThis = this;
    tcp.create_server();
}

void StateMachine::handler() {
    ethHandler();
    switch (currentState) {
    case States::OFF:
        buttonsHandler();
        break;
    case States::TURN_ON:
        turnHandler();
        break;
    case States::ON:
        buttonsHandler();
        break;
    case States::TURN_OFF:
        turnHandler();
        break;
    default:
        break;
    }
}

void StateMachine::ethHandler() {
    if (tcp.getServerIsReceived()) {
        //parse
        tcp.setServerIsReceived(false);
    }
}

void StateMachine::turnHandler() {}
void StateMachine::buttonsHandler() {
    if (Inputs::getButOn()) {
        if (!IsPowerOn) {
            currentState = States::TURN_ON;
            IsPowerOn = true;
        }
    }
    if (Inputs::getButOff()) {
        if (IsPowerOn) {
            currentState = States::TURN_OFF;
            IsPowerOn = false;
        }
    }
}

//---------------------- timers -----------------------------------------------
void StateMachine::timerReplyStart(uint32_t ms) {
    timerReplyCounterMax = ms;
    timerReplyCounter = 0;
    TimerReplyStart = true;
}
void StateMachine::timerReplyStop() {
    timerReplyCounter = 0;
    TimerReplyStart = false;
}

void StateMachine::timerReplyTimeout() {
    IsTimeout = true;
    timerReplyStop();
}
void StateMachine::timerTurnStart(uint32_t ms) {
    timerTurnCounterMax = ms;
    timerTurnCounter = 0;
    TimerTurnStart = true;
}
void StateMachine::timerTurnStop() {
    timerTurnCounter = 0;
    TimerTurnStart = false;
}

void StateMachine::timerTurnTimeout() {
    IsTurnTimeout = true;
    timerTurnStop();
}

void StateMachine::interruptHandler() {
    if (StateMachine::pThis->TimerReplyStart) {
        if (StateMachine::pThis->timerReplyCounter >=
            StateMachine::pThis->timerReplyCounterMax) {
            StateMachine::pThis->timerReplyTimeout();
            StateMachine::pThis->timerReplyCounter = 0;
            StateMachine::pThis->TimerReplyStart = false;
        } else {
            StateMachine::pThis->timerReplyCounter++;
        }
    }
    if (StateMachine::pThis->TimerTurnStart) {
        if (StateMachine::pThis->timerTurnCounter >=
            StateMachine::pThis->timerTurnCounterMax) {
            StateMachine::pThis->timerTurnTimeout();
            StateMachine::pThis->timerTurnCounter = 0;
            StateMachine::pThis->TimerTurnStart = false;
        } else {
            StateMachine::pThis->timerTurnCounter++;
        }
    }
}
