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

    // second variant (without pass object) is get raw address of overrided
    // virtual method and send it to xTaskCreate 
    // (undefined behaviour in common case)
    //void* fPtr = addr(this);
    //handler =
    //    xTaskCreate((TaskFunction_t)fPtr, (const char*)"run_task", stackSize,
    //                pvParameters, priority, (TaskHandle_t*)&handler)
    //        ? handler
    //        : nullptr;
}

void FR_OS::Run(void* obj) {
    static_cast<FR_OS*>(obj)->runTask(static_cast<FR_OS*>(obj)->pvParameters);
}

//void* addr(FR_OS* pC) {
//    void** vPtr = *reinterpret_cast<void***>(pC);
//    return vPtr[0];
//}
