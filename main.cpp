#include "leaf.hpp"
#include "simpletcpserver.h"
#include "tcpserver.h"
#include <cstdlib>
#include <iostream>
#include <system_error>

int main(int argc, char *argv[]) {
  auto server_ex = boost::leaf::try_handle_all(
    []() ->  boost::leaf::result<sws::SimpleTcpServer> {
      return sws::SimpleTcpServer::Create("127.0.0.1", 8080);
    },
    [](std::error_code err_code) -> sws::SimpleTcpServer {
       std::cerr << "Failed to start server: " << err_code.message()
              << std::endl;
       exit(EXIT_FAILURE);
    },
    []() -> sws::SimpleTcpServer {
      std::cerr << "Failed to start server"
              << std::endl;
      exit(EXIT_FAILURE);
    });
  server_ex.Run();
  return EXIT_SUCCESS;
}