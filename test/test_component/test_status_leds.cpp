#include <unity.h>
#include "status_leds.h"
#include "test_component.h"
#include "../virtual_hw.h"

StatusLeds* statusLeds = nullptr;

void setUp_status_leds() {
}

void test_status_leds_initial_state() {
    MastState state1 = MastState::IDLE;
    const MastState* states[] = {&state1};
    
    StatusLedsConfig config = {
        .pinStatusLed = 13,
        .mastStates = states,
        .mastCount = 1
    };
    
    statusLeds = new StatusLeds(config);
    statusLeds->initialize();
    
    TEST_ASSERT_EQUAL(HW::PinMode::OUTPUT, HW::getPinMode(13));
    TEST_ASSERT_TRUE(HW::digitalRead(13));
    
    delete statusLeds;
}

void test_status_leds_rising() {
    MastState state1 = MastState::IDLE;
    MastState state2 = MastState::IDLE;
    const MastState* states[] = {&state1, &state2};
    
    StatusLedsConfig config = {
        .pinStatusLed = 13,
        .mastStates = states,
        .mastCount = 2
    };
    
    statusLeds = new StatusLeds(config);
    statusLeds->initialize();
    
    state1 = MastState::RISING;
    statusLeds->tick(0);
    TEST_ASSERT_TRUE(HW::digitalRead(13));
    
    statusLeds->tick(50);
    TEST_ASSERT_TRUE(HW::digitalRead(13));

    statusLeds->tick(50);
    TEST_ASSERT_FALSE(HW::digitalRead(13));

    statusLeds->tick(100);
}

void test_status_leds_lowering() {
    MastState state1 = MastState::IDLE;
    MastState state2 = MastState::IDLE;
    const MastState* states[] = {&state1, &state2};
    
    StatusLedsConfig config = {
        .pinStatusLed = 13,
        .mastStates = states,
        .mastCount = 2
    };
    
    statusLeds = new StatusLeds(config);
    statusLeds->initialize();
    
    state2 = MastState::LOWERING_FORCED;
    statusLeds->tick(0);
    TEST_ASSERT_TRUE(HW::digitalRead(13));
    
    statusLeds->tick(100);
}

void run_status_leds_tests() {
    RUN_TEST(test_status_leds_initial_state);
    RUN_TEST(test_status_leds_rising);
    RUN_TEST(test_status_leds_lowering);
}
