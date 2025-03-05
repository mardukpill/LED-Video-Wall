#include "canvas.h"
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>


std::vector<Element> parseInput(const std::string inputFile) {
    std::vector<Element> elementsVec;

    try {
        YAML::Node config = YAML::LoadFile(inputFile);
        YAML::Node elements = config["elements"];

        if (!elements) {
            std::cerr << "No elements found in config." << std::endl;
            return elementsVec;
        }

        for (YAML::const_iterator it = elements.begin(); it != elements.end(); ++it) {
            std::string key = it->first.as<std::string>();
            YAML::Node value = it->second;

            std::string type = value["type"].as<std::string>();
            int id = value["id"].as<int>();

            if (type == "image") {
                if (!value["filepath"] || !value["location"]) {
                    std::cerr << "Missing filepath or location for element: " << key << std::endl;
                    abort();
                }

                std::string filepath = value["filepath"].as<std::string>();
                std::vector<int> locVec = value["location"].as<std::vector<int>>();

                //Checks to ensure location vector is of the expected form + init openCV point
                if ((locVec.size() != 2) || (locVec.at(0) < 0) || (locVec.at(1) < 0)) {
                    std::cerr << "Location for element " << key << " malformed." << std::endl;
                    abort();
                }
                cv::Point loc(locVec.at(0), locVec.at(1));

                //ELEMENT CREATION HERE
                Element elem(filepath, id, loc);
                elementsVec.push_back(elem);
            }
            else {
                std::cerr << "Unsupported element type: " << type << std::endl;
            }
        }
    } catch (const YAML::Exception& e) {
        std::cerr << "Error parsing config: " << e.what() << std::endl;
        abort();
    }

    return elementsVec;
}