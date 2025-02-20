#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <optional>

enum rotation { UP, DOWN, LEFT, RIGHT };

rotation parse_rotation(std::string str);

class CanvasPos {
public:
    uint32_t x;
    uint32_t y;
    rotation rot;

    CanvasPos(uint32_t x,
              uint32_t y,
              rotation rot);
};

class LEDMatrix {
public:
    std::string id;
    std::string spec;
    uint64_t num_leds;
    CanvasPos pos;

    LEDMatrix(std::string id,
              std::string spec,
              uint64_t num_leds,
              CanvasPos pos);

    std::string to_string();
};

class MatricesConnection {
public:
    uint8_t pin;
    std::vector<LEDMatrix*> matrices;
};

class Client {
public:
    uint64_t mac_addr;
    int socket;
    std::vector<MatricesConnection> mat_connections;

    Client(uint64_t mac_addr,
           int socket,
           std::vector<MatricesConnection> mat_connections);

    std::string to_string();
};

#endif
