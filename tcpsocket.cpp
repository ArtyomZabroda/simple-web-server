#include "tcpsocket.h"
#include <cerrno>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <system_error>

namespace sws {

TcpSocket::TcpSocket() {
  socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
}

TcpSocket::TcpSocket(int raw_socket_fd) {
  socket_fd_ = raw_socket_fd;
}

TcpSocket::TcpSocket(TcpSocket&& right) : socket_fd_(right.socket_fd_) {
  // Invalidate the moved-from socket so it doesn't get closed on destruction
  right.socket_fd_ = -1;
}

TcpSocket& TcpSocket::operator=(TcpSocket&& right) {
  if (this != &right) {
    if (socket_fd_ != -1) {
      close(socket_fd_);
    }
    socket_fd_ = right.socket_fd_;
    right.socket_fd_ = -1;
  }
  return *this;
}

TcpSocket::~TcpSocket() {
  if (socket_fd_ != -1) {
    close(socket_fd_);
  }
}

boost::leaf::result<void> TcpSocket::connect(const std::string& hostname, int port) {
  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(hostname.c_str());
  
  if (::connect(socket_fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
    return boost::leaf::new_error(std::error_code(errno, std::system_category()));
  }
  return {};
}

boost::leaf::result<int> TcpSocket::send(std::string_view data) {
  int bytes_sent_nr;
  if ((bytes_sent_nr = ::send(socket_fd_, data.data(), data.size(), MSG_NOSIGNAL)) == -1) {
    return boost::leaf::new_error(std::error_code(errno, std::system_category()));
  }
  return bytes_sent_nr;
}

boost::leaf::result<std::string> TcpSocket::recv(int n) {
  std::string buffer(n, '\0');
  int bytes_recieved_nr;
  if ((bytes_recieved_nr = ::recv(socket_fd_, buffer.data(), n, 0)) == -1) {
    return boost::leaf::new_error(std::error_code(errno, std::system_category()));
  }
  buffer.resize(bytes_recieved_nr);
  return buffer;
}

boost::leaf::result<void> TcpSocket::bind(const std::string& hostname, int port) {
  int opt = 1;
  if (setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
    return boost::leaf::new_error(std::error_code(errno, std::system_category()));
  }
  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(hostname.c_str());
  if (::bind(socket_fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
    return boost::leaf::new_error(std::error_code(errno, std::system_category()));
  }
  return {};
}

boost::leaf::result<void> TcpSocket::listen(int backlog) {
  if (::listen(socket_fd_, backlog) == -1) {
    return boost::leaf::new_error(std::error_code(errno, std::system_category()));
  }
  return {};
}

boost::leaf::result<TcpSocket> TcpSocket::accept() {
  // We pass nullptr because we don't need the client's address information
  int client_fd = ::accept(socket_fd_, nullptr, nullptr);
  if (client_fd == -1) {
    return boost::leaf::new_error(std::error_code(errno, std::system_category()));
  }
  return TcpSocket(client_fd);
}

}