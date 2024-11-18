#include "main.h"

#include "but.h"
#include "dynamic_queue.h"
#include "global_constants.h"
#include "led.h"
#include "pwm.h"
#include "queue.h"
#include "timer.h"
// #include "tirq.h"
#include "trcc.h"
#include "tuart.h"
#include "write.h"

#include "my_malloc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <charconv>
#include <iomanip>
#include <memory>
#include <queue>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
// void delay_ticks(volatile uint32_t ticks);
//
// extern "C" void _exit(){}
extern void* _heap_start;
extern void* _heap_end;
static uint8_t* _cur_brk = (uint8_t*)&_heap_start;
void printRegDebug();

TRcc::RccSys<TRcc::HSI, Glob::SysClk> rcc;

using Ua1 = TUart::Uart<Gpios::PD, 6, Gpios::PD, 5, 115200>;
Ua1 uart1;


Queue::QueueReceive que;

struct Map {
    std::string first;
    std::string second;
};

int main(void) {
    volatile uint32_t arr[2] = {0, 0x1FF};
    *((uint32_t*)0x20000100) = *((uint8_t*)arr + 4);
    // ObserverForInterrupt<TIM2_IRQn, But> observer_in_100us;
    // Led led;
    // But but;
    // tim2_init();
    __enable_irq();
    //__disable_irq();
    // std::queue<std::string> q; // too much heap

    // std::string hello = "text to be parsed";
    // auto split =
    //     hello | std::ranges::views::split(' ') |
    //     std::ranges::views::transform([](auto&& str) {
    //         return std::string_view(&*str.begin(),
    //         std::ranges::distance(str));
    //     });
    // for (auto&& word : split) {
    //     printStr(word);
    // }

    // constexpr std::string_view words{"Hello^_^C++^_^20^_^!"};
    // constexpr std::string_view delim{"^_^"};
    // std::vector<std::string_view> result;
    // for (auto&& word : std::ranges::views::split(words, delim)) {
    //     result.emplace_back((word));
    //     printStr(std::string_view(word));
    // }
    //  printStr(result[0]);
    //
    // constexpr std::string_view text =
    //    "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";
    //// for (auto&& word : text | std::ranges::views::split(' ')) {
    // for (auto&& word : std::ranges::views::split(text, ' ')) {
    //     printStr(std::string_view(word));
    // }

    printf("\r\n------------------------------------------------------\r\n");
    printf("Hello Riscv\r\n");
    printf("------------------------------------------------------\r\n");

    std::vector<std::string> result{""};
    DQueue<std::string_view> q;
    while (1) {
        if (TUart::IsReceived) {
            TUart::IsReceived = false;
            std::string rx = "";
            rx.append(TUart::data, strlen(TUart::data));
            std::string parse = rx.substr(0, 3);
            // print("RECEIVED\r\n");
            if (parse == "REB") {
                //______________ Software RESET _______________________
                NVIC_SystemReset();
            } else if (parse == "DEB") {
                printRegDebug();
                printf("\r\n");
                int* x = new (std::nothrow) int;
                *x = 6;
            } else if (parse == "PUS") {
                //-------------- C++23 - version ------------------------------
                constexpr std::string_view delim{" "};
                for (auto&& word : std::ranges::views::split(rx, delim)) {
                    result.emplace_back(std::string_view(word));
                    // result.push_back(std::string(std::string_view(word)));
                    que.push(word.data(), word.size());
                    q.push(std::string_view(word));

                    printStr(std::string_view(word));
                }
                for (auto i : result) {
                    printStr(i);
                    // q.emplace(i);
                }
                //-------------- C - version ----------------------------------
                // char buf[64] = {0};
                // memcpy(buf, rx.data(), rx.size());
                // char* strPtr = nullptr;

                // char buf1[4][16] = {{0}, {0}};
                // int c = 0;
                // strPtr = strtok(buf, " ");
                // memcpy(buf1 + c * 16, buf, strPtr - buf);
                // c = 1;
                // while (strPtr) {
                //     memcpy(buf1 + c * 16, strPtr, strPtr - buf);
                //     strPtr = strtok(nullptr, " ");
                //     c++;
                // }
                // for (int i = 0; i < 4; i++) {
                //     printStr((char*)(buf1 + i * 16));
                // }
                //-------------------------------------------------------------

                // int count = 0;
                // int find = 0;
                //  std::vector<std::string> result;
                //   for (auto&& i : rx) {
                //       if (i == ' ') {
                //           result.emplace_back(
                //               std::move(rx.substr(find, count - find)));
                //           find = count + 1;
                //       }
                //       count++;
                //   }
                //   result.emplace_back(std::move(rx.substr(find, count -
                //   find)));
                //  for (auto it = rx.begin(); it != rx.end(); ++it) {
                //      if (*it == ' ') {
                //          result.emplace_back(
                //              std::move(rx.substr(find, count - find)));
                //          find = count + 1;
                //      }
                //      count++;
                //  }
                //  result.emplace_back(std::move(rx.substr(find, count -
                //  find)));

                // for (auto&& i : result) {
                //     printStr(i);
                // }
            } else if (parse == "PUL") {
                Queue::QueueReceive::Data data = que.pop();
                do {
                    if (data.data) {
                        printStr((const char*)data.data);
                    }
                    data = que.pop();
                } while (data.data != nullptr);
                for (auto i : result) {
                    printStr(i);
                }
                while (!q.isEmpty()) {
                    printStr(q.pop());
                }
            }
        }
    }
}
extern "C" void* _sbrk_r(struct _reent* reent, ptrdiff_t diff) {
    // const char* arr = "sbrk-";
    printVal((char*)"_sbrk_diff = ", diff);
    printAddr((char*)"_sbrk_old_brk = 0x", (unsigned long)_cur_brk);
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
    printAddr((char*)"_sbrk_cur_brk = 0x", (unsigned long)_cur_brk);
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
