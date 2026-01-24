#include "../hw/hw.h"
#include <Arduino.h>
#include <Serial.h>

namespace {
    bool serialInitilized = false;
}

void initailizeSerialIfNeeded() {
    if (!serialInitilized) {
        Serial.begin(9600);
        serialInitilized = true;
    }
}

void HW::pinMode(uint8_t pin, PinMode mode) {
    if (mode == PinMode::INPUT) {
        pinMode(pin, INPUT);
    } else if (mode == PinMode::INPUT_PULLUP) {
        pinMode(pin, INPUT_PULLUP);
    } else {
        pinMode(pin, OUTPUT);
    }
}

void HW::digitalWrite(uint8_t pin, bool value) {
    ::digitalWrite(pin, value ? HIGH : LOW);
}

bool HW::digitalRead(uint8_t pin) {
    return ::digitalRead(pin) == HIGH;
}

int HW::analogRead(uint8_t pin) {
    return ::analogRead(pin);
}

void HW::println(const char* fmt, ...) {
    initailizeSerialIfNeeded();
    
    char buffer[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    Serial.println(buffer);
}
