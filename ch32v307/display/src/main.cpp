#include "main.h"

#include "basic_timer.h"
#include "buttons.h"
#include "lcdpar.h"
#include "gpio.h"
#include "rcc.h"

#include "frwrapper.h"
/* Global define */
#define TASK_RECEIVE_TASK_PRIO 5
#define TASK_RECEIVE_STK_SIZE 512
#define RECV_BUF_SIZE 2048
char recv_buf[RECV_BUF_SIZE] = {0};

/* Global Variable */
uint32_t SystemCoreClock = 144000000;
xQueueHandle queue1;
char* queue_buf;
//------------- objects in static memory (.data section) ----------------------
Rcc rcc(8);
LcdParIni parDisp;
// ---------------- OS classes ------------------------------------------------
Buttons but;
//-----------------------------------------------------------------------------

int main(void) {
    /* add queues, ... */
    queue1 = xQueueCreate(10, sizeof(uint32_t));

    FR_OS::startOS();
    //__enable_irq();
    // BasicTimer6::Instance().setCallback(timeout);
    // BasicTimer6::Instance().start(500);
    // Gpio::Out::init();
    while (1) {}
}
