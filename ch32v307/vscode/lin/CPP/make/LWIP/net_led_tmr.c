#include "net_led_tmr.h"

uint8_t net_data_led_require = 0;

static uint8_t net_data_led = 0;

void net_led_tmr(void) {
    if (net_data_led) {
        net_data_led = 0;
        // LED_DATASET(1); /* turn off data led. */
    } else {
        if (net_data_led_require != 0) {
            net_data_led = 1;
            // LED_DATASET(0); /* turn on data led. */
            net_data_led_require = 0;
        }
    }
}
