#include "socket.h"
#include "tl/expected.hpp"
#include <cerrno>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <system_error>

namespace sws {

TcpSocket::TcpSocket() {
  socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
}

TcpSocket::~TcpSocket() {
  close(socket_fd_);
}

tl::expected<void, std::error_code> TcpSocket::connect(const std::string& hostname, int port) {
  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(hostname.c_str());
  
  if (::connect(socket_fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
    return tl::unexpected(std::error_code(errno, std::system_category()));
  }
  return {};
}

tl::expected<int, std::error_code> TcpSocket::send(std::byte* data, int n) {
  int bytes_sent_nr;
  if ((bytes_sent_nr = ::send(socket_fd_, data, n, 0)) == -1) {
    return tl::unexpected(std::error_code(errno, std::system_category()));
  }
  return bytes_sent_nr;
}

tl::expected<std::vector<std::byte>, std::error_code> TcpSocket::recv(int n) {
  std::vector<std::byte> buffer(n);
  int bytes_recieved_nr;
  if ((bytes_recieved_nr = ::recv(socket_fd_, buffer.data(), n, 0)) == -1) {
    return tl::unexpected(std::error_code(errno, std::system_category()));
  }
  buffer.resize(bytes_recieved_nr);
  return buffer;
}

}