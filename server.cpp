#include "server.h"
#include "socket.h"
#include <iostream>

namespace sws {

tl::expected<TcpServer, std::error_code> TcpServer::Create(
    const std::string &hostname, int port,
    const std::function<void(TcpSocket &, std::vector<std::byte>)> &callback) {
  TcpSocket socket;
  CHECK(socket.bind(hostname, port));
  CHECK(socket.listen());
  return TcpServer(std::move(socket), callback);
}

TcpServer::TcpServer(
    TcpSocket &&socket,
    const std::function<void(TcpSocket &, std::vector<std::byte>)> &callback)
    : callback_(callback), socket_(std::move(socket)) {}

void TcpServer::Run() {
  while (true) {
    auto socket_ex = socket_.accept();
    if (!socket_ex.has_value()) {
      std::cerr << socket_ex.error().message() << std::endl;
      continue;
    }
    TcpSocket &connection_socket = socket_ex.value();
    auto recieved_data_ex = connection_socket.recv(1024);
    if (!recieved_data_ex.has_value()) {
      std::cerr << recieved_data_ex.error().message() << std::endl;
      continue;
    }
    auto recieved_data = recieved_data_ex.value();
    callback_(connection_socket, recieved_data);
  }
}

} // namespace sws
