#pragma once
#include "../component/mast.h"
#include <Arduino.h>

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

MastConfig mastConfigs[] = { mast1Config };
