#include "main.h"

#include "but.h"
#include "led.h"
#include "pwm.h"
#include "tirq.h"
#include "timer.h"

#include <map>
#include <string>
#include <vector>
void delay_ticks(volatile uint32_t ticks);

int main(void) {
    ObserverForInterrupt<TIM2_IRQn, But> observer_in_100us;
    RCC->CFGR0 &= ~RCC_HPRE; // SYS_CLK = 24MHz
    Led led;
    But but;
    tim2_init();
    __enable_irq();
    //__disable_irq();
    //-------------------------------------------------------------------------
    // char* x = (char*)malloc(0x20);
    char* x = new char[0x10];
    std::map<const int, int> m{{1, 2}};

    // x[9] = 'c';
    // std::string str("aaa");
    // std::vector<int> vint{1, 2, 3};
    // if (vint[2] == 3 && str == "aaa") {
    if (m.at(1) == 1) {
        led.on();
    }
    //}
    //-------------------------------------------------------------------------
    enum DutyStates {
        DUTY20,
        DUTY40,
        DUTY60,
        DUTY80,
        DUTY100,
    };
    DutyStates curDutyState = DUTY20;
    enum Khz {
        Khz2,
        Khz5,
        Khz10,
        Khz20,
        Khz30,
        Khz40,
        Khz50,
        Khz60
    };
    Khz curKhz = Khz5;

    pwm_init();
    pwm_ch1_start(20);
    pwm_ch1_set_khz(2000);
    bool Is1 = false;
    bool Is2 = false;
    while (1) {
        if (!Is2) {
            if (But::IsBut2) {
                Is2 = true;
                switch (curKhz) {
                case Khz2:
                    pwm_ch1_set_khz(2000);
                    pwm_ch1_stop();
                    pwm_ch1_start(60);
                    curKhz = Khz10;
                    break;
                case Khz5:
                    pwm_ch1_set_khz(5000);
                    pwm_ch1_stop();
                    pwm_ch1_start(60);
                    curKhz = Khz10;
                    break;
                case Khz10:
                    pwm_ch1_set_khz(10000);
                    pwm_ch1_stop();
                    pwm_ch1_start(60);
                    curKhz = Khz20;
                    break;
                case Khz20:
                    pwm_ch1_set_khz(20000);
                    pwm_ch1_stop();
                    pwm_ch1_start(60);
                    curKhz = Khz30;
                    break;
                case Khz30:
                    pwm_ch1_set_khz(30000);
                    pwm_ch1_stop();
                    pwm_ch1_start(60);
                    curKhz = Khz40;
                    break;
                case Khz40:
                    pwm_ch1_set_khz(40000);
                    pwm_ch1_stop();
                    pwm_ch1_start(60);
                    curKhz = Khz50;
                    break;
                case Khz50:
                    pwm_ch1_set_khz(50000);
                    pwm_ch1_stop();
                    pwm_ch1_start(60);
                    curKhz = Khz60;
                    break;
                case Khz60:
                    pwm_ch1_set_khz(60000);
                    pwm_ch1_stop();
                    pwm_ch1_start(60);
                    curKhz = Khz5;
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
                case DUTY20:
                    pwm_ch1_stop();
                    pwm_ch1_start(20);
                    curDutyState = DUTY40;
                    break;
                case DUTY40:
                    pwm_ch1_stop();
                    pwm_ch1_start(40);
                    curDutyState = DUTY60;
                    break;
                case DUTY60:
                    pwm_ch1_stop();
                    pwm_ch1_start(60);
                    curDutyState = DUTY80;
                    break;
                case DUTY80:
                    pwm_ch1_stop();
                    pwm_ch1_start(80);
                    curDutyState = DUTY100;
                    break;
                case DUTY100:
                    pwm_ch1_stop();
                    pwm_ch1_start(100);
                    curDutyState = DUTY20;
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

void delay_ticks(volatile uint32_t ticks) {
    while (ticks--) {}
}
