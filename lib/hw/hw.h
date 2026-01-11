#pragma once
#include <stdint.h>

namespace HW {
    enum class PinMode {
        INPUT,
        INPUT_PULLUP,
        OUTPUT
    };

    void pinMode(uint8_t pin, PinMode mode);
    void digitalWrite(uint8_t pin, bool value);
    bool digitalRead(uint8_t pin);
    void println(const char* fmt, ...);
}
