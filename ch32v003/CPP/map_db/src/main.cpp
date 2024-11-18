#include "main.h"

#include "but.h"
#include "led.h"
#include "pwm.h"
#include "timer.h"
// #include "tirq.h"
#include "uart.h"
#include "write.h"

#include "my_malloc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <charconv>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
// void delay_ticks(volatile uint32_t ticks);
extern void* _heap_start;
extern void* _heap_end;
static uint8_t* _cur_brk = (uint8_t*)&_heap_start;
void printRegDebug();

Uart uart;

int main(void) {
    // ObserverForInterrupt<TIM2_IRQn, But> observer_in_100us;
    RCC->CFGR0 &= ~RCC_HPRE; // SYS_CLK = 24MHz
    // Led led;
    // But but;
    // tim2_init();
    __enable_irq();
    //__disable_irq();
    std::map<std::string, std::string>* db =
       new std::map<std::string, std::string>{{"URAL", "VAGONOV"},
                                              {"KAMAZ", "POMOEV"}};
    
    // unordered_map not fit in memory
    //std::unordered_map<int, std::string>* db =
    //    new std::unordered_map<int, std::string>{{1, "VAG"}};
    printf("%s", db[0]);

    // printf("\r\n------------------------------------------------------\r\n");
    // printf("Hello Riscv\r\n");
    // printf("------------------------------------------------------\r\n");

    while (1) {
        /*
        if (Uart::IsReceived) {
            Uart::IsReceived = false;
            std::string rx = "";
            rx.append(Uart::data, strlen(Uart::data));
            std::string parse = rx.substr(0, 3);
            // print("RECEIVED\r\n");
            if (parse == "REB") {
                //______________ Software RESET _______________________
                NVIC_SystemReset();
            } else if (parse == "DEB") {
                printRegDebug();
                printf("\r\n");
            } else if (parse == "MAP") {
                printStr("\r\n");
                for (auto&& i : *db) {
                    printStr(i.first + " " + i.second);
                }
            } else if (parse == "GET") {
                printStr("\r\n");
                std::string str = rx.substr(3);
                bool has = false;
                if (str.size()) {
                    str = str.substr(1);
                }
                printStr(str);
                for (auto&& i : *db) {
                    if (i.first == str) {
                        has = true;
                        printStr(i.first + " " + i.second +
                                 " THAT IS C++ BABY");
                    }
                }
                if (!has) {
                    printStr("NO SUCH NAME");
                }
            } else if (parse == "ADD") {
                //std::vector<std::string> vStr;
                //int count = 0;
                //int find = 0;
                //for (auto it = rx.begin(); it != rx.end(); ++it) {
                //    if (*it == ' ') {
                //        vStr.emplace_back(
                //            std::move(rx.substr(find, count - find)));
                //        find = count + 1;
                //    }
                //    count++;
                //}
                //vStr.emplace_back(std::move(rx.substr(find, count - find)));
                //for (auto&& i : vStr) {
                //    printStr(i);
                //}
                //if (vStr.size() > 2) {
                //    db->insert({vStr[1], vStr[2]});
                //}
            } else if (parse == "DEL") {
                std::string str = rx.substr(3);
                if (str.size()) {
                    str = str.substr(1);
                }
                printStr(str);
                for (auto&& i : *db) {
                    if (i.first == str) {
                        db->erase(str);
                    }
                }
            } else if (parse == "INT") {
                //printStr(rx);
                //std::string sub = rx.substr(3);
                //printStr(sub);
                //int x = std::stoi(std::move(sub));
                //printf("NUM + 1 = %d\r\n", ++x);
                //std::string* opa = new std::string("OPA");
                //std::string* jopa = new std::string("J");
                //std::string res = *jopa + *opa;
                //printStr(res.c_str());
                //delete opa;
                //delete jopa;
            }
        }
    */
    }
}
extern "C" void* _sbrk_r(struct _reent* reent, ptrdiff_t diff) {
    // const char* arr = "sbrk-";
    printVal((char*)"_sbrk_diff = ", diff);
    printAddr((char*)"_sbrk_old_brk = 0x", (uint32_t)_cur_brk);
    uint8_t* _old_brk;
    _old_brk = _cur_brk;
    if ((uint32_t*)(_cur_brk + diff) > (uint32_t*)&_heap_end) {
        reent->_errno = ENOMEM;
        print((char*)"ENOMEM\r\n");
        // end of heap // if was new => bad_alloc
        // while (1) {}
        return (void*)-1;
    }
    _cur_brk += diff;
    printAddr((char*)"_sbrk_cur_brk = 0x", (uint32_t)_cur_brk);
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

    int* m = (int*)malloc(1 * sizeof(int));
    printf("malloc_ptr = 0x%x\r\n", (unsigned)m);
    free(m);
}
