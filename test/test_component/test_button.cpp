#include <unity.h>
#include "button.h"
#include "test_component.h"
#include "../virtual_hw.h"

void test_button_press() {
    Button btn(5);
    btn.initialize();
    TEST_ASSERT_EQUAL(HW::PinMode::INPUT, HW::getPinMode(5));

    // Not pressed
    HW::digitalWrite(5, false);
    btn.tick(0);
    TEST_ASSERT_FALSE(btn.wasPressed());

    // Button pressed
    HW::digitalWrite(5, true);
    btn.tick(0);
    TEST_ASSERT_FALSE(btn.wasPressed());
    
    // Button released
    HW::digitalWrite(5, false);
    btn.tick(0);
    TEST_ASSERT_TRUE(btn.wasPressed());
    
    // Press was consumed
    TEST_ASSERT_FALSE(btn.wasPressed());
}

void run_button_tests() {
    RUN_TEST(test_button_press);
}
