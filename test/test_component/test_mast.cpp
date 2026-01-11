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

void pressButton(Mast& mast, int pin) {
    HW::digitalWrite(pin, true);
    mast.tick(10);
    HW::digitalWrite(pin, false);
    mast.tick(0);
}

void test_mast_initial_conditions() {
    Mast mast(testConfig);
    mast.initialize();
    
    TEST_ASSERT_EQUAL(HW::PinMode::OUTPUT, HW::getPinMode(testConfig.pinValveUp));
    TEST_ASSERT_EQUAL(HW::PinMode::OUTPUT, HW::getPinMode(testConfig.pinValveDown));
    TEST_ASSERT_EQUAL(HW::PinMode::OUTPUT, HW::getPinMode(testConfig.pinLedUp));
    TEST_ASSERT_EQUAL(HW::PinMode::OUTPUT, HW::getPinMode(testConfig.pinLedDown));
    TEST_ASSERT_EQUAL(HW::PinMode::INPUT_PULLUP, HW::getPinMode(testConfig.pinButtonUp));
    TEST_ASSERT_EQUAL(HW::PinMode::INPUT_PULLUP, HW::getPinMode(testConfig.pinButtonDown));
}

void test_mast_press_up_button() {
    Mast mast(testConfig);
    mast.initialize();
    
    pressButton(mast, testConfig.pinButtonUp);

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
    mast.initialize();

    pressButton(mast, testConfig.pinButtonUp);

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
    mast.initialize();
    
    pressButton(mast, testConfig.pinButtonDown);

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
    mast.initialize();

    pressButton(mast, testConfig.pinButtonDown);

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
    mast.initialize();

    // Start raising the mast
    pressButton(mast, testConfig.pinButtonUp);

    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));

    mast.tick(800);

    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));

    // Interrupt with down button
    pressButton(mast, testConfig.pinButtonDown);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinLedUp));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinLedDown));

    // Start over with raising the mast
    pressButton(mast, testConfig.pinButtonUp);
    mast.tick(800);

    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));
    
    // Interrupt again, this time with up button
    pressButton(mast, testConfig.pinButtonUp);

    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveUp));
    TEST_ASSERT_FALSE(HW::digitalRead(testConfig.pinValveDown));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinLedUp));
    TEST_ASSERT_TRUE(HW::digitalRead(testConfig.pinLedDown));

    // Now start lowering the mast
    pressButton(mast, testConfig.pinButtonDown);

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

void run_mast_tests() {
    RUN_TEST(test_mast_initial_conditions);
    RUN_TEST(test_mast_press_up_button);
    RUN_TEST(test_mast_press_up_button_leds);
    RUN_TEST(test_mast_press_down_button);
    RUN_TEST(test_mast_press_down_button_leds);
    RUN_TEST(test_mast_press_interrupt);
}
