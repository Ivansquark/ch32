#ifndef VOLTAGE
#define VOLTAGE

#include "main.h"
#include "adc.h"
#include "dma.h"
#include "pwm.h"

#define VALUE_FOR_7V (int)((8*(1024/2)/5))
#define MAX_DEVIATION_FROM_7V (int)(30)

void voltage_handler();

#endif //VOLTAGE
