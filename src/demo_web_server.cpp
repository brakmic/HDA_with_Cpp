#define FMT_HEADER_ONLY
#include <drogon/HttpAppFramework.h>
#include <fmt/format.h>

#include <argparse/argparse.hpp>
#include <iostream>
#include <string>

#include "server/server_config.hpp"
#include "database/db_mgr.hpp"

int main(int argc, char* argv[]) {
  ServerConfig config{};

  argparse::ArgumentParser program("demo_web_server", "0.0.1");

  program.add_argument("-i", "--ip-address")
      .help("Server IP Address")
      .default_value(std::string{""});

  program.add_argument("-p", "--port")
      .help("Port")
      .default_value<unsigned short>(0)
      .scan<'d', unsigned short>();

  program.parse_args(argc, argv);

  auto ip{program.get<std::string>("ip-address")};
  auto port{program.get<unsigned short>("port")};

  if (!ip.empty() && (port > 0)) {
    drogon::app().addListener(ip, port);
  }

  drogon::app().loadConfigFile("config.json");

  if (!DbManager::bootstrap(ServerConfig::instance().get("database", "file"),
                            "contacts.csv")) {
    std::cerr << "warning: database bootstrap failed; "
              << "ensure demo.db and contacts.csv exist" << std::endl;
  }

  // registerBeginningAdvice fires per thread, so we print here instead.
  std::cout << "web server listening on http://"
            << (ip.empty() ? "127.0.0.1" : ip)
            << ":"
            << (port > 0 ? port : 3000)
            << std::endl;

  drogon::app().run();
  return 0;
}