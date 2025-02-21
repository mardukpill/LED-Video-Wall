#include "set_config.hpp"
#include "set_leds.hpp"

void set_leds(uint8_t *data, int length) {
  Serial.println("Handling set_leds");

  if (length < 7) {
    Serial.println("Error: Invalid set_leds packet length");
    return;
  }

  uint8_t gpio_pin = data[5];

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

  int max_pixels = (length - 7) / num_color_channels;
  num_pixels = min(num_pixels, max_pixels);

  uint8_t *pixel_data = &data[7];

  for (int i = 0; i < num_pixels; i++) {
    uint8_t r, g, b;

    if (num_color_channels == 3) { // RGB
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
