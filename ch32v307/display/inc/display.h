#ifndef DISPLAY_H
#define DISPLAY_H

#include "main.h"
#include "frwrapper.h"

class Display : public FR_OS {
    public:
        Display();
        void runTask(void* pvParameter) override;
};

#endif //DISPLAY_H
