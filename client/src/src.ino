#include "network.hpp"
#include <Arduino.h>

void setup() {
  Serial.begin(460800);
  connect_wifi();
}

void loop() {
  delay(1000);
  if (!socket.connected()) {
    Serial.println("Reconnecting to server...");
    send_checkin();
    delay(5000); // TODO: unhardcore
    return;
  }

  if (socket.available()) {
    parse_tcp_message();
  }
}
