#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>

// TODO: define esp-idf configs
#define WIFI_RECONNECT_DELAY_MS 500

#define SERVER_IP "yoshi.cse.buffalo.edu"
#define SERVER_PORT_START 7070
#define SERVER_PORT_END 7074

#define CHECK_IN_DELAY_MS 500

extern WiFiClient socket;

void connect_wifi();
void send_checkin();
void parse_tcp_message();

#endif
