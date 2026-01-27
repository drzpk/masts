#include "status_leds.h"
#include "hw.h"

StatusLeds::StatusLeds(const StatusLedsConfig& config)
    : _config(config),
      _statusLed(config.pinStatusLed),
      _wasMoving(false) {}

void StatusLeds::initialize() {
    _statusLed.initialize();
    _statusLed.setState(true);
}

void StatusLeds::tick(int deltaMs) {
    _statusLed.tick(deltaMs);
    
    bool isMoving = isAnyMastMoving();
    if (isMoving != _wasMoving) {
        if (isMoving) {
            _statusLed.blink(STATUS_LED_BLINK_INTERVAL_MS);
        } else {
            _statusLed.setState(true);
        }
        _wasMoving = isMoving;
    }
}

bool StatusLeds::isAnyMastMoving() const {
    for (int i = 0; i < _config.mastCount; ++i) {
        const MastState* state = _config.mastStates[i];
        if (state && (*state == MastState::RISING || *state == MastState::LOWERING 
            || *state == MastState::RISING_FORCED || *state == MastState::LOWERING_FORCED)) {
            return true;
        }
    }

    return false;
}
