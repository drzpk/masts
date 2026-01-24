#pragma once
#include <Arduino.h>
#include "../component/mast.h"
#include "../component/status_leds.h"
#include "../readout/readout.h"

#define DEBUG_ANALOG_READOUT false

MastConfig mast1Config = {
  .mastId = 1,
  .pinValveUp = A0,
  .pinValveDown = A1,
  .pinLedUp = D5,
  .pinLedDown = D7,
  .pinButtonUp = D4,
  .pinButtonDown = D6,
  .raiseTimeMs = 3000,
  .lowerTimeMs = 2000
};

MastConfig mast2Config = {
  .mastId = 2,
  .pinValveUp = A2,
  .pinValveDown = A3,
  .pinLedUp = D10,
  .pinLedDown = D12,
  .pinButtonUp = D9,
  .pinButtonDown = D11,
  .raiseTimeMs = 3000,
  .lowerTimeMs = 2000
};

Mast mast1(mast1Config);
Mast mast2(mast2Config);

const MastState* mastStates[] = {
  &mast1.getState(),
  &mast2.getState()
};

StatusLedsConfig statusLedsConfig = {
  .pinStatusLed = LED_BUILTIN,
  .mastStates = mastStates,
  .mastCount = sizeof(mastStates) / sizeof(MastState*)
};

Component* components[] = {
  &mast1,
  &mast2,
  new StatusLeds(statusLedsConfig),
#if DEBUG_ANALOG_READOUT
  new Readout(A0),
#endif
};

int componentCount = sizeof(components) / sizeof(Component*);
