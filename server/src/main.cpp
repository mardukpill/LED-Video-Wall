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
#include "input-parser.hpp"
#include <opencv2/opencv.hpp>

int main() {
    //Create a virtual canvas
    VirtualCanvas vCanvas(cv::Size(16, 32));

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

    std::optional<LEDTCPServer> server_opt = create_server(INADDR_ANY, 7070, 7074);
    if (!server_opt.has_value()) {
        exit(-1);
    }
    LEDTCPServer server = server_opt.value();

    server.wait_all_join(clients_exp);

    int x = 0;
    int y = 0;
    int dx = 1;
    int dy = 1;
    while(1) {
        for (Client* c : clients_exp) {
            vCanvas.removeElementFromCanvas(elem1);
            if (x >= 11) {
                dx = -1;
            } else if (x <= 0) {
                dx = 1;
            }
            if (y >= 26) {
                dy = -1;
            } else if (y <= 0) {
                dy = 1;
            }
            x += dx;
            y += dy;
            elem1.setLocation(cv::Point(x, y));
            vCanvas.addElementToCanvas(elem1);
            c->set_leds_all_matrices(vCanvas.getPixelMatrix());
        }
        // usleep(33333); // ~30 fps
        // usleep(100000); // 10 fps
        usleep(200000); // 5 fps
        // usleep(250000); // 4 fps
    }

    // std::string inputFilePath = "input.yaml";
    // VirtualCanvas vCanvas(cv::Size(2000, 2000));
    // canvas_debug(vCanvas, inputFilePath);
    // return 0;
}

void canvas_debug(VirtualCanvas& vCanvas, std::string inputFilePath) {

    std::vector<Element> elementsVec = parseInput(inputFilePath);
    vCanvas.addElementVecToCanvas(elementsVec);

    cv::imshow("Display Cats", vCanvas.getPixelMatrix());
    cv::waitKey(0);


    /*
    //Create elements (filepath, id, location)
    Element elem1("images/img.jpg", 1, cv::Point(0, 0));
    vCanvas.addElementToCanvas(elem1);

    cv::imshow("Display Cat 1", vCanvas.getPixelMatrix());
    cv::waitKey(0);

    Element elem2("images/img2.jpg", 1, cv::Point(1700, 69));
    vCanvas.addElementToCanvas(elem2);

    cv::imshow("Display Cat 2", vCanvas.getPixelMatrix());
    cv::waitKey(0);
    
    
    */

    
}
