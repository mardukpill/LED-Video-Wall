#include "tcp.hpp"
#include "client.hpp"
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <optional>
#include <string.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <map>

const int MAX_WAITING_CLIENTS = 256;

std::optional<LEDTCPServer> create_server(uint32_t addr, uint16_t port) {
    struct protoent* protocol_entry = getprotobyname("tcp");
    const int tcp_protocol_num = protocol_entry->p_proto;
    
    int server_socket = socket(AF_INET, SOCK_STREAM, tcp_protocol_num);
    if (server_socket == -1) {
        std::cerr << "Bad socket!\n";
        return std::nullopt;
    }

    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(port);
    s_addr.sin_addr.s_addr = addr;

    int res = bind(server_socket, (struct sockaddr *)&s_addr, sizeof(s_addr));
    if (res == -1) {
        std::cerr << "Failed bind: " << strerror(errno) << "\n";
        return std::nullopt;
    }

    return LEDTCPServer(addr, port, server_socket);
}

LEDTCPServer::LEDTCPServer(uint32_t addr, uint16_t port, int socket)
    : addr(addr),
      port(port),
      socket(socket)
{}

void LEDTCPServer::wait_all_join(const std::vector<Client> clients) {
    listen(this->socket, MAX_WAITING_CLIENTS);

    std::map<uint64_t, Client> mac_to_client;
    for (Client c : clients) {
        mac_to_client[c.mac_addr] = c;
    }

    // keeps track of the clients that have the appropriate mac address
    int accepted_clients = 0;
    while (accepted_clients < clients.size()) {
        int client_socket = accept(this->socket, NULL, NULL);
        char check_in_buf[9];
        recv(client_socket, check_in_buf, 12, 0);
        uint16_t op_code;
        std::memcpy(&op_code, &check_in_buf + 4, 2);
        uint64_t mac_addr;
        std::memcpy(&mac_addr, &check_in_buf + 6, 6);

        auto c = mac_to_client.find(mac_addr); // c is an iterator
        // if c is the value representing the end of the iterator, it is not present
        if (!(mac_to_client.find(mac_addr) == mac_to_client.end())) {
            // todo
        }
    }
}
