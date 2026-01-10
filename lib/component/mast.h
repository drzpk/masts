#pragma once
#include "component.h"
#include "button.h"
#include "led.h"
#include <stdint.h>


namespace {
    constexpr int BLINK_INTERVAL_MS = 400;
    constexpr int TICK_TIME_MS = 10;
}

enum class MastState {
    IDLE,
    RISING,
    LOWERING,
    UP,
    DOWN
};

enum class MastButtonDirection {
    UP,
    DOWN
};

struct MastConfig {
    int mastId;
    int pinValveUp;
    int pinValveDown;
    int pinLedUp;
    int pinLedDown;
    int pinButtonUp;
    int pinButtonDown;
    unsigned long raiseTimeMs;
    unsigned long lowerTimeMs;
};

class Mast : public Component {
public:
    Mast(const MastConfig& config);
    void initialize() override;
    void tick(int deltaMs) override;

private:
    MastConfig _config;
    MastState _state;
    Button _upButton;
    Button _downButton;
    LED _upLed;
    LED _downLed;
    long _movementTimeLeftMillis;
    void setState(MastState newState);
};
