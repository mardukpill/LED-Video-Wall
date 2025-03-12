#include "led_strip.h"
#include "protocol.hpp"
#include <Arduino.h>

void set_brightness(SetBrightnessMessage *msg) {
  if (msg == NULL) {
    Serial.println("Error: Null message pointer");
    return;
  }

  Serial.println("Handling change_brightness");

  uint16_t brightness = msg->brightness;

  if (brightness > 255) {
    brightness = 255;
  }

  // TODO: needs to set the rgb color values
  Serial.printf("Brightness set to: %d\n", brightness);
}
