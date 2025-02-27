#include "protocol.hpp"
#include "FastLED.h"
#include "set_config.hpp"
#include <Arduino.h>

uint8_t num_color_channels;
uint8_t bit_depth;
uint8_t num_pins = 0;
uint8_t *connected_pins = nullptr;
uint16_t *num_leds_per_pin = nullptr;
CRGB **led_buffers = nullptr;
uint8_t color_order = GRB;

void free_led_buffers() {
  if (led_buffers) {
    for (int i = 0; i < num_pins; i++) {
      if (led_buffers[i]) {
        free(led_buffers[i]);
      }
    }
    free(led_buffers);
    led_buffers = nullptr;
  }

  if (connected_pins) {
    free(connected_pins);
    connected_pins = nullptr;
  }

  if (num_leds_per_pin) {
    free(num_leds_per_pin);
    num_leds_per_pin = nullptr;
  }
}

void set_config(SetConfigMessage *msg) {
  if (msg == NULL) {
    Serial.println("Error: Null set_config message pointer");
    return;
  }

  Serial.println("Handling set_config");

  free_led_buffers();

  num_color_channels = msg->num_color_channels;
  uint16_t brightness = msg->init_brightness;
  num_pins = msg->pins_used;

  if (num_pins == 0) {
    Serial.println("Error: num_pins cannot be zero");
    return;
  }

  connected_pins = (uint8_t *)malloc(num_pins * sizeof(uint8_t));
  num_leds_per_pin = (uint16_t *)malloc(num_pins * sizeof(uint16_t));
  led_buffers = (CRGB **)malloc(num_pins * sizeof(CRGB *));

  if (!connected_pins || !num_leds_per_pin || !led_buffers) {
    Serial.println("Error: Memory allocation failed");
    free_led_buffers();
    return;
  }

  for (int i = 0; i < num_pins; i++) {
    PinInfo *pinfo = &msg->pin_info[i];
    connected_pins[i] = pinfo->pin_num;
    num_leds_per_pin[i] = (uint16_t)pinfo->max_leds;

    if (i == 0) {
      color_order = pinfo->color_order;
    }

    if (num_leds_per_pin[i] > 0) {
      led_buffers[i] = (CRGB *)malloc(num_leds_per_pin[i] * sizeof(CRGB));
      if (!led_buffers[i]) {
        Serial.println("Error: Failed to allocate LED buffer!");
        free_led_buffers();
        return;
      }

      // TODO: this is pretty bad
      switch (connected_pins[i]) {
      case 1:
        FastLED.addLeds<WS2811, 1, GRB>(led_buffers[i], num_leds_per_pin[i]);
        break;
      case 2:
        FastLED.addLeds<WS2811, 2, GRB>(led_buffers[i], num_leds_per_pin[i]);
        break;
      case 3:
        FastLED.addLeds<WS2811, 3, GRB>(led_buffers[i], num_leds_per_pin[i]);
        break;
      case 4:
        FastLED.addLeds<WS2811, 4, GRB>(led_buffers[i], num_leds_per_pin[i]);
        break;
      case 5:
        FastLED.addLeds<WS2811, 5, GRB>(led_buffers[i], num_leds_per_pin[i]);
        break;
      case 13:
        FastLED.addLeds<WS2811, 13, GRB>(led_buffers[i], num_leds_per_pin[i]);
        break;
      default:
        Serial.println("Error: Unsupported GPIO pin (not hardcoded yet)");
        return;
      }
    } else {
      led_buffers[i] = nullptr;
    }
  }

  FastLED.setBrightness(brightness);

  Serial.println("Configuration updated.");
}
