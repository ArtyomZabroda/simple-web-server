#include "httpserver.h"
#include "tcpserver.h"

namespace sws {

boost::leaf::result<HttpServer> HttpServer::Create(const std::string& hostname, int port) {
  BOOST_LEAF_AUTO(server_socket, CreateServerSocket(hostname, port));
  HttpServer server(std::move(server_socket));
  return server;
}

HttpServer::HttpServer(TcpSocket&& server_socket) : TcpServer(std::move(server_socket)) {}

boost::leaf::result<void> HttpServer::Dispatch(TcpSocket& socket) {
  std::string buf;
  std::string recieved_data;
  while (buf.find("\r\n\r\n") == std::string::npos) {
    BOOST_LEAF_ASSIGN(buf, socket.recv(1024));
    recieved_data += buf;
  }

  std::string_view recieved_data_view{recieved_data};
  size_t pos = recieved_data_view.find('\n');
  std::string_view request = recieved_data_view.substr(0, pos);

  std::string_view method;
  std::string_view uri;
  std::string_view version;

  auto start = request.begin();
  auto current = start;
  while (*current != ' ') ++current;
  method = std::string_view(start, current);
  ++current;
  start = current;

  while (*current != ' ') ++current;
  uri = std::string_view(start, current);
  ++current;
  start = current;
  while (*current != ' ') ++current;
  version = std::string_view(start, current);

  std::cout << "Method: " << method << "; URI: " << uri << "; Version: " << version << "\n";
  
  return {};
}

}