#define FMT_HEADER_ONLY
#include <drogon/HttpAppFramework.h>
#include <fmt/format.h>

#include <argparse/argparse.hpp>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  argparse::ArgumentParser program("demo_web_server", "0.0.1");

  program.add_argument("-i", "--ip-address")
      .help("Server IP Address")
      .default_value<std::string>({"127.0.0.1"});

  program.add_argument("-p", "--port")
      .help("Port")
      .default_value<unsigned int>(3000)
      .scan<'d', unsigned int>();

  program.parse_args(argc, argv);

  auto ip{program.get<std::string>("ip-address")};
  auto port{program.get<unsigned int>("port")};

  drogon::app().addListener(ip, port);
  std::cout << "web server listening on "
            << fmt::format("http://{}:{}", ip, port) << std::endl;
  drogon::app().run();
  return 0;
}