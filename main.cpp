#include "server.h"
#include <iostream>

int main(int argc, char *argv[]) {
  auto server_ex = sws::TcpServer::Create(
      "127.0.0.1", 8080,
      [](sws::TcpSocket &connection_socket, std::vector<std::byte> bytes) {
        std::string recieved_str(reinterpret_cast<const char *>(bytes.data()),
                                 bytes.size());
        std::cout << "Recieved: " << recieved_str << std::endl;
        std::string response = "ok";
        connection_socket.send(std::span<std::byte>(
            reinterpret_cast<std::byte *>(response.data()), response.size()));
      });

  if (server_ex.has_value()) {
    std::cout << "Server is running on 127.0.0.1:8080..." << std::endl;
    server_ex.value().Run();
  } else {
    std::cerr << "Failed to start server: " << server_ex.error().message()
              << std::endl;
    return 1;
  }

  return 0;
}