#pragma once
#include <Arduino.h>
#include "../component/mast.h"
#include "../component/status_leds.h"
#include "../readout/readout.h"

#define DEBUG_ANALOG_READOUT true

MastConfig mast1Config = {
  .mastId = 1,
  .pinValveUp = A7,
  .pinValveDown = D9,
  .pinLedUp = D12,
  .pinLedDown = D11,
  .pinButtonUp = D2,
  .pinButtonDown = D10,
  .raiseTimeMs = 3000,
  .lowerTimeMs = 2000
};

Mast mast1(mast1Config);

const MastState* mastStates[] = {
  &mast1.getState()
};

StatusLedsConfig statusLedsConfig = {
  .pinStatusLed = LED_BUILTIN,
  .mastStates = mastStates,
  .mastCount = sizeof(mastStates) / sizeof(MastState*)
};

Component* components[] = {
  &mast1,
  new StatusLeds(statusLedsConfig),
#if DEBUG_ANALOG_READOUT
  new Readout(A0),
#endif
};

int componentCount = sizeof(components) / sizeof(Component*);
