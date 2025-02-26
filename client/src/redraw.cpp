#include "protocol.hpp"
#include "redraw.hpp"
#include "set_config.hpp"
#include <Arduino.h>
#include <FastLED.h>

void redraw(RedrawMessage *msg) {
  Serial.println("Handling redraw");

  if (!led_buffers || num_pins == 0) {
    Serial.println("Error: No LED buffers configured.");
    return;
  }

  FastLED.show();

  Serial.println("LEDs updated across all configured pins.");
}
