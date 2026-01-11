#include "config.h"

#define SLEEP_DELAY_MS 10

Mast** masts;
int mastCount;
unsigned long prevTime = 0;

void setup() {
  mastCount = sizeof(mastConfigs) / sizeof(MastConfig);
  masts = new Mast*[mastCount];
  for (int i = 0; i < mastCount; ++i) {
    masts[i] = new Mast(mastConfigs[i]);
    masts[i]->initialize();
  }

  prevTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  int deltaMs = static_cast<int>(currentTime - prevTime);

  for (int i = 0; i < mastCount; ++i) {
    masts[i]->tick(deltaMs);
  }

  prevTime = currentTime;
  delay(SLEEP_DELAY_MS);
}
