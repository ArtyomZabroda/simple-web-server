#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "tl/expected.hpp"
#include <span>
#include "utils.h"

namespace sws {

class TcpSocket {
 public:
  TcpSocket();
  TcpSocket(int raw_socket_fd);
  ~TcpSocket();

  TcpSocket& operator=(const TcpSocket& other) = delete;
  TcpSocket(const TcpSocket& other) = delete;
  TcpSocket(TcpSocket&& right);
  TcpSocket& operator=(TcpSocket&& right);
  
  tl::expected<void, std::error_code> connect(const std::string& hostname, int port);
  tl::expected<int, std::error_code> send(std::span<std::byte> data);
  tl::expected<std::vector<std::byte>, std::error_code> recv(int n);
  tl::expected<void, std::error_code> bind(const std::string& hostname, int port);
  tl::expected<void, std::error_code> listen(int backlog = SOMAXCONN);
  tl::expected<TcpSocket, std::error_code> accept();

 private:
  int socket_fd_ = -1;
};

}




#endif