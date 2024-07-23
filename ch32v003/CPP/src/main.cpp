#include "main.h"

#include "but.h"
#include "led.h"
#include "pwm.h"
#include "timer.h"
#include "tirq.h"

#include "stdio.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <map>
// #include <string>
// #include <vector>
// void delay_ticks(volatile uint32_t ticks);
extern void* _heap_start;
extern void* _heap_end;
static uint8_t* _cur_brk = (uint8_t*)&_heap_start;
void printRegDebug();

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
    char* x = new char[0x10];
    std::map<const int, int> m{{1, 2}};
    x[0] = m[1];
    std::pair<const int, int> pa{1, 2};
    pa.second = 7;

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
void* _sbrk_r(struct _reent* reent, ptrdiff_t diff) {
    // const char* arr = "sbrk-";
    char addr[11] = {0};
    snprintf(addr, 10, "%x", (unsigned)_cur_brk);
    strcat(addr, "\n");
    uint8_t* _old_brk;
    _old_brk = _cur_brk;
    if ((uint32_t*)(_cur_brk + diff) > (uint32_t*)&_heap_end) {
        reent->_errno = ENOMEM;
        // end of heap
        while (1) {}
        return (void*)-1;
    }
    _cur_brk += diff;
    return _old_brk;
}

void printRegDebug() {
    // void* p = NULL; //get sp
    //------- get registers ---------------------
    uint32_t x1 = 1;
    asm volatile("mv %0, x1" : "=r"(x1));
    uint32_t x2 = 2;
    asm volatile("mv %0, x2" : "=r"(x2));
    uint32_t x3 = 3;
    asm volatile("mv %0, x3" : "=r"(x3));
    uint32_t x4 = 4;
    asm volatile("mv %0, x4" : "=r"(x4));
    uint32_t x5 = 5;
    asm volatile("mv %0, x5" : "=r"(x5));
    uint32_t x6 = 6;
    asm volatile("mv %0, x6" : "=r"(x6));
    uint32_t x7 = 7;
    asm volatile("mv %0, x7" : "=r"(x7));
    uint32_t x8 = 8;
    asm volatile("mv %0, x8" : "=r"(x8));
    uint32_t x9 = 9;
    asm volatile("mv %0, x9" : "=r"(x9));
    const char* command = "values";
    printf("\r\n\r\n");
    printf("-----------------------------------------------------\r\n");
    printf("cur_heap_pos = %x\r\n", (unsigned)_cur_brk);
    // printf("cur_heap_pos = %x\r\n", cur_heap_pos);
    printf("-----------------------------------------------------");
    printf("\r\n"
           "Registers (%s):\r\n"
           "ra =  0x%x\r\n"
           "sp =  0x%x\r\n"
           "gp =  0x%x\r\n"
           "tp =  0x%x\r\n"
           "t0 =  0x%x\r\n"
           "t1 =  0x%x\r\n"
           "t2 =  0x%x\r\n"
           "s0 =  0x%x\r\n"
           "s1 =  0x%x\r\n",
           command, (unsigned)x1, (unsigned)x2, (unsigned)x3, (unsigned)x4,
           (unsigned)x5, (unsigned)x6, (unsigned)x7, (unsigned)x8,
           (unsigned)x9);

    int* m = (int*)malloc(100 * sizeof(int));
    printf("malloc_ptr = %x\r\n", (unsigned)m);
    free(m);

    // int* new_int_ptr = new int[0x10];
    // for (volatile int i = 0; i < 0x10; i++) {
    //     new_int_ptr[i] = i;
    // }
    // printf("new_int_ptr[5] = %d\r\n", new_int_ptr[5]);
    // printf("new_int_ptr = %x\r\n", new_int_ptr);
    //  delete[] new_int_ptr;
    //   Buzzer* buzz1 = new Buzzer;
    //-------------------------------------------
    // printf("sp =  (%p):\r\n", (void*)&p);
}
// void delay_ticks(volatile uint32_t ticks) {
//     while (ticks--) {}
// }
