#pragma once
#include <json/json.h>

class ServerConfig {
 public:
  ServerConfig();
  ~ServerConfig();
  const Json::Value& to_json() const { return _configJson; }
  std::string get(const std::string& context, const std::string& key) const;
  inline static ServerConfig& instance() {
    static ServerConfig instance;
    return instance;
  }

 private:
  std::string _configFile;
  Json::Value _configJson;
};
