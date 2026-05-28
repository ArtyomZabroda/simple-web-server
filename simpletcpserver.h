#ifndef SIMPLE_TCP_SERVER_H_
#define SIMPLE_TCP_SERVER_H_

#include "tcpserver.h"
#include <iostream>

namespace sws {

class SimpleTcpServer : public TcpServer {
public:
  static boost::leaf::result<SimpleTcpServer> Create(const std::string hostname, int port) {
    BOOST_LEAF_AUTO(server_socket, CreateServerSocket(hostname, port));
    SimpleTcpServer server(std::move(server_socket));
    return server;
  };
  SimpleTcpServer(TcpSocket&& server_socket) : TcpServer(std::move(server_socket)) {}
protected:
  boost::leaf::result<void> Dispatch(TcpSocket& socket) override {
    BOOST_LEAF_AUTO(recieved_data, socket.recv(1024));
    std::cout << recieved_data << std::endl;
    std::string response = "hello, world";
    std::string_view remaining_response = response;
    while (!remaining_response.empty()) {
      BOOST_LEAF_AUTO(bytes_sent, socket.send(remaining_response));
      remaining_response.remove_prefix(bytes_sent);
    }
    
    return {};
  }
};

}

#endif