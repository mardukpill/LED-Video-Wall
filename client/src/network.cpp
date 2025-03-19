#include "get_status.hpp"
#include "network.hpp"
#include "protocol.hpp"
#include "redraw.hpp"
#include "set_brightness.hpp"
#include "set_config.hpp"
#include "set_leds.hpp"
#include <Arduino.h>
#include <WiFi.h>
#include <cstddef>
#include <cstdint>
#include <esp_wifi.h>


// There should be a separate header file, "wifi_credentials.hpp", that defines
// preprocessor constants, WIFI_SSID and WIFI_PASSWORD, that are used by the
// clients to connect to wifi.
// !!! THE CREDENTIALS FILE MUST _NOT_ BE ADDDED TO GIT/SOURCE CONTROL !!!
// Here is an example of what "wifi_credentials.hpp" look like:
// -----------------------------------------------------------
// #ifndef CREDENTIALS_HPP
// #define CREDENTIALS_HPP
// #define WIFI_SSID "UB_Connect"
// #define WIFI_PASSWORD ""
// #endif
// -----------------------------------------------------------
#if __has_include("wifi_credentials.hpp")
#include "wifi_credentials.hpp"
#else
#warning "'wifi_credentials.hpp' not specified! Using default credentials."
#define WIFI_SSID "UB_Connect"
#define WIFI_PASSWORD ""
#endif

WiFiClient socket;

// TODO: you might be able to auto set wifi in esp-idf settings
void connect_wifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_RECONNECT_DELAY_MS);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  send_checkin();
}

void send_checkin() {
  Serial.println("Sending check-in message");

  // When attempting to connect to the server, there are multiple ports that it
  // may have connected to. The preprocessor constants SERVER_PORT_START and
  // SERVER_PORT_END denote the range of ports, start and end included, that the
  // server may be using. The code below attempts to connect on all the ports in
  // the range, and simply returns if none of the ports resulted in a successful
  // connection. If a connection does succeed then the loop ends early and
  // continues with the rest of the code.
  for (uint16_t port = SERVER_PORT_START; port <= SERVER_PORT_END; port++) {
    if (socket.connect(SERVER_IP, port)) {
      break;
    } else {
      Serial.print("Tried and failed to connect to server port: ");
      Serial.print(port);
      Serial.println("; trying a different port.");
      if (port == SERVER_PORT_END) {
        Serial.println("Failed to connect to any server port");
        return;
      }
    }
  }
  uint8_t mac[6];
  esp_wifi_get_mac(WIFI_IF_STA, mac);

  uint32_t msg_size = 0;
  uint8_t *buffer = encode_check_in(mac, &msg_size);
  if (buffer) {
    socket.write(buffer, msg_size);
    Serial.println("Check-in message sent");
    free_message_buffer(buffer);
  } else {
    Serial.println("Failed to encode check-in message");
  }
}

void parse_tcp_message() {
  uint8_t sizeBuffer[sizeof(uint32_t)];
  int bytesRead = socket.read(sizeBuffer, sizeof(sizeBuffer));

  if (bytesRead < sizeof(uint32_t)) {
    Serial.println("Failed to read message size");
    return;
  }

  uint32_t messageSize = get_message_size(sizeBuffer);

  Serial.printf("Message size from header: %u bytes\n",
                (unsigned int)messageSize);

  uint32_t remainingBytes = messageSize - sizeof(uint32_t);

  uint8_t *buffer = (uint8_t *)malloc(messageSize);
  if (!buffer) {
    Serial.println("Failed to allocate message buffer");
    return;
  }

  memcpy(buffer, sizeBuffer, sizeof(uint32_t));

  bytesRead = socket.read(buffer + sizeof(uint32_t), remainingBytes);

  Serial.printf("Read %d bytes of %u remaining\n", bytesRead,
                (unsigned int)remainingBytes);

  if (bytesRead != remainingBytes) {
    Serial.println("Failed to read complete message");
    free(buffer);
    return;
  }

  uint16_t op_code = get_message_op_code(buffer);
  Serial.printf("Received OpCode: 0x%04X\n", op_code);

  switch (op_code) {
  case OP_SET_LEDS: {
    SetLedsMessage *msg = decode_set_leds(buffer);
    if (msg) {
      set_leds(msg);
    }
    break;
  }
  case OP_GET_STATUS: {
    GetStatusMessage *msg = decode_get_status(buffer);
    if (msg) {
      get_status(msg);
    }
    break;
  }
  case OP_SET_BRIGHTNESS: {
    SetBrightnessMessage *msg = decode_set_brightness(buffer);
    if (msg) {
      set_brightness(msg);
    }
    break;
  }
  case OP_REDRAW: {
    RedrawMessage *msg = decode_redraw(buffer);
    if (msg) {
      redraw(msg);
    }
    break;
  }
  case OP_SET_CONFIG: {
    SetConfigMessage *msg = decode_set_config(buffer);
    if (msg) {
      set_config(msg);
    }
    break;
  }
  default:
    Serial.printf("Unknown OpCode: 0x%02X\n", op_code);
    break;
  }

  free_message_buffer(buffer);
}
