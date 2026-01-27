#include <unity.h>
#include "mast.h"
#include "test_component.h"
#include "../virtual_hw.h"

const MastConfig testConfig = {
    .mastId = 1,
    .pinValveUp = 1,
    .pinValveDown = 2,
    .pinLedUp = 3,
    .pinLedDown = 4,
    .pinButtonUp = 5,
    .pinButtonDown = 6,
    .raiseTimeMs = 1000,
    .lowerTimeMs = 600
};

void initializeMast(Mast& mast) {
    // Buttons work in pull-up mode, and signals must be reversed 
    HW::digitalWrite(testConfig.pinButtonUp, true);
    HW::digitalWrite(testConfig.pinButtonDown, true);
    mast.initialize();
}

void pressButtonLong(Mast& mast, int pin) {
    HW::digitalWrite(pin, false);
    mast.tick(0);
    mast.tick(3000); // Hold for 3 seconds to trigger long press
}

void releaseButton(Mast& mast, int pin) {
    HW::digitalWrite(pin, true);
    mast.tick(0);
}

void pressButtonShort(Mast& mast, int pin) {
    HW::digitalWrite(pin, false);
    mast.tick(10);
    HW::digitalWrite(pin, true);
    mast.tick(0);
}

void test_mast_initial_conditions() {
    Mast mast(testConfig);
    initializeMast(mast);
    
    TEST_ASSERT_EQUAL(HW::PinMode::OUTPUT, HW::getPinMode(testConfig.pinValveUp));
    TEST_ASSERT_EQUAL(HW::PinMode::OUTPUT, HW::getPinMode(testConfig.pinValveDown));
    TEST_ASSERT_EQUAL(HW::PinMode::OUTPUT, HW::getPinMode(testConfig.pinLedUp));
    TEST_ASSERT_EQUAL(HW::PinMode::OUTPUT, HW::getPinMode(testConfig.pinLedDown));
    TEST_ASSERT_EQUAL(HW::PinMode::INPUT_PULLUP, HW::getPinMode(testConfig.pinButtonUp));
    TEST_ASSERT_EQUAL(HW::PinMode::INPUT_PULLUP, HW::getPinMode(testConfig.pinButtonDown));
}

void test_mast_press_up_button() {
    Mast mast(testConfig);
    initializeMast(mast);
    
    pressButtonShort(mast, testConfig.pinButtonUp);

    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));

    mast.tick(500);

    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));

    mast.tick(499);

    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));

    mast.tick(1);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));
}

void test_mast_press_up_button_leds() {
    Mast mast(testConfig);
    initializeMast(mast);

    pressButtonShort(mast, testConfig.pinButtonUp);

    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinLedUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinLedDown));

    mast.tick(399);

    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinLedUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinLedDown));

    mast.tick(1);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinLedUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinLedDown));

    mast.tick(399);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinLedUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinLedDown));

    mast.tick(1);

    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinLedUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinLedDown));

    mast.tick(400);

    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinLedUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinLedDown));
}

void test_mast_press_down_button() {
    Mast mast(testConfig);
    initializeMast(mast);
    
    pressButtonShort(mast, testConfig.pinButtonDown);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveDown));

    mast.tick(300);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveDown));

    mast.tick(299);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveDown));

    mast.tick(1);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));
}

void test_mast_press_down_button_leds() {
    Mast mast(testConfig);
    initializeMast(mast);

    pressButtonShort(mast, testConfig.pinButtonDown);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinLedUp));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinLedDown));

    mast.tick(399);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinLedUp));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinLedDown));

    mast.tick(1);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinLedUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinLedDown));
}

void test_mast_press_interrupt() {
    Mast mast(testConfig);
    initializeMast(mast);

    // Start raising the mast
    pressButtonShort(mast, testConfig.pinButtonUp);

    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));

    mast.tick(800);

    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));

    // Interrupt with down button
    pressButtonShort(mast, testConfig.pinButtonDown);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinLedUp));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinLedDown));

    // Start over with raising the mast
    pressButtonShort(mast, testConfig.pinButtonUp);
    mast.tick(800);

    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));
    
    // Interrupt again, this time with up button
    pressButtonShort(mast, testConfig.pinButtonUp);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinLedUp));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinLedDown));

    // Now start lowering the mast
    pressButtonShort(mast, testConfig.pinButtonDown);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveDown));

    mast.tick(599);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveDown));

    mast.tick(1);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinLedUp));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinLedDown));
}

void test_mast_long_press_forced_up() {
    Mast mast(testConfig);
    initializeMast(mast);

    pressButtonLong(mast, testConfig.pinButtonUp);

    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));

    mast.tick(10000);

    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));

    releaseButton(mast, testConfig.pinButtonUp);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));
}

void test_mast_long_press_forced_down() {
    Mast mast(testConfig);
    initializeMast(mast);

    pressButtonLong(mast, testConfig.pinButtonDown);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveDown));

    mast.tick(10000);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveDown));

    releaseButton(mast, testConfig.pinButtonDown);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));
}

void run_mast_tests() {
    RUN_TEST(test_mast_initial_conditions);
    RUN_TEST(test_mast_press_up_button);
    RUN_TEST(test_mast_press_up_button_leds);
    RUN_TEST(test_mast_press_down_button);
    RUN_TEST(test_mast_press_down_button_leds);
    RUN_TEST(test_mast_press_interrupt);
    RUN_TEST(test_mast_long_press_forced_up);
    RUN_TEST(test_mast_long_press_forced_down);
}
