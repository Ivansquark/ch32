#include "main.h"

#include "adc.h"
#include "basic_timer.h"
#include "eeprom.h"
#include "eth.h"
#include "gpio.h"
#include "my_http.h"
#include "my_tcp.h"
#include "my_udp.h"
#include "rcc.h"
#include "sd.h"
#include "w25q.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

uint32_t SystemCoreClock = 144000000;

Rcc rcc(8);

Adc adc1;
Eeprom eeprom;
W25q flash;
// Eth eth(192, 168, 0, 100);
Eth eth(192, 168, 1, 100);
// Udp udp;
// Http http;
// Sd sd;
void timeout();
bool IsTimeout;

constexpr uint16_t SIZE = 256 * 12;
uint8_t tempArr[SIZE];

void delay(volatile uint32_t val);

/* Global define */
#define TASK_HTTP_TASK_PRIO 1
#define TASK_HTTP_STK_SIZE 2048
#define TASK_RECEIVE_TASK_PRIO 5
#define TASK_RECEIVE_STK_SIZE 512
#define RECV_BUF_SIZE 2048
char recv_buf[RECV_BUF_SIZE] = {0};

/* Global Variable */
TaskHandle_t TaskHttp_Handler;
TaskHandle_t TaskReceive_Handler;
TaskHandle_t TaskUdp_Handler;
xQueueHandle queue1;
char* queue_buf;

void receive_task([[maybe_unused]] void* pvParameters) {
    Gpio::Out::init();
    while (1) {
        eth.rx_handler();
        vTaskDelay(1); // every ms
    }
}

void http_task(__attribute__((unused)) void* pvParameters) {
    Gpio::Out::init();
    while (1) {
        //__disable_irq();
        Gpio::Out::toggleBlue();
        // http.httpHandler();
        //__enable_irq();
        vTaskDelay(1);
    }
}
extern sys_mutex_t lock_tcpip_core;
void udp_task([[maybe_unused]] void* pvParameters) {
    lwip_socket_thread_init();
    sys_mutex_new(&lock_tcpip_core);
    // uint32_t mutex = 1;
    // lock_tcpip_core.mut = &mutex;
    int sock, new_sd;
    struct sockaddr_in address, dest_addr;
    int size;

    int n, nwrote;
    sock = lwip_socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) { return; }
    address.sin_family = AF_INET;
    address.sin_port = htons(55555);
    address.sin_addr.s_addr = INADDR_ANY;
    int err = bind(sock, (struct sockaddr*)&address, sizeof(address));
    while (1) {
        // n = read(sock, recv_buf, RECV_BUF_SIZE);
        n = recvfrom(sock, (void*)recv_buf, 256, 0, (struct sockaddr*)&address,
                     (socklen_t*)&size);
        // lwip_recvfrom(int s, void *mem, size_t len, int flags,
        //      struct sockaddr *from, socklen_t *fromlen)
        if (n < 0) {
            vTaskDelete(NULL); // Should delete this connection handler
            // error
        } else if (n > 0) {
            Gpio::Out::toggleBlue();
            // read complete
            recv_buf[n] = ' ';
            recv_buf[n] = 'o';
            recv_buf[n] = 'k';
            write(sock, recv_buf, n + 3);
        }
        vTaskDelay(1); // every ms
    }
}

int main(void) {
    /* create two task */
    xTaskCreate((TaskFunction_t)receive_task, (const char*)"task2",
                (uint16_t)TASK_RECEIVE_STK_SIZE, (void*)NULL,
                (UBaseType_t)TASK_RECEIVE_TASK_PRIO,
                (TaskHandle_t*)&TaskReceive_Handler);

    // xTaskCreate((TaskFunction_t)http_task, (const char*)"task1",
    //            (uint16_t)TASK_HTTP_STK_SIZE, (void*)NULL,
    //            (UBaseType_t)TASK_RECEIVE_TASK_PRIO,
    //            (TaskHandle_t*)&TaskHttp_Handler);

    xTaskCreate((TaskFunction_t)udp_task, (const char*)"udp_task",
                (uint16_t)TASK_HTTP_STK_SIZE, (void*)NULL,
                (UBaseType_t)TASK_RECEIVE_TASK_PRIO,
                (TaskHandle_t*)&TaskUdp_Handler);

    /* add queues, ... */
    queue1 = xQueueCreate(10, sizeof(uint32_t));

    vTaskStartScheduler();

    // flash.reset();
    // delay(1000000);
    // flash.WAKEUP();
    // delay(10000);
    // uint16_t id = 0;
    // do {
    //    id = 0;
    //    //id = flash.readSR();
    //    id = flash.readID();
    //}
    // while (id != flash.W25Q32);
    // const char* str = "Hello wq25 flash";
    // uint8_t tempArr[20] = {0};
    // flash.write((uint8_t*)str, 0, 15);
    // flash.erase_Sector(0);
    // flash.write(tempArr, 0, sizeof(tempArr));
    // flash.read(tempArr, 0x0, sizeof(tempArr));
    // SysTim::init(144000000); // 1 s
    __enable_irq();
    BasicTimer6::Instance().setCallback(timeout);
    BasicTimer6::Instance().start(500);
    IsTimeout = false;
    bool timeoutSate = false;
    Gpio::Out::init();

    while (1) {

        // if (IsTimeout) {
        //    IsTimeout = false;
        //    if (!timeoutSate) {
        //        timeoutSate = true;
        //        Gpio::Out::setRed();
        //        // Gpio::Out::resetBlue();
        //    } else {
        //        timeoutSate = false;
        //        Gpio::Out::resetRed();
        //        // Gpio::Out::setBlue();
        //    }
        //}
        // if (udp.SendFlag) {
        //    static uint8_t add;
        //    add += 1;
        //    static uint16_t lineCounter;

        //    for (uint32_t i = 0; i < sizeof(tempArr); i++) {
        //        tempArr[i] = i + add;
        //    }
        //    Udp::Header head;
        //    head.PackFlag = 0xFFFF;
        //    head.pixel.dataLen = 0x100;
        //    head.pixel.dataNum = 0x0;
        //    head.line.dataLen = 0x000;
        //    head.line.dataNum = 0x0;

        //    uint16_t MAX_SIZE = 0x5ba;
        //    uint8_t count = sizeof(tempArr) / MAX_SIZE; // packet nums
        //    uint16_t l = 0;
        //    head.pixel.dataNum = count;
        //    for (int i = 0; i < count + 1; i++) {
        //        if (lineCounter >= 0xFFF) {
        //            lineCounter = 0;
        //        } else {
        //            lineCounter++;
        //        }
        //        l = sizeof(tempArr) - (MAX_SIZE * i);
        //        if (l >= MAX_SIZE) {
        //            head.pixel.dataLen = MAX_SIZE;
        //            head.pixel.dataNum = count;
        //            head.line.dataLen = lineCounter;
        //            head.line.dataNum = i;
        //            uint8_t temp = *(uint8_t*)&head.pixel;
        //            *(uint8_t*)&head.pixel = *((uint8_t*)(&head.pixel) + 1);
        //            *((uint8_t*)(&head.pixel) + 1) = temp;
        //            temp = *(uint8_t*)&head.line;
        //            *(uint8_t*)&head.line = *((uint8_t*)(&head.line) + 1);
        //            *((uint8_t*)(&head.line) + 1) = temp;
        //            memcpy(Udp::pThis->udp_msg_send, &head,
        //                   sizeof(Udp::Header));
        //            memcpy((uint8_t*)Udp::pThis->udp_msg_send +
        //                       sizeof(Udp::Header),
        //                   (uint8_t*)tempArr + i * (MAX_SIZE), MAX_SIZE);
        //            uint16_t len = sizeof(Udp::Header) + MAX_SIZE;
        //            udp.sendToPC(udp.udp_msg_send, len);
        //        } else {
        //            // last packet
        //            head.pixel.dataLen = l;
        //            head.pixel.dataNum = count;
        //            head.line.dataLen = lineCounter;
        //            head.line.dataNum = i;
        //            uint8_t temp = *(uint8_t*)&head.pixel;
        //            *(uint8_t*)&head.pixel = *((uint8_t*)(&head.pixel) + 1);
        //            *((uint8_t*)(&head.pixel) + 1) = temp;
        //            temp = *(uint8_t*)&head.line;
        //            *(uint8_t*)&head.line = *((uint8_t*)(&head.line) + 1);
        //            *((uint8_t*)(&head.line) + 1) = temp;
        //            memcpy(Udp::pThis->udp_msg_send, &head,
        //                   sizeof(Udp::Header));
        //            memcpy((uint8_t*)Udp::pThis->udp_msg_send +
        //                       sizeof(Udp::Header),
        //                   (uint8_t*)tempArr + i * (MAX_SIZE), l);
        //            uint16_t len = sizeof(Udp::Header) + l;
        //            udp.sendToPC(udp.udp_msg_send, len);
        //        }
        //    }
        //}
        // if (SysTim::getIsTimeout()) {
        //    SysTim::setIsTimeout(false);
        //    if (!timeoutSate) {
        //        timeoutSate = true;
        //        Gpio::Out::setRed();
        //        Gpio::Out::resetBlue();
        //    } else {
        //        timeoutSate = false;
        //        Gpio::Out::resetRed();
        //        Gpio::Out::setBlue();
        //    }
        //}
    }
}

void timeout() { IsTimeout = true; }

void delay(volatile uint32_t val) {
    while (val--) {}
}
