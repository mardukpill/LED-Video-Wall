#include "tcp.hpp"
#include "client.hpp"
#include <asm-generic/socket.h>
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
#include "opencv2/core.hpp"
#include "protocol.hpp"

const int MAX_WAITING_CLIENTS = 256;

std::optional<LEDTCPServer> create_server(uint32_t addr,
                                          uint16_t start_port,
                                          uint16_t end_port) {
    struct protoent* protocol_entry = getprotobyname("tcp");
    const int tcp_protocol_num = protocol_entry->p_proto;
    
    int server_socket = socket(AF_INET, SOCK_STREAM, tcp_protocol_num);
    if (server_socket == -1) {
        std::cerr << "Bad socket!\n";
        return std::nullopt;
    }

    int enable = 1;
    setsockopt(server_socket, tcp_protocol_num, SO_REUSEPORT, &enable, sizeof(enable));

    uint16_t port;
    for (port = start_port; port <= end_port; port ++) {
        struct sockaddr_in s_addr;
        s_addr.sin_family = AF_INET;
        s_addr.sin_port = htons(port);
        s_addr.sin_addr.s_addr = addr;

        int res = bind(server_socket, (struct sockaddr *)&s_addr, sizeof(s_addr));
        if (res == -1) {
            std::cerr << "Failed bind: " << strerror(errno) << "\n";
            if (port == end_port) {
                std::cerr << "ERROR: could not bind to any of the ports in the range "
                          << start_port << " to "
                          << end_port << "\n";
                return std::nullopt;
            }
        } else {
            break;
        }
    }

    return LEDTCPServer(addr, port, server_socket);
}

LEDTCPServer::LEDTCPServer(uint32_t addr, uint16_t port, int socket)
    : addr(addr),
      port(port),
      socket(socket)
{}

void LEDTCPServer::wait_all_join(const std::vector<Client*> clients) {
    listen(this->socket, MAX_WAITING_CLIENTS);

    std::map<uint64_t, Client*> mac_to_client;
    for (Client* c : clients) {
        mac_to_client[c->mac_addr] = c;
    }

    // keeps track of the clients that have the appropriate mac address
    size_t accepted_clients = 0;
    while (accepted_clients < clients.size()) {
        int client_socket = accept(this->socket, NULL, NULL);
        uint8_t check_in_buf[32];
        recv(client_socket, check_in_buf, 12, 0);
        CheckInMessage* msg = decode_check_in(check_in_buf);
        uint64_t mac_addr;
        memcpy(&mac_addr, msg->mac_address, 6);

        // if c is the value representing the end of the iterator, it is not present
        std::cout << "Got message from " << mac_addr << "\n";
        if (!(mac_to_client.find(mac_addr) == mac_to_client.end())) {
            accepted_clients++;
            std::cout << "Accepted client\n";
            Client *c = mac_to_client.at(mac_addr);
            std::cout << "socket: " << client_socket << "\n";
            c->socket = client_socket;

            PinInfo info[1] = {(PinInfo){13, COLOR_ORDER_GRB, 8*32, LED_TYPE_WS2811}};
            const PinInfo* inf = info;
            uint32_t out_size;
            uint8_t* msg = encode_set_config(3, 10, 1, inf, &out_size);
            send(client_socket, msg, out_size, 0);
            std::cout << "Sent set_config to " << mac_addr << "\n";
        } else {
            close(client_socket);
        }
    }
}

void tcp_set_leds(int client_socket, const cv::Mat &cvmat, LEDMatrix* ledmat, uint8_t pin, uint8_t bit_depth) {
    uint32_t width = ledmat->spec->width;
    uint32_t height = ledmat->spec->height;
    uint32_t x = ledmat->pos.x;
    uint32_t y = ledmat->pos.y;
    rotation rot = ledmat->pos.rot;
    // swap width and height if rotated +/-90 degrees
    if (rot == LEFT || rot == RIGHT) {
        uint32_t temp = width;
        height = width;
        height = temp;
    }
    cv::Mat sub_cvmat = cvmat(cv::Rect(x, y, width, height)).clone();
    if (rot == LEFT) {
        cv::rotate(sub_cvmat, sub_cvmat, cv::ROTATE_90_CLOCKWISE);
    } else if (rot == RIGHT) {
        cv::rotate(sub_cvmat, sub_cvmat, cv::ROTATE_90_COUNTERCLOCKWISE);
    } else if (rot == DOWN) {
        cv::rotate(sub_cvmat, sub_cvmat, cv::ROTATE_180);
    }

    uint32_t array_size = ledmat->packed_pixel_array_size;
    uint32_t msg_size;
    SetLedsMessage* msg_buf = encode_fixed_set_leds(pin, bit_depth, array_size, &msg_size);
    uint8_t* pixel_buf = &(msg_buf->pixel_data[0]);
    const uint8_t* data = sub_cvmat.data;
    for (uint32_t i = 0; (i < ledmat->packed_pixel_array_size / 3); ++i) {
        uint32_t a = i * 3;
        if ((i / width) % 2 != 0) {
            pixel_buf[a + 2] = data[a] / 20;
            pixel_buf[a + 1] = data[a + 1] / 20;
            pixel_buf[a] = data[a + 2] / 20;
        } else {
            uint32_t irem = i % width;
            uint32_t b = (((width - 1) - irem) + (i - irem)) * 3;
            pixel_buf[a + 2] = data[b] / 20;
            pixel_buf[a + 1] = data[b + 1] / 20;
            pixel_buf[a] = data[b + 2] / 20;
        }
    }
    // uint32_t msg_size = ledmat->packed_pixel_array_size;
    int sent = send(client_socket, msg_buf, msg_size, 0);
    free_message_buffer(msg_buf);
    if (sent == -1) {
        std::cout << "Error sending set_leds: " << strerror(errno) << "\n";
    }
}
