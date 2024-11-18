#include "tirq.h"

void (*InterruptManager::arrIrq[39])(void) = {
    &InterruptManager::defaultHandler};
