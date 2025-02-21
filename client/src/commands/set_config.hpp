#ifndef SET_CONFIG_H
#define SET_CONFIG_H

#include <Arduino.h>
#include <FastLED.h>

extern uint8_t num_color_channels;
extern uint8_t bit_depth;
extern uint8_t num_pins;
extern uint8_t *connected_pins;
extern uint16_t *num_leds_per_pin;
extern CRGB **led_buffers;
extern uint8_t color_order;

void set_config(uint8_t *data, int length);
void free_led_buffers();

#endif
