#include <Arduino.h>

#define STATE_IDLE 1
#define STATE_RISING 2
#define STATE_LOWERING 3
#define STATE_UP 4
#define STATE_DOWN 5

#define BUTTON_UP 1
#define BUTTON_DOWN 2

#define BLINK_INTERVAL_MS 400
#define DEBUG false
#define TICK_TIME_MS 10

struct MastState {
  int state_no;
  long state_data;
  bool button_up_state;
  bool button_down_state;
};

struct MastConfig {
  int mastId;
  int pin_valve_up;
  int pin_valve_down;
  int pin_led_up;
  int pin_led_down;
  int pin_button_up;
  int pin_button_down;
  unsigned long raise_time_ms;
  unsigned long lower_time_ms;
  MastState state;
};

MastConfig mast_1_config = {
  .mastId = 1,
  .pin_valve_up = A7,
  .pin_valve_down = D9,
  .pin_led_up = D12,
  .pin_led_down = D11,
  .pin_button_up = D2,
  .pin_button_down = D10,
  .raise_time_ms = 3000,
  .lower_time_ms = 3000,
  .state = { STATE_IDLE, 0, false, false }
};

MastConfig masts[] = { mast_1_config };
unsigned long last_time = 0;

void state_step(MastConfig &mast, unsigned long delta_ms, unsigned long total_ms);
void set_state(MastConfig &mast, int new_state);
bool was_button_pressed(MastConfig &mast, int direction);
void blink_led(MastConfig &mast, int direction, unsigned long total_ms);
void initialize_mast(MastConfig config);


void setup() {
  Serial.begin(9600);
  for (int i = 0; i < sizeof(masts) / sizeof(MastConfig); i++) {
    initialize_mast(masts[i]);
    set_state(masts[i], STATE_IDLE);
  }

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  unsigned long current_time = millis();
  unsigned long delta_ms = current_time - last_time;

  for (int i = 0; i < sizeof(masts) / sizeof(MastConfig); i++) {
    state_step(masts[i], delta_ms, current_time);
  }

  last_time = current_time;
  delay(TICK_TIME_MS);
}

void state_step(MastConfig &mast, unsigned long delta_ms, unsigned long total_ms) {
  switch (mast.state.state_no) {
    case STATE_IDLE:
      if (was_button_pressed(mast, BUTTON_UP)) {
        set_state(mast, STATE_RISING);
      } else if (was_button_pressed(mast, BUTTON_DOWN)) {
        set_state(mast, STATE_LOWERING);
      }
      break;
    case STATE_UP:
      if (was_button_pressed(mast, BUTTON_DOWN)) {
        set_state(mast, STATE_LOWERING);
      }
      break;
    case STATE_DOWN:
      if (was_button_pressed(mast, BUTTON_UP)) {
        set_state(mast, STATE_RISING);
      }
      break;
    case STATE_RISING:
      blink_led(mast, BUTTON_UP, total_ms);
      mast.state.state_data -= delta_ms;
      if (was_button_pressed(mast, BUTTON_UP) || was_button_pressed(mast, BUTTON_DOWN)) {
        set_state(mast, STATE_IDLE);
      } else if (mast.state.state_data <= 0) {
        set_state(mast, STATE_UP);
      }
      break;
    case STATE_LOWERING:
      blink_led(mast, BUTTON_DOWN, total_ms);
      mast.state.state_data -= delta_ms;
      if (was_button_pressed(mast, BUTTON_UP) || was_button_pressed(mast, BUTTON_DOWN)) {
        set_state(mast, STATE_IDLE);
      } else if (mast.state.state_data <= 0) {
        set_state(mast, STATE_DOWN);
      }
      break;
  }
}

void set_state(MastConfig &mast, int new_state) {
  switch (new_state) {
    case STATE_IDLE:
      digitalWrite(mast.pin_valve_up, LOW);
      digitalWrite(mast.pin_valve_down, LOW);
      digitalWrite(mast.pin_led_up, HIGH);
      digitalWrite(mast.pin_led_down, HIGH);
      mast.state.state_data = 0;
      Serial.println("Set state to IDLE");
      break;
    case STATE_RISING:
      digitalWrite(mast.pin_valve_up, HIGH);
      digitalWrite(mast.pin_valve_down, LOW);
      digitalWrite(mast.pin_led_down, LOW);
      mast.state.state_data = mast.raise_time_ms;
      Serial.println("Set state to RISING");
      break;
    case STATE_LOWERING:
      digitalWrite(mast.pin_valve_up, LOW);
      digitalWrite(mast.pin_valve_down, HIGH);
      digitalWrite(mast.pin_led_up, LOW);
      mast.state.state_data = mast.lower_time_ms;
      Serial.println("Set state to LOWERING");
      break;
    case STATE_UP:
      digitalWrite(mast.pin_valve_up, LOW);
      digitalWrite(mast.pin_valve_down, LOW);
      digitalWrite(mast.pin_led_up, HIGH);
      digitalWrite(mast.pin_led_down, LOW);
      Serial.println("Set state to UP");
      break;
    case STATE_DOWN:
      digitalWrite(mast.pin_valve_up, LOW);
      digitalWrite(mast.pin_valve_down, LOW);
      digitalWrite(mast.pin_led_up, LOW);
      digitalWrite(mast.pin_led_down, HIGH);
      Serial.println("Set state to DOWN");
      break;
  }

  mast.state.state_no = new_state;
}

bool was_button_pressed(MastConfig &mast, int direction) {
  MastState &state = mast.state;
  bool prev_state;
  if (direction == BUTTON_UP) {
    prev_state = state.button_up_state;
  } else if (direction == BUTTON_DOWN) {
    prev_state = state.button_down_state;
  }

  bool current_state;
  if (direction == BUTTON_UP) {
    current_state = digitalRead(mast.pin_button_up) == LOW;
  } else if (direction == BUTTON_DOWN) {
    current_state = digitalRead(mast.pin_button_down) == LOW;
  }

  bool pressed = prev_state && !current_state;
  if (direction == BUTTON_UP) {
    state.button_up_state = current_state;
  } else if (direction == BUTTON_DOWN) {
    state.button_down_state = current_state;
  }

  if (DEBUG) {
    Serial.println("Button state in mast " + String(mast.mastId) + ", direction " + String(direction) +
                   ": prev=" + prev_state +
                   ", current=" + current_state +
                   ", pressed=" + pressed);
  }

  if (pressed) {
    Serial.println("Detected button press on mast " + String(mast.mastId) + ", direction " + String(direction));
  }

  digitalWrite(LED_BUILTIN, current_state ? HIGH : LOW);
  return pressed;
}

void blink_led(MastConfig &mast, int direction, unsigned long total_ms) {
  int led_pin = (direction == BUTTON_UP) ? mast.pin_led_up : mast.pin_led_down;
  if ((total_ms / BLINK_INTERVAL_MS) % 2 == 0) {
    digitalWrite(led_pin, HIGH);
  } else {
    digitalWrite(led_pin, LOW);
  }
}

void initialize_mast(MastConfig config) {
  pinMode(config.pin_valve_up, OUTPUT);
  pinMode(config.pin_valve_down, OUTPUT);
  pinMode(config.pin_led_up, OUTPUT);
  pinMode(config.pin_led_down, OUTPUT);
  pinMode(config.pin_button_up, INPUT_PULLUP);
  pinMode(config.pin_button_down, INPUT_PULLUP);
}
