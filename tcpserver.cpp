#include "tcpserver.h"
#include "tcpsocket.h"
#include <iostream>

namespace sws {

boost::leaf::result<TcpSocket> TcpServer::CreateServerSocket(
    const std::string &hostname, int port) {
  TcpSocket server_socket;
  BOOST_LEAF_CHECK(server_socket.bind(hostname, port));
  BOOST_LEAF_CHECK(server_socket.listen());
  return server_socket;
}

TcpServer::TcpServer(
    TcpSocket&& socket)
    : socket_(std::move(socket)) {}

void TcpServer::Run() {
  stopped_ = false;
  while (!stopped_) {
    auto socket_ex = socket_.accept();
    if (!socket_ex.has_value()) {
      std::cerr << socket_ex.error() << std::endl;
      continue;
    }
    auto dispatch_result = Dispatch(socket_ex.value());
    if (!dispatch_result) {
      std::cerr << "Failed to dispatch a socket\n";
      continue;
    }
  }
}

} // namespace sws
