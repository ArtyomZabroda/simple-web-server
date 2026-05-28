#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include "tcpserver.h"

namespace sws {

class HttpServer : public TcpServer {
public:
  static boost::leaf::result<HttpServer> Create(const std::string& hostname = "127.0.0.1", int port = 80);
protected:
  boost::leaf::result<void> Dispatch(TcpSocket& socket) override;
private:
  HttpServer(TcpSocket&& server_socket);
};

}

#endif