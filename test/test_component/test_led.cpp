#include <unity.h>
#include "led.h"
#include "test_component.h"
#include "../virtual_hw.h"

LED led(13);

void setUp_led() {
    led.initialize();
}

void test_led_initial_state() {
    TEST_ASSERT_EQUAL(HW::PinMode::OUTPUT, HW::getPinMode(13));
    TEST_ASSERT_FALSE(HW::digitalRead(13));
}

void test_led_set_on() {
    led.setState(true);
    TEST_ASSERT_TRUE(HW::digitalRead(13));
}

void test_led_set_off() {
    led.setState(false);
    TEST_ASSERT_FALSE(HW::digitalRead(13));
}

void test_led_blink() {
    auto blinkDelay = 400;

    led.blink(blinkDelay);
    TEST_ASSERT_FALSE(HW::digitalRead(13));

    led.tick(0);
    TEST_ASSERT_TRUE(HW::digitalRead(13));

    led.tick(blinkDelay - 1);
    TEST_ASSERT_TRUE(HW::digitalRead(13));

    led.tick(1);
    TEST_ASSERT_FALSE(HW::digitalRead(13));

    led.tick(blinkDelay - 1);
    TEST_ASSERT_FALSE(HW::digitalRead(13));

    led.tick(1);
    TEST_ASSERT_TRUE(HW::digitalRead(13));
}


void run_led_tests() {
    RUN_TEST(test_led_initial_state);
    RUN_TEST(test_led_set_on);
    RUN_TEST(test_led_set_off);
    RUN_TEST(test_led_blink);
}
