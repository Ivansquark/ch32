#include "main.h"
#include "rcc.h"
#include "systim.h"

#include "FreeRTOS.h"
#include "task.h"

/* Global define */
#define TASK1_TASK_PRIO     5
#define TASK1_STK_SIZE      256
#define TASK2_TASK_PRIO     5
#define TASK2_STK_SIZE      256

/* Global Variable */
TaskHandle_t Task1Task_Handler;
TaskHandle_t Task2Task_Handler;

void task1_task(void *pvParameters)
{
    while(1)
    {
        vTaskDelay(250);
        vTaskDelay(250);
    }
}

void task2_task(void *pvParameters)
{
    while(1)
    {
        vTaskDelay(500);
        vTaskDelay(500);
    }
}

void delay(volatile uint32_t val);

int main(void) {
    Rcc_init(8);

    uint8_t i = 0;
    int y = 0;
    // NVIC_PriorityGroupConfig(NVIC_PriorityGrou:wqp_2);
    // GPIO_Toggle_INIT();
    RCC->APB2PCENR |= RCC_IOPAEN;
    GPIOA->CFGHR |= GPIO_CFGHR_MODE15;
    GPIOA->CFGHR &= ~GPIO_CFGHR_CNF15;
    RCC->APB2PCENR |= RCC_IOPBEN;
    GPIOB->CFGLR |= GPIO_CFGLR_MODE4;
    GPIOB->CFGLR &= ~GPIO_CFGLR_CNF4;

    /* create two task */
    xTaskCreate((TaskFunction_t )task2_task,
                        (const char*    )"task2",
                        (uint16_t       )TASK2_STK_SIZE,
                        (void*          )NULL,
                        (UBaseType_t    )TASK2_TASK_PRIO,
                        (TaskHandle_t*  )&Task2Task_Handler);

    xTaskCreate((TaskFunction_t )task1_task,
                    (const char*    )"task1",
                    (uint16_t       )TASK1_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK1_TASK_PRIO,
                    (TaskHandle_t*  )&Task1Task_Handler);
    vTaskStartScheduler();

    while (1) {
        // GPIO_WriteBit(GPIOA, GPIO_Pin_0, (i == 0) ? (i = Bit_SET) : (i =
        // Bit_RESET));
        i++;
        y += i;
        GPIOA->BSHR |= GPIO_BSHR_BS15;
        GPIOB->BSHR |= GPIO_BSHR_BR4;
        delay(1000000);
        GPIOA->BSHR |= GPIO_BSHR_BR15;
        GPIOB->BSHR |= GPIO_BSHR_BS4;
        delay(1000000);
    }
}

void delay(volatile uint32_t val) {
    while (val--) {}
}
