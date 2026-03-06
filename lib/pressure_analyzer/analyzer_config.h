#pragma once

// Number of measurements for linear regression slope calculation
constexpr int REGRESSION_WINDOW_SIZE = 5;

// EMA smoothing factor (alpha)
// Higher values (closer to 1.0) = less smoothing, faster response
// Lower values (closer to 0.0) = more smoothing, slower response
constexpr float EMA_ALPHA = 0.4f;

