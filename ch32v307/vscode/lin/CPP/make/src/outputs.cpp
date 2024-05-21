#include "outputs.h"

Outputs* Outputs::pThis = nullptr;

Outputs::Outputs() {
    pThis = this;
    lightOff();
}
