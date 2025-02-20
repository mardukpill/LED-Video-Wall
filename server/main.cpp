#include <cstdio>
#include <iostream>
#include <iterator>
#include <string.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h> // for close
#include "tcp.hpp"
#include "client.hpp"
#include "config-parser.hpp"

int main() {
    parse_config("config.yaml");
}

void start_server() {
    int server_socket = create_server_socket(INADDR_ANY, 7070);

    listen(server_socket, 1);

    int client_socket = accept(server_socket, NULL, NULL);

    char buf[256];
    recv(client_socket, buf, 256, 0);
    buf[255] = '\0';
    printf(buf, "%s");
    while (true) {
        std::string msg = "This is a test!!!";
        send(client_socket, msg.c_str(), msg.size(), 0);
    }

    close(server_socket);
}
