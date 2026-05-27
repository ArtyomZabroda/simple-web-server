#ifndef SERVER_H_
#define SERVER_H_

#include "socket.h"
#include <functional>

namespace sws {

class TcpServer {
public:
  static tl::expected<TcpServer, std::error_code> Create(
      const std::string &hostname, int port,
      const std::function<void(TcpSocket &, std::vector<std::byte>)> &callback);
  void Run();

private:
  TcpServer(
      TcpSocket &&socket,
      const std::function<void(TcpSocket &, std::vector<std::byte>)> &callback);
  std::function<void(TcpSocket &, std::vector<std::byte>)> callback_;
  TcpSocket socket_;
};

} // namespace sws

#endif