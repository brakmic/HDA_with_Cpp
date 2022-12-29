#include <drogon/HttpAppFramework.h>
#include <fmt/core.h>

#include <iostream>
#include <string>

int main() {
  uint port{3000};
  std::string server{"127.0.0.1"};
  drogon::app().addListener(server, port);
  std::cout << "web server listening on "
            << fmt::format("http://{}:{}", server, port) << std::endl;
  drogon::app().run();
  return 0;
}