#include "main.h"

#include "but.h"
#include "led.h"
#include "pwm.h"
#include "timer.h"
#include "tirq.h"

// #include <map>
// #include <string>
// #include <vector>
// void delay_ticks(volatile uint32_t ticks);

int main(void) {
    ObserverForInterrupt<TIM2_IRQn, But> observer_in_100us;
    RCC->CFGR0 &= ~RCC_HPRE; // SYS_CLK = 24MHz
    // Led led;
    But but;
    tim2_init();
    __enable_irq();
    //__disable_irq();
    //-------------------------------------------------------------------------
    // char* x = (char*)malloc(0x20);
    // char* x = new char[0x10];
    // std::map<const int, int> m{{1, 2}};

    // x[9] = 'c';
    // std::string str("aaa");
    // std::vector<int> vint{1, 2, 3};
    // if (vint[2] == 3 && str == "aaa") {
    // if (m.at(1) == 1) {
    //    led.on();
    //}
    //}
    //-------------------------------------------------------------------------
    enum DutyStates {
        DUTY50,
        DUTY70,
        DUTY100,
    };
    DutyStates curDutyState = DUTY50;
    enum Khz {
        Hz500,
        kHz1,
        kHz5,
        kHz10,
        kHz20,
        kHz40,
        kHz80
    };
    Khz curKhz = Hz500;

    pwm_init();
    pwm_ch1_start(50);
    pwm_ch1_set_khz(500);
    bool Is1 = false;
    bool Is2 = false;
    while (1) {
        if (!Is2) {
            if (But::IsBut2) {
                Is2 = true;
                switch (curKhz) {
                case Hz500:
                    pwm_ch1_set_khz(500);
                    pwm_ch1_stop();
                    pwm_ch1_start(50);
                    curKhz = kHz1;
                    break;
                case kHz1:
                    pwm_ch1_set_khz(1000);
                    pwm_ch1_stop();
                    pwm_ch1_start(50);
                    curKhz = kHz5;
                    break;
                case kHz5:
                    pwm_ch1_set_khz(5000);
                    pwm_ch1_stop();
                    pwm_ch1_start(50);
                    curKhz = kHz10;
                    break;
                case kHz10:
                    pwm_ch1_set_khz(10000);
                    pwm_ch1_stop();
                    pwm_ch1_start(50);
                    curKhz = kHz20;
                    break;
                case kHz20:
                    pwm_ch1_set_khz(20000);
                    pwm_ch1_stop();
                    pwm_ch1_start(50);
                    curKhz = kHz40;
                    break;
                case kHz40:
                    pwm_ch1_set_khz(40000);
                    pwm_ch1_stop();
                    pwm_ch1_start(50);
                    curKhz = kHz80;
                    break;
                case kHz80:
                    pwm_ch1_set_khz(80000);
                    pwm_ch1_stop();
                    pwm_ch1_start(50);
                    curKhz = Hz500;
                    break;
                default:
                    break;
                }
            }
        } else {
            if (!But::IsBut2) {
                Is2 = false;
            }
        }
        if (!Is1) {
            if (But::IsBut1) {
                Is1 = true;
                switch (curDutyState) {
                case DUTY50:
                    pwm_ch1_stop();
                    pwm_ch1_start(50);
                    curDutyState = DUTY70;
                    break;
                case DUTY70:
                    pwm_ch1_stop();
                    pwm_ch1_start(70);
                    curDutyState = DUTY100;
                    break;
                case DUTY100:
                    pwm_ch1_stop();
                    pwm_ch1_start(100);
                    curDutyState = DUTY50;
                    break;
                default:
                    break;
                }
            }
        } else {
            if (!But::IsBut1) {
                Is1 = false;
                // pwm_ch1_stop();
                // pwm_ch1_start(50);
            }
        }
    }
}

// void delay_ticks(volatile uint32_t ticks) {
//     while (ticks--) {}
// }
