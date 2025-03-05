#include "network.hpp"
#include <Arduino.h>

void setup() {
  Serial.begin(460800);
  connect_wifi();
}

void loop() {
  if (!socket.connected()) {
    Serial.println("Reconnecting to server...");
    send_checkin();
    delay(CHECK_IN_DELAY_MS);
    return;
  }

  if (socket.available()) {
    parse_tcp_message();
  }
}
