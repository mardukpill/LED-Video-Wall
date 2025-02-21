#include "commands/get_status.hpp"
#include "commands/redraw.hpp"
#include "commands/set_brightness.hpp"
#include "commands/set_config.hpp"
#include "commands/set_leds.hpp"
#include "network.hpp"
#include <cstddef>
#include <esp_wifi.h>

const char *wifi_ssid = "UB_Connect";
const char *wifi_password = NULL;
const char *server_ip = "128.205.44.25"; // yoshi.cse.buffalo.edu
const uint16_t server_port = 7070;

WiFiClient socket;

void connect_wifi() {
  Serial.begin(115200);
  WiFi.begin((char *)wifi_ssid, wifi_password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); // TODO: unhardcore
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  send_checkin();
}

void send_checkin() {
  Serial.println("Sending check-in message");

  if (socket.connect(server_ip, server_port)) {
    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac);

    uint16_t msg_size = 4 + 1 + 6;
    uint8_t buffer[msg_size];
    memset(buffer, 0, msg_size);

    buffer[0] = (msg_size >> 24) & 0xFF;
    buffer[1] = (msg_size >> 16) & 0xFF;
    buffer[2] = (msg_size >> 8) & 0xFF;
    buffer[3] = msg_size & 0xFF;

    buffer[4] = 0x07;

    memcpy(&buffer[5], mac, 6);

    socket.write(buffer, msg_size);
    Serial.println("Check-in message sent");
  } else {
    Serial.println("Failed to connect to server");
  }
}

void parse_tcp_message() {
  uint8_t buffer[256];
  int length = socket.read(buffer, sizeof(buffer));
  if (length <= 0)
    return;

  uint8_t op_code = buffer[4];
  Serial.printf("Received OpCode: 0x%02X\n", op_code);

  switch (op_code) {
  case 0x01:
    set_leds(buffer, length);
    break;
  case 0x02:
    get_status();
    break;
  case 0x03:
    set_brightness(buffer, length);
    break;
  case 0x04:
    redraw();
    break;
  case 0x05:
    set_config(buffer, length);
    break;
  default:
    Serial.println("Unknown OpCode");
    break;
  }
}
