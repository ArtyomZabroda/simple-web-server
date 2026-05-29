#include "tcpserver.h"
#include "tcpsocket.h"
#include <iostream>

namespace sws {

boost::leaf::result<TcpSocket>
TcpServer::CreateServerSocket(const std::string &hostname, int port) {
  TcpSocket server_socket;
  BOOST_LEAF_CHECK(server_socket.bind(hostname, port));
  BOOST_LEAF_CHECK(server_socket.listen());
  return server_socket;
}

TcpServer::TcpServer(TcpSocket &&socket) : socket_(std::move(socket)) {}

void TcpServer::Run() {
  stopped_ = false;
  while (!stopped_) {
    std::erase_if(workers_, [](const std::jthread &t) {
      // A thread that has finished executing its function is no longer joinable
      return !t.joinable();
    });

    auto socket_ex = socket_.accept();
    if (!socket_ex.has_value()) {
      std::cerr << socket_ex.error() << std::endl;
      continue;
    }
    
    workers_.emplace_back(
        [this, client_socket = std::move(socket_ex.value())]() mutable {
          auto dispatch_result = Dispatch(client_socket);
          if (!dispatch_result) {
            std::cerr << "Failed to dispatch a socket\n";
          }
        });
  }
}

} // namespace sws
