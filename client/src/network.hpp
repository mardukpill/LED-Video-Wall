#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>

extern WiFiClient socket;

void connect_wifi();
void send_checkin();
void parse_tcp_message();

#endif
