#include "FastLED.h"
#include "set_brightness.hpp"
#include <Arduino.h>

void set_brightness(uint8_t *data, int length) {
  Serial.println("Handling change_brightness");

  if (length < 9) {
    Serial.println("Error: Invalid set_brightness packet length");
    return;
  }

  uint16_t brightness = (data[8] << 8) | data[9];

  if (brightness > 255) {
    brightness = 255;
  }

  FastLED.setBrightness(brightness);

  Serial.printf("Brightness set to: %d\n", brightness);
}
