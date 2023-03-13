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

  private:
    // xTaskCreate in init
    void init(uint16_t stackSize, UBaseType_t priority, void* pvParameters);
    TaskHandle_t handler = nullptr;
    FR_OS* obj = nullptr;
    void* pvParameters = nullptr;
    static void Run(void* obj);

    //void* addr(FR_OS* pC);
};

#endif // FRWRAPPER_H
