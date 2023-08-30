#ifndef VOLTAGE
#define VOLTAGE

#include "main.h"
#include "adc.h"
#include "dma.h"
#include "pwm.h"

#define VALUE_FOR_7V (int)((9*(1024/2)/5))
#define MAX_DEVIATION_FROM_7V (int)(50)

void voltage_handler();

#endif //VOLTAGE
