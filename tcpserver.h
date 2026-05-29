#ifndef SERVER_H_
#define SERVER_H_

#include "tcpsocket.h"

namespace sws {

class TcpServer {
public:
  static boost::leaf::result<TcpSocket> CreateServerSocket(
      const std::string &hostname, int port);
  TcpServer(TcpSocket&& server_socket);
  void Run();
protected:
  virtual boost::leaf::result<void> Dispatch(TcpSocket& socket) = 0;
private:

  TcpSocket socket_;
  bool stopped_ = true;
  std::vector<std::jthread> workers_;
};

} // namespace sws

#endif