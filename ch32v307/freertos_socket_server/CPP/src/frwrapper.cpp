#include "frwrapper.h"

FR_OS::FR_OS(uint16_t stackSize, UBaseType_t priority, void* pvParameters) {
    init(stackSize, priority, pvParameters);
}

void FR_OS::init(uint16_t stackSize, UBaseType_t priority, void* pvParameters) {
    this->pvParameters = pvParameters;
    obj = this;
    handler = xTaskCreate((TaskFunction_t)Run, (const char*)"run_task",
                          stackSize, reinterpret_cast<void*>(obj), priority,
                          (TaskHandle_t*)&handler)
                  ? handler
                  : nullptr;
    // void *fp = &FR_OS::runTask;
    // void* f = GetMethodPointer(this->Run(this));
}

void FR_OS::Run(void* obj) {
    static_cast<FR_OS*>(obj)->runTask(static_cast<FR_OS*>(obj)->pvParameters);
}
