#pragma once
#include <Arduino.h>
#include "../component/mast.h"
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

Component* components[] = {
  new Mast(mast1Config),
#if DEBUG_ANALOG_READOUT
  new Readout(A0),
#endif
};

int componentCount = sizeof(components) / sizeof(Component*);
