#define FMT_HEADER_ONLY
#include <drogon/HttpAppFramework.h>
#include <fmt/format.h>

#include <argparse/argparse.hpp>
#include <iostream>
#include <string>

#include "../server/server_config.hpp"

/// @brief Will be executed after drogon::app().run().
/// more info:
/// https://github.com/drogonframework/drogon-docs/blob/master/ENG-12-AOP-Aspect-Oriented-Programming.md
void displayNetworkInfo() {
  auto listeners = drogon::app().getListeners();

  for (const auto& l : listeners) {
    std::cout << "web server listening on "
              << fmt::format("http://{}:{}", l.toIp(), l.toIpPort())
              << std::endl;
  }
}

int main(int argc, char* argv[]) {
  ServerConfig config{};

  argparse::ArgumentParser program("demo_web_server", "0.0.1");

  program.add_argument("-i", "--ip-address")
      .help("Server IP Address")
      .default_value(std::string{""});

  program.add_argument("-p", "--port")
      .help("Port")
      .default_value<unsigned int>(0)
      .scan<'d', unsigned int>();

  program.parse_args(argc, argv);

  auto ip{program.get<std::string>("ip-address")};
  auto port{program.get<unsigned int>("port")};

  if (!ip.empty() && (port > 0)) {
    drogon::app().addListener(ip, port);
  }

  drogon::app().loadConfigFile("config.json");

  drogon::app().registerBeginningAdvice(displayNetworkInfo);

  drogon::app().run();
  return 0;
}