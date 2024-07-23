#include "tirq.h"

void (*InterruptManager::arrIrq[104])(void) = {
    &InterruptManager::defaultHandler};
