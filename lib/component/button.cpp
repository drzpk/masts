#include "button.h"
#include "hw.h"

Button::Button(int pin) : _pin(pin), _prevState(false), _pressed(false) {}

void Button::initialize() {
    HW::pinMode(_pin, HW::PinMode::INPUT);
}

void Button::tick(int deltaMs) {
    if (_pressed) {
        // Previous press not yet consumed
        return;
    }

    bool currentState = HW::digitalRead(_pin);
    _pressed = _prevState && !currentState;
    if (_pressed) {
        HW::println("Button press detected on pin %d", _pin);
    }

    _prevState = currentState;
}

bool Button::wasPressed() {
    bool was = _pressed;
    _pressed = false;
    return was;
}
