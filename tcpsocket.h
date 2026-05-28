#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <vector>
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
  
  boost::leaf::result<void> connect(const std::string& hostname, int port);
  boost::leaf::result<int> send(std::span<std::byte> data);
  boost::leaf::result<std::vector<std::byte>> recv(int n);
  boost::leaf::result<void> bind(const std::string& hostname, int port);
  boost::leaf::result<void> listen(int backlog = SOMAXCONN);
  boost::leaf::result<TcpSocket> accept();

 private:
  int socket_fd_ = -1;
};

}




#endif