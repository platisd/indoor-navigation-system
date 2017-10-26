#include <stdint.h>

const uint8_t WIFI_MODULE_PIN = PB2;

uint8_t turnWifi(uint8_t state) {
  uint8_t error = 0;
  digitalWrite(WIFI_MODULE_PIN, state);
  return error;
}

void setup() {
  // Setup pins
  pinMode(WIFI_MODULE_PIN, OUTPUT);
  turnWifi(HIGH);
}

void loop() {}
