#include <cstdio>
#include <iostream>
#include <iterator>
#include <string.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h> // for close


int main() {
    struct protoent* protocol_entry = getprotobyname("tcp");
    const int tcp_protocol_num = protocol_entry->p_proto;
    
    int server_socket = socket(AF_INET, SOCK_STREAM, tcp_protocol_num);
    if (server_socket == -1) {
        std::cerr << "Bad socket!\n";
        exit(-1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1024);
    addr.sin_addr.s_addr = INADDR_ANY;

    int res = bind(server_socket, (struct sockaddr *)&addr, sizeof(addr));
    if (res == -1) {
        std::cerr << "Failed bind: " << strerror(errno) << "\n";
        exit(-1);
    }

    listen(server_socket, 1);

    int client_socket = accept(server_socket, NULL, NULL);

    // std::string msg = "This is a test!!!";
    // send(client_socket, msg.c_str(), msg.size(), 0);
    char buf[256];
    recv(client_socket, buf, 256, 0);
    buf[255] = '\0';
    printf(buf, "%s");

    close(server_socket);
}
