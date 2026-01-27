#include "mast.h"
#include "hw.h"
#include <cstdio>

Mast::Mast(const MastConfig& config)
    : _config(config),
      _upButton(config.pinButtonUp),
      _downButton(config.pinButtonDown),
      _upLed(config.pinLedUp),
      _downLed(config.pinLedDown),
      _state(MastState::IDLE),
      _movementTimeLeftMillis(0) {}

void Mast::initialize() {
    HW::pinMode(_config.pinValveUp, HW::PinMode::OUTPUT);
    HW::pinMode(_config.pinValveDown, HW::PinMode::OUTPUT);
    _upLed.initialize();
    _downLed.initialize();
    _upButton.initialize();
    _downButton.initialize();

    _upLed.setState(true);
    _downLed.setState(true);
}

static const char* mastStateToString(MastState state);

void Mast::tick(int deltaMs) {
    _upButton.tick(deltaMs);
    _downButton.tick(deltaMs);
    _upLed.tick(deltaMs);
    _downLed.tick(deltaMs);
    
    switch (_state) {
        case MastState::IDLE:
            if (_upButton.wasPressed()) {
                setState(MastState::RISING);
            } else if (_downButton.wasPressed()) {
                setState(MastState::LOWERING);
            } else if (_upButton.isLongPressed()) {
                setState(MastState::RISING_FORCED);
            } else if (_downButton.isLongPressed()) {
                setState(MastState::LOWERING_FORCED);
            }
            break;
        case MastState::UP:
            if (_downButton.wasPressed()) {
                setState(MastState::LOWERING);
            } else if (_downButton.isLongPressed()) {
                setState(MastState::LOWERING_FORCED);
            } else if (_upButton.isLongPressed()) {
                setState(MastState::RISING_FORCED);
            }
            break;
        case MastState::DOWN:
            if (_upButton.wasPressed()) {
                setState(MastState::RISING);
            } else if (_upButton.isLongPressed()) {
                setState(MastState::RISING_FORCED);
            } else if (_downButton.isLongPressed()) {
                setState(MastState::LOWERING_FORCED);
            }
            break;
        case MastState::RISING:
            _movementTimeLeftMillis -= deltaMs;
            if (_movementTimeLeftMillis <= 0) {
                setState(MastState::UP);
            } else if (_upButton.wasPressed() || _downButton.wasPressed()) {
                setState(MastState::IDLE);
            }
            break;
        case MastState::LOWERING:
            _movementTimeLeftMillis -= deltaMs;
            if (_movementTimeLeftMillis <= 0) {
                setState(MastState::DOWN);
            } else if (_upButton.wasPressed() || _downButton.wasPressed()) {
                setState(MastState::IDLE);
            }
            break;
        case MastState::RISING_FORCED:
            if (!_upButton.isLongPressed()) {
                setState(MastState::IDLE);
            }
            break;
        case MastState::LOWERING_FORCED:
            if (!_downButton.isLongPressed()) {
                setState(MastState::IDLE);
            }
            break;
    }

    _upButton.clearPress();
    _downButton.clearPress();
}

void Mast::setState(MastState newState) {
    HW::println("Mast %d changing state from %s to %s", _config.mastId, mastStateToString(_state), mastStateToString(newState));
    switch (newState) {
        case MastState::IDLE:
            HW::digitalWrite(_config.pinValveUp, false);
            HW::digitalWrite(_config.pinValveDown, false);
            _upLed.setState(true);
            _downLed.setState(true);
            break;
        case MastState::RISING:
            HW::digitalWrite(_config.pinValveUp, true);
            HW::digitalWrite(_config.pinValveDown, false);
            _upLed.blink(BTN_BLINK_INTERVAL_NORMAL_MS);
            _upLed.tick(0);
            _downLed.setState(false);
            _movementTimeLeftMillis = _config.raiseTimeMs;
            break;
        case MastState::LOWERING:
            HW::digitalWrite(_config.pinValveUp, false);
            HW::digitalWrite(_config.pinValveDown, true);
            _upLed.setState(false);
            _downLed.blink(BTN_BLINK_INTERVAL_NORMAL_MS);
            _downLed.tick(0);
            _movementTimeLeftMillis = _config.lowerTimeMs;
            break;
        case MastState::UP:
            HW::digitalWrite(_config.pinValveUp, false);
            HW::digitalWrite(_config.pinValveDown, false);
            _upLed.setState(true);
            _downLed.setState(false);
            break;
        case MastState::DOWN:
            HW::digitalWrite(_config.pinValveUp, false);
            HW::digitalWrite(_config.pinValveDown, false);
            _upLed.setState(false);
            _downLed.setState(true);
            break;
        case MastState::RISING_FORCED:
            HW::digitalWrite(_config.pinValveUp, true);
            HW::digitalWrite(_config.pinValveDown, false);
            _upLed.blink(BTN_BLINK_INTERVAL_FORCED_MS);
            _upLed.tick(0);
            break;
        case MastState::LOWERING_FORCED:
            HW::digitalWrite(_config.pinValveUp, false);
            HW::digitalWrite(_config.pinValveDown, true);
            _downLed.blink(BTN_BLINK_INTERVAL_FORCED_MS);
            _downLed.tick(0);
            break;
    }
    _state = newState;
}

static const char* mastStateToString(MastState state) {
    switch (state) {
        case MastState::IDLE: return "IDLE";
        case MastState::RISING: return "RISING";
        case MastState::LOWERING: return "LOWERING";
        case MastState::UP: return "UP";
        case MastState::DOWN: return "DOWN";
        case MastState::RISING_FORCED: return "RISING_FORCED";
        case MastState::LOWERING_FORCED: return "LOWERING_FORCED";
        default: return "UNKNOWN";
    }
}
