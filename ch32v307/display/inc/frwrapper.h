#ifndef FRWRAPPER_H
#define FRWRAPPER_H

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

class FR_OS {
  public:
    FR_OS(uint16_t stackSize = configMINIMAL_STACK_SIZE,
          UBaseType_t priority = 0, void* pvParameters = nullptr);
    virtual void runTask(void* pvParameters) = 0; // pure virtual
    static void startOS() { vTaskStartScheduler(); }

    // must be called in constructor of derived class, xTaskCreat in init_FR_OS
    void init_FR_OS();

    TaskHandle_t handler = nullptr;
    FR_OS* obj = nullptr;
    uint16_t stackSize = configMINIMAL_STACK_SIZE;
    UBaseType_t priority = 0;
    void* pvParameters = nullptr;

  private:
    static void Run(void* obj);
};

#endif // FRWRAPPER_H
