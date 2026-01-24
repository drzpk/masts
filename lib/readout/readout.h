#pragma once
#include "component.h"

namespace {
    constexpr int PRINT_INTERVAL_MS = 200;
}

class Readout : public Component {
public:
    Readout(int pin);
    void initialize() override;
    void tick(int deltaMs) override;

private:
    int _pin;
    int _timeCounter;
};
