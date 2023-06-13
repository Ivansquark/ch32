#include "main.h"
#include "dac.h"
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
        // setRed();
        GPIOA->OUTDR |= (GPIO_OUTDR_ODR4 | GPIO_OUTDR_ODR5);
        GPIOA->OUTDR |= (GPIO_OUTDR_ODR9 | GPIO_OUTDR_ODR10 | GPIO_OUTDR_ODR11 |
                         GPIO_OUTDR_ODR12);
        GPIOB->OUTDR |= (GPIO_OUTDR_ODR12 | GPIO_OUTDR_ODR13 |
                         GPIO_OUTDR_ODR14 | GPIO_OUTDR_ODR15);
        GPIOD->OUTDR |=
            (GPIO_OUTDR_ODR8 | GPIO_OUTDR_ODR9 | GPIO_OUTDR_ODR10 |
             GPIO_OUTDR_ODR11 | GPIO_OUTDR_ODR12 | GPIO_OUTDR_ODR13 |
             GPIO_OUTDR_ODR14 | GPIO_OUTDR_ODR15);
        vTaskDelay(1000);

        // resetRed();
        GPIOA->OUTDR &= ~(GPIO_OUTDR_ODR4 | GPIO_OUTDR_ODR5);
        GPIOA->OUTDR &= ~(GPIO_OUTDR_ODR9 | GPIO_OUTDR_ODR10 |
                          GPIO_OUTDR_ODR11 | GPIO_OUTDR_ODR12);
        GPIOB->OUTDR &= ~(GPIO_OUTDR_ODR12 | GPIO_OUTDR_ODR13 |
                          GPIO_OUTDR_ODR14 | GPIO_OUTDR_ODR15);
        GPIOD->OUTDR &=
            ~(GPIO_OUTDR_ODR8 | GPIO_OUTDR_ODR9 | GPIO_OUTDR_ODR10 |
              GPIO_OUTDR_ODR11 | GPIO_OUTDR_ODR12 | GPIO_OUTDR_ODR13 |
              GPIO_OUTDR_ODR14 | GPIO_OUTDR_ODR15);
        vTaskDelay(1000);
    }
}

void task2_task(__attribute__((unused)) void* pvParameters)
{
    // UNUSED(pvParameters);
    //dac_init();
    //uint16_t x = 0;
    while (1) {
        //if (x < 0xFFF) {
            //dac1_set(x++);
            //dac2_set(x++);
        //} else {
            //x = 0;
        //}
        vTaskDelay(5);
    }
}

void delay(volatile uint32_t val);

int main(void)
{
    Rcc_init(8);
    gpio_init();
    dac_init();
    dac1_set(0x7ff);
    dac2_set(0x7ff);
    // spi2_init();
    // while(1) {
    //    spi2_readWriteByte(0xAA);
    //}

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
