#include "virtual_hw.h"
#include <map>
#include <stdexcept>


namespace {
    std::map<uint8_t, HW::PinMode> pinModes;
    std::map<uint8_t, bool> pinStates;
}

void reset_hw() {
    pinModes.clear();
    pinStates.clear();
}

void HW::pinMode(uint8_t pin, PinMode mode) {
    pinModes[pin] = mode;
}

void HW::digitalWrite(uint8_t pin, bool value) {
    if (pinModes[pin] != PinMode::OUTPUT) {
        throw std::runtime_error("Pin not set as OUTPUT");
    }

    pinStates[pin] = value;
}

bool HW::digitalRead(uint8_t pin) {
    if (pinModes[pin] != PinMode::OUTPUT) {
        throw std::runtime_error("Pin not set as OUTPUT");
    }

    return pinStates[pin] ?: false;
}
