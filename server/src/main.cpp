#include <cstdio>
#include <exception>
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
#include <vector>
#include "canvas.h"
#include <opencv2/opencv.hpp>

int main() {
    //Create a virtual canvas
    VirtualCanvas vCanvas(cv::Size(8, 32));

    std::vector<Client*> clients_exp;
    try {
        clients_exp = parse_config_throws("config.yaml");
    } catch (std::exception& ex) {
        std::cerr << "Error Parsing config file: " << ex.what() << "\n";
        exit(-1);
    }

    for (Client* c : clients_exp) {
        std::cout << c->to_string() << "\n";
    }

    Element elem1("images/img5x5_1.jpg", 1, cv::Point(0, 0));
    vCanvas.addElementToCanvas(elem1);

    std::optional<LEDTCPServer> server_opt = create_server(INADDR_ANY, 7070);
    if (!server_opt.has_value()) {
        exit(-1);
    }
    LEDTCPServer server = server_opt.value();

    server.wait_all_join(clients_exp);

    int pos = 0;
    while(1) {
        for (Client* c : clients_exp) {
            vCanvas.removeElementFromCanvas(elem1);
            if (pos > 25) {
                pos = 0;
            } else {
                pos++;
            }
            elem1.setLocation(cv::Point(0, pos));
            vCanvas.addElementToCanvas(elem1);
            c->set_leds_all_matrices(vCanvas.getPixelMatrix());
        }
    }
}

