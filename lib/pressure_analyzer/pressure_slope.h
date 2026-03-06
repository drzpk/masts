#pragma once

#include "analyzer_config.h"

class PressureSlope {
public:
    PressureSlope();
    
    // Takes raw pressure value (0 to 1) and time delta in milliseconds
    // Returns normalized slope value
    float update(float raw, int deltaMs);

private:
    float _emaValue;  // -1.0f = uninitialized (raw values are 0 to 1)
    
    float _measurements[REGRESSION_WINDOW_SIZE];
    unsigned long _timestamps[REGRESSION_WINDOW_SIZE];
    int _currentIndex;
    int _measurementCount;
    
    void applyEMA(float raw);
    float calculateSlope();
};
