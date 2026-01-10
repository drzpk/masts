#include "virtual_hw.h"

#include <cstdarg>
#include <cstdio>
#include <map>
#include <stdexcept>
#include <iostream>


namespace {
    std::map<uint8_t, HW::PinMode> pinModes;
    std::map<uint8_t, bool> pinStates;
}

void resetHw() {
    pinModes.clear();
    pinStates.clear();
}

HW::PinMode HW::getPinMode(uint8_t pin) {
    return pinModes[pin];
}

void HW::pinMode(uint8_t pin, PinMode mode) {
    pinModes[pin] = mode;
}

void HW::digitalWrite(uint8_t pin, bool value) {
    pinStates[pin] = value;
}

bool HW::digitalRead(uint8_t pin) {
    return pinStates[pin] ?: false;
}

void HW::println(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buffer[512];
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    std::cout << buffer << std::endl;
}
