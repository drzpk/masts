#pragma once
#include "component.h"
#include "led.h"
#include "mast.h"
#include <stdint.h>

namespace {
    constexpr int STATUS_LED_BLINK_INTERVAL_MS = 200;
}

struct StatusLedsConfig {
    int pinStatusLed;
    const MastState* const* mastStates;
    int mastCount;
};

class StatusLeds : public Component {
public:
    StatusLeds(const StatusLedsConfig& config);
    void initialize() override;
    void tick(int deltaMs) override;

private:
    StatusLedsConfig _config;
    LED _statusLed;
    bool _wasMoving;
    bool isAnyMastMoving() const;
};
