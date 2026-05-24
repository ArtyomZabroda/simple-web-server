#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "tl/expected.hpp"
#include "utils.h"

namespace sws {

class TcpSocket {
 public:
  TcpSocket();
  ~TcpSocket();

  TcpSocket& operator=(const TcpSocket& other) = delete;
  TcpSocket(const TcpSocket& other) = delete;
  TcpSocket(TcpSocket&& right) noexcept;
  TcpSocket& operator=(TcpSocket&& right) noexcept;
  
  tl::expected<void, std::error_code> connect(const std::string& hostname, int port);
  tl::expected<int, std::error_code> send(std::byte* data, int n);
  tl::expected<std::vector<std::byte>, std::error_code> recv(int n);
  tl::expected<void, std::error_code> bind(const std::string& hostname, int port);
  tl::expected<void, std::error_code> listen();

 private:
  int socket_fd_;
};

}




#endif