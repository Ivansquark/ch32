#include "main.h"
#include "gpio.h"
#include "rcc.h"
#include "spi.h"
#include "systim.h"

#include "FreeRTOS.h"
#include "task.h"

/* Global define */
#define TASK1_TASK_PRIO 5
#define TASK1_STK_SIZE 256
#define TASK2_TASK_PRIO 5
#define TASK2_STK_SIZE 256

/* Global Variable */
TaskHandle_t Task1Task_Handler;
TaskHandle_t Task2Task_Handler;

void task1_task(void* pvParameters)
{
    UNUSED(pvParameters);
    while (1) {
        setRed();
        vTaskDelay(250);
        resetRed();
        vTaskDelay(250);
    }
}

void task2_task(__attribute__((unused)) void* pvParameters)
{
    // UNUSED(pvParameters);
    while (1) {
        setBlue();
        vTaskDelay(500);
        resetBlue();
        vTaskDelay(500);
    }
}

void delay(volatile uint32_t val);

int main(void)
{
    Rcc_init(8);
    gpio_init();
    
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

    while (1) {}
}

void delay(volatile uint32_t val)
{
    while (val--) {}
}

__attribute__((interrupt)) void SysTick_IRQHandler(void) {}
