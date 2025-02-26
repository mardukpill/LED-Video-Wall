#ifndef TCP_H
#define TCP_H

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <string.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h> // for close
#include <vector>
#include "client.hpp"

int create_server_socket(uint32_t addr, uint16_t port);

class LEDTCPServer {
public:
    uint32_t addr;
    uint16_t port;
    int socket;

    LEDTCPServer(uint32_t addr, uint16_t port, int socket);
    void wait_all_join(std::vector<Client> clients);
};

#endif
