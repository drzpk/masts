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
            }
            break;
        case MastState::UP:
            if (_downButton.wasPressed()) {
                setState(MastState::LOWERING);
            }
            break;
        case MastState::DOWN:
            if (_upButton.wasPressed()) {
                setState(MastState::RISING);
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
            _upLed.blink();
            _upLed.tick(0);
            _downLed.setState(false);
            _movementTimeLeftMillis = _config.raiseTimeMs;
            break;
        case MastState::LOWERING:
            HW::digitalWrite(_config.pinValveUp, false);
            HW::digitalWrite(_config.pinValveDown, true);
            _upLed.setState(false);
            _downLed.blink();
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
        default: return "UNKNOWN";
    }
}
