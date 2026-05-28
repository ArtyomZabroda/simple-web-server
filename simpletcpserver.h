#ifndef SIMPLE_TCP_SERVER_H_
#define SIMPLE_TCP_SERVER_H_

#include "tcpserver.h"
#include <iostream>

namespace sws {

class SimpleTcpServer : public TcpServer {
public:
  static boost::leaf::result<SimpleTcpServer> Create(const std::string hostname, int port) {
    BOOST_LEAF_CHECK(CreateServerSocket(hostname, port));
    BOOST_LEAF_AUTO(server_socket, CreateServerSocket(hostname, port));
    SimpleTcpServer server(std::move(server_socket));
    return server;
  };
  SimpleTcpServer(TcpSocket&& server_socket) : TcpServer(std::move(server_socket)) {}
protected:
  boost::leaf::result<void> Dispatch(TcpSocket& socket) override {
    BOOST_LEAF_AUTO(recieved_data, socket.recv(1024));
    std::string recieved_str(reinterpret_cast<const char *>(recieved_data.data()),
                                 recieved_data.size());
    std::cout << recieved_str << std::endl;
    std::string response = "hello, world";
    int sent_sz = 0;
    while (sent_sz < response.size()) {
      BOOST_LEAF_ASSIGN(sent_sz, socket.send(std::span<std::byte>(
            reinterpret_cast<std::byte *>(response.data()), response.size())));
    }
    
    return {};
  }
};

}

#endif