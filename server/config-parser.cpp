#include "client.hpp"
#include <cstdint>
#include <yaml-cpp/node/detail/iterator_fwd.h>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <regex>

std::regex mac_48_regex(
    "^[\\dA-F][\\dA-F](([\\dA-F][\\dA-F]){5}|([\\dA-F][\\dA-F]){5})$");

uint8_t parse_hex(char c) {
    if (c <= '9') {
        return c - '0';
    } else {
        return c - 'A' + 10;
    }
}

uint64_t parse_mac_addr_48_bit(std::string str) {
    std::smatch match;
    std::regex_search(str, match, mac_48_regex);
    if (match.empty()) {
        //todo error
    }
    uint64_t res = 0;
    for (int i = 0; i < 6; ++i) {
        int offset = i * 3;
        res <<= 4;
        res += parse_hex(str[offset]);
        res <<= 4;
        res += parse_hex(str[offset + 1]);
    }

    return res;
}

void parse_config(std::string file) {
    YAML::Node config = YAML::LoadFile(file);

    YAML::Node y_clients = config["clients"];
    YAML::Node y_matrices = config["matrices"];
    YAML::Node y_matrix_specs = config["matrix-specs"];

    // Parse Matrices
    std::map<std::string, LEDMatrix*> matrices;
    // std::vector<LEDMatrix> matrices;
    for (YAML::const_iterator it=y_matrices.begin(); it!=y_matrices.end(); ++it) {
        std::string id = it->first.as<std::string>();
        YAML::Node matrix_node = it->second;

        std::string spec = matrix_node["spec"].as<std::string>();

        std::uint64_t num_leds = 0; //todo: calc leds from specs
        
        YAML::Node pos_node = matrix_node["pos"];
        if (pos_node.size() != 2) {
            //todo error
        }
        rotation rot = parse_rotation(matrix_node["rot"].as<std::string>());
        CanvasPos pos = CanvasPos(pos_node[0].as<int>(),
                                  pos_node[1].as<int>(),
                                  rot);
        LEDMatrix* mat = new LEDMatrix(id, spec, num_leds, pos);
        std::cout << mat->to_string() << "\n";
        matrices[id] = mat;
    }

    // Parse Clients
    std::vector<Client> clients;
    for (YAML::const_iterator it=y_clients.begin(); it!=y_clients.end(); ++it) {
        uint64_t mac_addr = parse_mac_addr_48_bit(it->first.as<std::string>());

        YAML::Node connections_node = it->second["matrix-connections"];

        // Parse Matrix Connections
        std::vector<MatricesConnection> mat_connections;
        for (size_t i = 0; i < connections_node.size(); ++i) {
            YAML::Node connection_node = connections_node[i];
            uint8_t pin = connection_node["pin"].as<uint8_t>();
            MatricesConnection conn;
            conn.pin = pin;
            YAML::Node matrices_node = connection_node["matrices"];
            for (size_t j = 0; j < matrices_node.size(); ++j) {
                std::string id = matrices_node[j].as<std::string>();
                conn.matrices.push_back(matrices[id]);
            }
            mat_connections.push_back(conn);
        }

        Client c(mac_addr, -1, mat_connections);
        std::cout << c.to_string() << "\n";
        clients.push_back(c);
    }
}
