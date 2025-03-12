#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <string>
#include <vector>
#include "client.hpp"

std::pair<std::vector<Client *>, cv::Size> parse_config_throws(std::string file);

#endif
