#include "button.h"
#include "hw.h"

Button::Button(int pin) : _pin(pin), _prevState(false), _pressed(false), _longPressed(false), _isCurrentlyPressed(false), _pressDuration(-1) {}

void Button::initialize() {
    HW::pinMode(_pin, HW::PinMode::INPUT_PULLUP);
}

void Button::tick(int deltaMs) {
    bool currentState = !HW::digitalRead(_pin);

     if (!_longPressed) {
        _pressed = _prevState && !currentState;
        if (_pressed) {
            HW::println("Button press detected on pin %d", _pin);
        }
    }

    // Long press detection
    if (currentState && !_prevState && !_pressed) {
        _pressDuration = 0;
    } else if (currentState && _prevState && _pressDuration >= 0) {
        _pressDuration += deltaMs;
    } else if (!currentState) {
        _pressDuration = -1;
        _longPressed = false;
    }

    bool justLongPressed = !_longPressed && (_pressDuration >= LONG_PRESS_DURATION_MS);
    if (justLongPressed) {
        _longPressed = true;
        _pressed = false;
        HW::println("Long press detected on pin %d", _pin);
    }

    _prevState = currentState;
}
