#include <unity.h>
#include "button.h"
#include "test_component.h"
#include "../virtual_hw.h"

void test_button_short_press() {
    Button btn(5);
    btn.initialize();
    TEST_ASSERT_EQUAL(HW::PinMode::INPUT_PULLUP, HW::getPinMode(5));

    // Not pressed
    HW::digitalWrite(5, true);
    btn.tick(0);
    TEST_ASSERT_FALSE(btn.wasPressed());
    TEST_ASSERT_FALSE(btn.isLongPressed());
    btn.clearPress();

    // Button pressed
    HW::digitalWrite(5, false);
    btn.tick(0);
    TEST_ASSERT_FALSE(btn.wasPressed());
    TEST_ASSERT_FALSE(btn.isLongPressed());
    
    // Button released (short press)
    HW::digitalWrite(5, true);
    btn.tick(0);
    TEST_ASSERT_TRUE(btn.wasPressed());
    TEST_ASSERT_FALSE(btn.isLongPressed());
    btn.clearPress();
    
    // Press was consumed
    TEST_ASSERT_FALSE(btn.wasPressed());
    TEST_ASSERT_FALSE(btn.isLongPressed());
}

void test_button_long_press() {
    Button btn(5);
    btn.initialize();

    // Button pressed down
    HW::digitalWrite(5, false);
    btn.tick(0);
    TEST_ASSERT_FALSE(btn.wasPressed());
    TEST_ASSERT_FALSE(btn.isLongPressed());

    // Hold for 1 second (not long enough)
    btn.tick(1000);
    TEST_ASSERT_FALSE(btn.wasPressed());
    TEST_ASSERT_FALSE(btn.isLongPressed());

    // Hold for another 2 seconds (total 3 seconds - should trigger long press)
    btn.tick(2000);
    TEST_ASSERT_FALSE(btn.wasPressed()); // Long press should force normal press to false
    TEST_ASSERT_TRUE(btn.isLongPressed());

    // Button released after long press
    HW::digitalWrite(5, true);
    btn.tick(0);
    TEST_ASSERT_FALSE(btn.wasPressed()); // Should still be false
    TEST_ASSERT_FALSE(btn.isLongPressed());
}

void run_button_tests() {
    RUN_TEST(test_button_short_press);
    RUN_TEST(test_button_long_press);
}
