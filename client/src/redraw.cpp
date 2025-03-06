#include "led_strip.h"
#include "protocol.hpp"
#include "redraw.hpp"
#include "set_config.hpp"
#include <Arduino.h>

void redraw(RedrawMessage *msg) {
  Serial.println("Handling redraw");

  if (pin_to_handle.empty()) {
    Serial.println("Error: No LED strips configured.");
    return;
  }

  for (auto &entry : pin_to_handle) {
    led_strip_refresh(entry.second);
  }

  Serial.println("LEDs refreshed across all configured pins.");
}
