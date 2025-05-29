#include "server_config.hpp"

#include <fstream>
#include <iostream>

ServerConfig::ServerConfig() {
  _configFile = "server_config.json";
  try {
    std::ifstream infile(_configFile, std::ifstream::in);
    if (infile) {
      infile >> _configJson;
    }
  } catch (std::exception& e) {
    throw std::runtime_error("Error reading config file " + _configFile + ": " +
    e.what());
  }
}

ServerConfig::~ServerConfig() {}

std::string ServerConfig::get(const std::string& context,
                              const std::string& key) const {
  auto value = _configJson[context].get(key, "").asString();
  return std::string{value.c_str()};
}
