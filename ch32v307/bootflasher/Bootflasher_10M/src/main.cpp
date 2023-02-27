#include "main.h"

#include "adc.h"
#include "basic_timers.h"
#include "dac.h"
#include "eth.h"
#include "gptimers.h"
#include "i2c.h"
#include "my_tcp.h"
#include "my_udp.h"
#include "rcc.h"

/* Global Variable */
uint32_t SystemCoreClock = 144000000;
char* queue_buf;
//------------- objects in static memory (.data section) ----------------------
Rcc rcc(24);
Flash flash;
Eth eth(192, 168, 1, 5);
Udp udp(192, 168, 1, 5);
Tcp tcp;
//Eeprom eeprom;
Uart uart;
Adc adc1;
Dac dac;
FrequencyMeter freqMeter;
//-----------------------------------------------------------------------------

int main(void) {
    //------------ Interrupts objects -----------------
    //InterruptManagerForObjects<TIM3_IRQn> timer_ms;
    //timer_ms.setVector();
    //GpTimer tim3(3);

    //__enable_irq();
    //-------------------- XRay -----------------------------
    while (1) {
        eth.rx_handler();

        if (GpTimer::timFlag_100us) {
            GpTimer::timFlag_100us = false;
            if (Adc::pThis->isConversionCompleteted1()) {
                ADC1->STATR = 0;
                Adc::pThis->adcHandler1();
            }
            Adc::pThis->startConversion1();
            if (Adc::pThis->isConversionCompleteted2()) {
                ADC2->STATR = 0;
                Adc::pThis->adcHandler2();
            }
            Adc::pThis->startConversion2();

        }
        //------------- Temp TANK - ds1822 ----------------------
        if (GpTimer::timFlag_1s) {
            GpTimer::timFlag_1s = false;
            __disable_irq();
            __enable_irq();
            // TODO:send to pc
        }
        //------------- Firmware update --------------------------
        if (FirmwareUpdate::getFirmwareUpdateFlag()) {
            FirmwareUpdate::firmwareUpdate_init();
            tcp.create_server();
            bool Error_or_Ok = false;
            while (true) {
                ethernetif_input(&eth.WCH_NetIf);
                sys_check_timeouts();
                Error_or_Ok = FirmwareUpdate::firmwareUpdateHandler();
                //if (Error_or_Ok == true) {
                //    FirmwareUpdate::setFirmwareUpdateFlag(false);
                //    break;
                //}
            }
        }
    }
}

