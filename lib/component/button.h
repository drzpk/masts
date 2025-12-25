#pragma once
#include "component.h"

class Button : public Component {
    public:
        Button(int pin);
        void initialize() override;
        void tick(int deltaMs) override;
        bool wasPressed();

    private:
        int _pin;
        bool _prevState;
        bool _pressed;
};
