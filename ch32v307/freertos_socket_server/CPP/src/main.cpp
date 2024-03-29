#include "main.h"

#include "adc.h"
#include "basic_timer.h"
#include "eeprom.h"
#include "eth.h"
#include "gpio.h"
//#include "gpios.h"
#include "http_os.h"
#include "rcc.h"
#include "tcp_os.h"
#include "udp_os.h"
#include "w25q.h"

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
Rcc rcc(24);
Adc adc1;
Eeprom eeprom;
W25q flash;
// ---------------- OS classes ------------------------------------------------
Eth eth(192, 168, 1, 100, TASK_RECEIVE_STK_SIZE);
UdpOS udp((uint8_t*)recv_buf);
TcpOS tcp(recv_buf);
//-----------------------------------------------------------------------------
// void timeout();

int main(void) {
    //Gpios::In<Gpios::PA, 2> in2;
    //in2.setMode(Gpios::ANALOG);
    //Gpios::init_in();
    // initialize mutex for proper socket work
    extern sys_mutex_t lock_tcpip_core;
    sys_mutex_new(&lock_tcpip_core);
    /* add queues, ... */
    queue1 = xQueueCreate(10, sizeof(uint32_t));

    FR_OS::startOS();
    //__enable_irq();
    // BasicTimer6::Instance().setCallback(timeout);
    // BasicTimer6::Instance().start(500);
    // Gpio::Out::init();
    while (1) {}
}

void timeout() {
    // callback function from hardware timer
}
