#include "get_status.hpp"
#include "network.hpp"
#include <Arduino.h>

void get_status() {
  Serial.println("Handling get_status");
  send_status();
}

void send_status() {
  Serial.println("Handling send_status");

  const char *debug_msg = "Device running normally";
  uint16_t msg_size = 4 + 1 + strlen(debug_msg) + 1;

  uint8_t buffer[msg_size];
  memset(buffer, 0, msg_size);

  buffer[0] = (msg_size >> 24) & 0xFF;
  buffer[1] = (msg_size >> 16) & 0xFF;
  buffer[2] = (msg_size >> 8) & 0xFF;
  buffer[3] = msg_size & 0xFF;

  buffer[4] = 0x06;

  strcpy((char *)&buffer[5], debug_msg);

  socket.write(buffer, msg_size);
  Serial.println("Status message sent");
}
