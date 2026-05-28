#include "leaf.hpp"
#include "httpserver.h"
#include <cstdlib>

int main(int argc, char *argv[]) {
  auto server_ex = boost::leaf::try_handle_all(
    []() ->  boost::leaf::result<sws::HttpServer> {
      return sws::HttpServer::Create("127.0.0.1", 8080);
    },
    [](std::error_code err_code) -> sws::HttpServer {
       std::cerr << "Failed to start server: " << err_code.message()
              << std::endl;
       exit(EXIT_FAILURE);
    },
    []() -> sws::HttpServer {
      std::cerr << "Failed to start server"
              << std::endl;
      exit(EXIT_FAILURE);
    });
  server_ex.Run();
  return EXIT_SUCCESS;
}