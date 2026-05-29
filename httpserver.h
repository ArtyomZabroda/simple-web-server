#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include "tcpserver.h"
#include "config.h"

namespace sws {

class HttpServer : public TcpServer {
public:
  static boost::leaf::result<HttpServer> Create(ServerConfig config);
protected:
  boost::leaf::result<void> Dispatch(TcpSocket& socket) override;
private:
  HttpServer(TcpSocket&& server_socket, ServerConfig&& config);
  ServerConfig config_;
  
  static constexpr size_t kMaxHeaderSize = 8192;
};

}

#endif