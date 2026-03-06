#include "pressure_slope.h"

PressureSlope::PressureSlope() : _emaValue(-1.0f), _currentIndex(0), _measurementCount(0) {
    for (int i = 0; i < REGRESSION_WINDOW_SIZE; i++) {
        _measurements[i] = 0.0f;
        _timestamps[i] = 0;
    }
}

float PressureSlope::update(float raw, int deltaMs) {
    // Phase 1: EMA (Exponential Moving Average) smoothing
    applyEMA(raw);
    
    // Phase 2: Store smoothed value in regression window
    // Update circular buffer
    _measurements[_currentIndex] = _emaValue;
    
    // Calculate timestamp based on delta (in milliseconds)
    unsigned long timestamp = 0;
    if (_measurementCount > 0) {
        int prevIndex = (_currentIndex - 1 + REGRESSION_WINDOW_SIZE) % REGRESSION_WINDOW_SIZE;
        timestamp = _timestamps[prevIndex] + deltaMs;
    }
    _timestamps[_currentIndex] = timestamp;
    
    // Move to next position
    _currentIndex = (_currentIndex + 1) % REGRESSION_WINDOW_SIZE;
    if (_measurementCount < REGRESSION_WINDOW_SIZE) {
        _measurementCount++;
    }
    
    // Calculate and return slope
    return calculateSlope();
}

void PressureSlope::applyEMA(float raw) {
    if (_emaValue < 0.0f) {
        _emaValue = raw;
    } else {
        _emaValue = EMA_ALPHA * raw + (1.0f - EMA_ALPHA) * _emaValue;
    }
}

float PressureSlope::calculateSlope() {
    if (_measurementCount < 2) {
        return 0.0f;
    }
    
    // Least squares linear regression
    // slope = (N * Σ(x*y) - Σx * Σy) / (N * Σ(x²) - (Σx)²)
    
    float sumX = 0.0f;
    float sumY = 0.0f;
    float sumXY = 0.0f;
    float sumX2 = 0.0f;
    
    int n = _measurementCount;
    
    for (int i = 0; i < n; i++) {
        float x = static_cast<float>(_timestamps[i]);
        float y = _measurements[i];
        
        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumX2 += x * x;
    }
    
    float denominator = n * sumX2 - sumX * sumX;
    if (denominator == 0.0f) {
        return 0.0f;
    }
    
    float slope = (n * sumXY - sumX * sumY) / denominator;
    return slope;
}
