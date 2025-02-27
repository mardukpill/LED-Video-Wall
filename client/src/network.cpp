#include "get_status.hpp"
#include "redraw.hpp"
#include "set_brightness.hpp"
#include "set_config.hpp"
#include "set_leds.hpp"
#include "network.hpp"
#include "protocol.hpp"
#include <Arduino.h>
#include <WiFi.h>
#include <cstddef>
#include <esp_wifi.h>

const char wifi_ssid[] = "UB_Devices";
const char wifi_password[] = "goubbulls";
const char *server_ip = "yoshi.cse.buffalo.edu"; // yoshi.cse.buffalo.edu
const uint16_t server_port = 7070;

WiFiClient socket;

void connect_wifi() {
  char wifi_ssid[] = "UB_Connect";
  char wifi_password[] = "goubbulls";
  WiFi.begin(wifi_ssid);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); // TODO: unhardcode delay
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

    uint32_t msg_size = 0;
    uint8_t *buffer = encode_check_in(mac, &msg_size);
    if (buffer) {
      socket.write(buffer, msg_size);
      Serial.println("Check-in message sent");
      free_message_buffer(buffer);
    } else {
      Serial.println("Failed to encode check-in message");
    }
  } else {
    Serial.println("Failed to connect to server");
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
    
    Serial.printf("Message size from header: %u bytes\n", messageSize);
    
    uint32_t remainingBytes = messageSize - sizeof(uint32_t);
    
    uint8_t* buffer = (uint8_t*)malloc(messageSize);
    if (!buffer) {
        Serial.println("Failed to allocate message buffer");
        return;
    }
    
    memcpy(buffer, sizeBuffer, sizeof(uint32_t));
    
    bytesRead = socket.read(buffer + sizeof(uint32_t), remainingBytes);
    
    Serial.printf("Read %d bytes of %u remaining\n", bytesRead, remainingBytes);
    
    if (bytesRead != remainingBytes) {
        Serial.println("Failed to read complete message");
        free(buffer);
        return;
    }
    
    uint16_t op_code = get_message_op_code(buffer);
    Serial.printf("Received OpCode: 0x%04X\n", op_code);
    
    // TODO: free structs after + reduce all these allocs
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
    
    free(buffer);
}
