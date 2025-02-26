#include "protocol.hpp"
#include "set_config.hpp"
#include <Arduino.h>

void set_leds(SetLedsMessage *msg) {
  Serial.println("Handling set_leds");

  if (!msg) {
    Serial.println("Error: Invalid set_leds message (null)");
    return;
  }

  uint8_t gpio_pin = msg->gpio_pin;
  int strip_index = -1;

  for (int i = 0; i < num_pins; i++) {
    if (connected_pins[i] == gpio_pin) {
      strip_index = i;
      break;
    }
  }

  if (strip_index == -1) {
    Serial.println("Error: Received data for an unconfigured GPIO pin.");
    return;
  }

  CRGB *leds = led_buffers[strip_index];
  int num_pixels = num_leds_per_pin[strip_index];

  if (!leds || num_pixels == 0) {
    Serial.println("Error: LED buffer not allocated.");
    return;
  }

  uint32_t data_size = msg->header.size - sizeof(SetLedsMessage);
  int max_pixels = data_size / num_color_channels;
  num_pixels = min(num_pixels, max_pixels);

  uint8_t *pixel_data = msg->pixel_data;

  for (int i = 0; i < num_pixels; i++) {
    uint8_t r, g, b;
    if (num_color_channels == 3) {
      r = pixel_data[i * 3];
      g = pixel_data[i * 3 + 1];
      b = pixel_data[i * 3 + 2];
    } else {
      Serial.println("Error: Unsupported color channel configuration.");
      return;
    }
    leds[i] = CRGB(r, g, b);
  }
}
