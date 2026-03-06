#include <unity.h>
#include "test_pressure_analyzer.h"

void setUp() {}
void tearDown() {}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    run_pressure_slope_tests();
    UNITY_END();
    return 0;
}
