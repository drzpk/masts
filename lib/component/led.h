#pragma once
#include "component.h"

class LED : public Component {
public:
    LED(int port);
    void initialize() override;
    void tick(int deltaMs) override;
    void setState(bool on);
    void blink(int delayMs = 400);
private:
    int _port;
    bool _isBlinking;
    bool _state;
    int _elapsedMs;
    int _blinkDelayMs;
};
