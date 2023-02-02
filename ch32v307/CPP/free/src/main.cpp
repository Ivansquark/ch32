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
#include "systim.h"
#include "w25q.h"

#include "FreeRTOS.h"
#include "task.h"

Rcc rcc(8);

Adc adc1;
Eeprom eeprom;
// W25q flash;
// Eth eth(192, 168, 0, 100);
Eth eth(192, 168, 1, 100);
Udp udp;
Http http;
// Sd sd;
void timeout();
bool IsTimeout;

constexpr uint16_t SIZE = 256 * 12;
uint8_t tempArr[SIZE];

void delay(volatile uint32_t val);

/* Global define */
#define TASK1_TASK_PRIO 5
#define TASK1_STK_SIZE 256
#define TASK2_TASK_PRIO 5
#define TASK2_STK_SIZE 256

/* Global Variable */
TaskHandle_t Task1Task_Handler;
TaskHandle_t Task2Task_Handler;

void task1_task(void* pvParameters) {
    Gpio::Out::init();
    while (1) {

        Gpio::Out::setRed();
        vTaskDelay(250);
        Gpio::Out::resetRed();
        vTaskDelay(250);
    }
}

void task2_task(__attribute__((unused)) void* pvParameters) {
    Gpio::Out::init();
    while (1) {
        Gpio::Out::setBlue();
        vTaskDelay(500);
        Gpio::Out::resetBlue();
        vTaskDelay(500);
    }
}

int main(void) {

    /* create two task */
    xTaskCreate((TaskFunction_t)task2_task, (const char*)"task2",
                (uint16_t)TASK2_STK_SIZE, (void*)NULL,
                (UBaseType_t)TASK2_TASK_PRIO,
                (TaskHandle_t*)&Task2Task_Handler);

    xTaskCreate((TaskFunction_t)task1_task, (const char*)"task1",
                (uint16_t)TASK1_STK_SIZE, (void*)NULL,
                (UBaseType_t)TASK1_TASK_PRIO,
                (TaskHandle_t*)&Task1Task_Handler);
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
        eth.rx_handler();
        // TCP handler
        http.httpHandler();

        if (IsTimeout) {
            IsTimeout = false;
            if (!timeoutSate) {
                timeoutSate = true;
                Gpio::Out::setRed();
                // Gpio::Out::resetBlue();
            } else {
                timeoutSate = false;
                Gpio::Out::resetRed();
                // Gpio::Out::setBlue();
            }
        }
        if (udp.SendFlag) {
            static uint8_t add;
            add += 1;
            static uint16_t lineCounter;

            for (uint32_t i = 0; i < sizeof(tempArr); i++) {
                tempArr[i] = i + add;
            }
            Udp::Header head;
            head.PackFlag = 0xFFFF;
            head.pixel.dataLen = 0x100;
            head.pixel.dataNum = 0x0;
            head.line.dataLen = 0x000;
            head.line.dataNum = 0x0;

            uint16_t MAX_SIZE = 0x5ba;
            uint8_t count = sizeof(tempArr) / MAX_SIZE; // packet nums
            uint16_t l = 0;
            head.pixel.dataNum = count;
            for (int i = 0; i < count + 1; i++) {
                if (lineCounter >= 0xFFF) {
                    lineCounter = 0;
                } else {
                    lineCounter++;
                }
                l = sizeof(tempArr) - (MAX_SIZE * i);
                if (l >= MAX_SIZE) {
                    head.pixel.dataLen = MAX_SIZE;
                    head.pixel.dataNum = count;
                    head.line.dataLen = lineCounter;
                    head.line.dataNum = i;
                    uint8_t temp = *(uint8_t*)&head.pixel;
                    *(uint8_t*)&head.pixel = *((uint8_t*)(&head.pixel) + 1);
                    *((uint8_t*)(&head.pixel) + 1) = temp;
                    temp = *(uint8_t*)&head.line;
                    *(uint8_t*)&head.line = *((uint8_t*)(&head.line) + 1);
                    *((uint8_t*)(&head.line) + 1) = temp;
                    memcpy(Udp::pThis->udp_msg_send, &head,
                           sizeof(Udp::Header));
                    memcpy((uint8_t*)Udp::pThis->udp_msg_send +
                               sizeof(Udp::Header),
                           (uint8_t*)tempArr + i * (MAX_SIZE), MAX_SIZE);
                    uint16_t len = sizeof(Udp::Header) + MAX_SIZE;
                    udp.sendToPC(udp.udp_msg_send, len);
                } else {
                    // last packet
                    head.pixel.dataLen = l;
                    head.pixel.dataNum = count;
                    head.line.dataLen = lineCounter;
                    head.line.dataNum = i;
                    uint8_t temp = *(uint8_t*)&head.pixel;
                    *(uint8_t*)&head.pixel = *((uint8_t*)(&head.pixel) + 1);
                    *((uint8_t*)(&head.pixel) + 1) = temp;
                    temp = *(uint8_t*)&head.line;
                    *(uint8_t*)&head.line = *((uint8_t*)(&head.line) + 1);
                    *((uint8_t*)(&head.line) + 1) = temp;
                    memcpy(Udp::pThis->udp_msg_send, &head,
                           sizeof(Udp::Header));
                    memcpy((uint8_t*)Udp::pThis->udp_msg_send +
                               sizeof(Udp::Header),
                           (uint8_t*)tempArr + i * (MAX_SIZE), l);
                    uint16_t len = sizeof(Udp::Header) + l;
                    udp.sendToPC(udp.udp_msg_send, len);
                }
            }
        }
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
