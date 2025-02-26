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
#include "canvas/canvas.h"
#include <opencv2/opencv.hpp>

void canvas_debug();

int main() {
    std::vector<Client> clients_exp;
    try {
        clients_exp = parse_config_throws("config.yaml");
    } catch (std::exception& ex) {
        std::cerr << "Error Parsing config file: " << ex.what() << "\n";
        exit(-1);
    }

    for (Client c : clients_exp) {
        std::cout << c.to_string() << "\n";
    }

    canvas_debug();
}

void canvas_debug() {
    //Create a virtual canvas
    VirtualCanvas vCanvas(cv::Size(3000, 3000));

    //Create elements (filepath, id, location)
    Element elem1("canvas/img.jpg", 1, cv::Point(100, 100));
    Element elem2("canvas/img2.jpg", 2, cv::Point(500, 700));


    // Add elements to the canvas
    vCanvas.addElementToCanvas(elem1);
    vCanvas.addElementToCanvas(elem2);



    cv::imshow("Canvas Before Removal", vCanvas.getPixelMatrix());
    cv::waitKey(0);

    // Remove one element
    vCanvas.removeElementFromCanvas(elem1);


    printf("Show removal\n");
    cv::imshow("Canvas After Removal", vCanvas.getPixelMatrix());
    cv::waitKey(0);
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
