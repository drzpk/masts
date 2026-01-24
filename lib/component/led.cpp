#include "led.h"
#include "hw.h"
#include <iostream>




LED::LED(int port) : _port(port), _isBlinking(false), _state(false), _elapsedMs(0), _blinkDelayMs(0) {
}

void LED::initialize() {
    HW::pinMode(_port, HW::PinMode::OUTPUT);
    HW::digitalWrite(_port, false);
}

void LED::tick(int deltaMs) {
    if (_isBlinking) {
        _elapsedMs += deltaMs;
        HW::digitalWrite(_port, (_elapsedMs / _blinkDelayMs) % 2 == 0);
    }
}

void LED::setState(bool on) {
    _isBlinking = false;
    _state = on;
    HW::digitalWrite(_port, on);
}

void LED::blink(int delayMs) {
    _isBlinking = true;
    _elapsedMs = 0;
    _blinkDelayMs = delayMs;
}
