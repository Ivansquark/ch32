#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "inputs.h"
#include "my_tcp.h"
#include "outputs.h"
#include "proto.h"

class StateMachine {
  public:
    StateMachine();
    static StateMachine* pThis;
    enum class States {
        OFF,
        TURN_ON,
        ON,
        TURN_OFF
    };
    States currentState = States::OFF;
    void handler();
    //------------- interrupt -------------------------------------------------
    static void interruptHandler();
    //------------- timers ----------------------------------------------------
    bool TimerReplyStart = false;
    uint32_t timerReplyCounter = 0;
    uint32_t timerReplyCounterMax = 0;
    void timerReplyStart(uint32_t ms);
    void timerReplyStop();
    void timerReplyTimeout();
    bool IsTimeout = false;

    bool TimerTurnStart = false;
    uint32_t timerTurnCounter = 0;
    uint32_t timerTurnCounterMax = 0;
    void timerTurnStart(uint32_t ms);
    void timerTurnStop();
    void timerTurnTimeout();
    bool IsTurnTimeout = false;

  private:
    uint8_t tcpBuf[1024] = {0};
    Tcp tcp = {nullptr, tcpBuf};
    void ethHandler();
    void turnHandler();
    void buttonsHandler();
    bool IsPowerOn = false;
};

#endif // STATE_MACHINE_H
