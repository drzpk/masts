#include "config.h"

#define SLEEP_DELAY_MS 10

unsigned long prevTime = 0;

void setup() {
  for (int i = 0; i < componentCount; ++i) {
    components[i]->initialize();
  }

  prevTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  int deltaMs = static_cast<int>(currentTime - prevTime);

  for (int i = 0; i < componentCount; ++i) {
    components[i]->tick(deltaMs);
  }

  prevTime = currentTime;
  delay(SLEEP_DELAY_MS);
}
