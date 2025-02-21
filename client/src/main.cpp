#include "network.hpp"

void setup() { connect_wifi(); }

void loop() {
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
