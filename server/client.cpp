#include "client.hpp"
#include <cstdint>
#include <regex>
#include <optional>
#include <string>

std::regex mac_regex("^[\\dA-F][\\dA-F](([\\dA-F][\\dA-F]){5}|([\\dA-F][\\dA-F]){7})$");
std::regex rot_regex("^(up|down|left|right)$");

rotation parse_rotation(std::string str) {
    std::smatch match;
    std::regex_search(str, match, rot_regex);
    std::string str_match = match[0].str();
    if (str_match == "up") {
        return UP;
    } else if (str_match == "down") {
        return DOWN;
    } else if (str_match == "left") {
        return LEFT;
    } else if (str_match == "right") {
        return RIGHT;
    }

    //todo error
}

CanvasPos::CanvasPos(uint32_t x,
                     uint32_t y,
                     rotation rot):
    x(x), y(y), rot(rot)
{}

LEDMatrix::LEDMatrix(std::string id,
                     std::string spec,
                     uint64_t num_leds,
                     CanvasPos pos):
    id(id),
    spec(spec),
    num_leds(num_leds),
    pos(pos)
{}

std::string LEDMatrix::to_string() {
    return this->id + ", " + this->spec + ", " + std::to_string(this->num_leds) +
        ", " + std::to_string(this->pos.x) + ", " + std::to_string(this->pos.x);
}

Client::Client(uint64_t mac_addr,
               int socket,
               std::vector<MatricesConnection> mat_connections):
    mac_addr(mac_addr),
    socket(socket),
    mat_connections(mat_connections)
{}

std::string Client::to_string() {
    return std::to_string(this->mac_addr) + ", " + std::to_string(this->socket);
}
