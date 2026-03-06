#include <unity.h>
#include "pressure_slope.h"
#include "analyzer_config.h"
#include "test_pressure_analyzer.h"

// Single measurement – not enough data for slope
void test_slope_initial_returns_zero() {
    PressureSlope ps;
    float result = ps.update(0.5f, 100);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, result);
}

// Two identical measurements – slope should be exactly 0
void test_slope_two_identical_measurements() {
    PressureSlope ps;
    ps.update(0.5f, 100);
    float result = ps.update(0.5f, 100);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, result);
}

// Constant input over many updates – EMA stabilises and slope stays ~0
void test_slope_constant_input_near_zero() {
    PressureSlope ps;
    float result = 0.0f;
    for (int i = 0; i < REGRESSION_WINDOW_SIZE * 4; i++) {
        result = ps.update(0.5f, 100);
    }
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 0.0f, result);
}

// Increasing values – slope must be positive
void test_slope_positive_for_increasing_values() {
    PressureSlope ps;

    // Stabilise EMA at 0.0 first
    for (int i = 0; i < REGRESSION_WINDOW_SIZE * 4; i++) {
        ps.update(0.0f, 100);
    }

    // Feed steadily increasing values; fill the window several times over
    float result = 0.0f;
    for (int i = 1; i <= REGRESSION_WINDOW_SIZE * 4; i++) {
        result = ps.update(i * 0.05f, 100);
    }

    TEST_ASSERT_TRUE(result > 0.0f);
}

// Decreasing values – slope must be negative
void test_slope_negative_for_decreasing_values() {
    PressureSlope ps;

    // Stabilise EMA at 1.0 first
    for (int i = 0; i < REGRESSION_WINDOW_SIZE * 4; i++) {
        ps.update(1.0f, 100);
    }

    // Feed steadily decreasing values
    float result = 0.0f;
    for (int i = 1; i <= REGRESSION_WINDOW_SIZE * 4; i++) {
        result = ps.update(1.0f - i * 0.05f, 100);
    }

    TEST_ASSERT_TRUE(result < 0.0f);
}

// More updates than the window size – circular buffer must wrap without issues
void test_slope_window_wraps_without_error() {
    PressureSlope ps;
    float result = 0.0f;
    for (int i = 0; i < REGRESSION_WINDOW_SIZE * 10; i++) {
        result = ps.update(0.5f, 100);
    }
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 0.0f, result);
}

// Varying deltaMs – timestamps are accumulated correctly and slope sign is preserved
void test_slope_varying_delta_ms() {
    PressureSlope ps;
    const int deltas[] = {50, 150, 100, 200, 100};

    // Stabilise EMA at 0.0
    for (int i = 0; i < REGRESSION_WINDOW_SIZE * 4; i++) {
        ps.update(0.0f, 100);
    }

    // Increasing values with varying time steps
    float result = 0.0f;
    for (int i = 1; i <= REGRESSION_WINDOW_SIZE * 4; i++) {
        int delta = deltas[i % 5];
        result = ps.update(i * 0.05f, delta);
    }

    TEST_ASSERT_TRUE(result > 0.0f);
}

// EMA initialisation: first raw value is used directly (no smoothing on first call)
void test_slope_ema_initialises_to_first_value() {
    PressureSlope ps;

    // If EMA initialises to the first raw value (0.8), two identical updates
    // produce the same measurement → slope = 0.
    ps.update(0.8f, 100);
    float result = ps.update(0.8f, 100);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, result);
}

void run_pressure_slope_tests() {
    RUN_TEST(test_slope_initial_returns_zero);
    RUN_TEST(test_slope_two_identical_measurements);
    RUN_TEST(test_slope_constant_input_near_zero);
    RUN_TEST(test_slope_positive_for_increasing_values);
    RUN_TEST(test_slope_negative_for_decreasing_values);
    RUN_TEST(test_slope_window_wraps_without_error);
    RUN_TEST(test_slope_varying_delta_ms);
    RUN_TEST(test_slope_ema_initialises_to_first_value);
}
