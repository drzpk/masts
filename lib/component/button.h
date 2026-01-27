#pragma once
#include "component.h"

class Button : public Component {
    public:
        Button(int pin);
        void initialize() override;
        void tick(int deltaMs) override;
        bool wasPressed() { return _pressed; }
        void clearPress() { _pressed = false; }
        bool isLongPressed() { return _longPressed; }

    private:
        int _pin;
        bool _prevState;
        bool _pressed;
        bool _longPressed;
        bool _isCurrentlyPressed;
        int _pressDuration;
        static const int LONG_PRESS_DURATION_MS = 3000;
};
