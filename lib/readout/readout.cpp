#include "readout.h"
#include "hw.h"

Readout::Readout(int pin) : _pin(pin) {}

void Readout::initialize() {
    HW::pinMode(_pin, HW::PinMode::INPUT);
}

void Readout::tick(int deltaMs) {
    _timeCounter += deltaMs;
    if (_timeCounter >= PRINT_INTERVAL_MS) {
        int value = HW::analogRead(_pin);
        HW::println("#V: %d", value);
        _timeCounter = 0;
    }
}
