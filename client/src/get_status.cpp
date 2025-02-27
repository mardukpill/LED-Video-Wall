#include "protocol.hpp"
#include "get_status.hpp"
#include <Arduino.h>

void get_status(GetStatusMessage *msg) {
  Serial.println("Handling get_status");
  send_status();
}

void send_status() {}
