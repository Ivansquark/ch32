#ifndef BUTTONS_H
#define BUTTONS_H

#include "main.h"
#include "frwrapper.h"
#include "ch32v30x_adc.h"

class Buttons : FR_OS {
    public:
        Buttons(uint16_t stackSize = 16);
        void runTask(void* pvParameters) override;

    private:
        uint16_t V = 0;
        uint16_t H = 0;
        void but_init();
        void joy_init();
        inline uint16_t getV();
        inline uint16_t getH();

};

#endif //BUTTONS_H
