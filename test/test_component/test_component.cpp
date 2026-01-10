#include <unity.h>
#include "test_component.h"
#include "../virtual_hw.h"

void setUp() {
    resetHw();
    setUp_led();
}

void tearDown() {

}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    run_button_tests();
    run_led_tests();
    run_mast_tests();
    UNITY_END();
    return 0;
}
