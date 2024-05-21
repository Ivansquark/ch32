#ifndef NET_LED_TMR
#define NET_LED_TMR

#include <stdint.h>

#define NET_LED_PERIOD_MSECS 100

extern uint8_t net_data_led_require;
void net_led_tmr(void);

#endif //NET_LED_TMR
