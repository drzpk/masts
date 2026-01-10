#pragma once
#include "hw.h"

void resetHw();

namespace HW {
    HW::PinMode getPinMode(uint8_t pin);
}
