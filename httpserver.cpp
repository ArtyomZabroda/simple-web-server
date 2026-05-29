#include "httpserver.h"
#include "tcpserver.h"

#include <fstream>
#include <ios>
#include <sstream>
#include <thread>

namespace sws {

boost::leaf::result<HttpServer> HttpServer::Create(ServerConfig config) {
  BOOST_LEAF_AUTO(server_socket, CreateServerSocket(config.host, config.port));
  HttpServer server(std::move(server_socket), std::move(config));
  return server;
}

HttpServer::HttpServer(TcpSocket &&server_socket, ServerConfig &&config)
    : TcpServer(std::move(server_socket)), config_(std::move(config)) {}

boost::leaf::result<void> HttpServer::Dispatch(TcpSocket &socket) {
  std::string buf;
  std::string recieved_data;
  size_t start;
  size_t parse_position = 0;
  size_t offset;
  
  while (true) {
    start = parse_position;
    while ((offset = recieved_data.find("\r\n\r\n")) == std::string::npos) {
      BOOST_LEAF_ASSIGN(buf, socket.recv(1024));
      if (buf.empty()) {
        return {};
      }
      recieved_data += buf;
      if (recieved_data.size() > kMaxHeaderSize) {
        return boost::leaf::new_error(std::string("Request headers exceed max size"));
      }
    }
    
    std::string_view recieved_data_view{recieved_data};
    size_t pos = recieved_data_view.find('\n');
    std::string_view request = recieved_data_view.substr(0, pos);

    std::string_view method;
    std::string_view uri;
    std::string_view version;

    while (recieved_data_view[parse_position] != ' ')
      ++parse_position;
    method = std::string_view(&recieved_data_view[start], &recieved_data_view[parse_position]);
    ++parse_position;
    start = parse_position;

    while (recieved_data_view[parse_position] != ' ')
      ++parse_position;
    uri = std::string_view(&recieved_data_view[start], &recieved_data_view[parse_position]);
    ++parse_position;
    start = parse_position;
    while (recieved_data_view[parse_position] != '\r')
      ++parse_position;
    version = std::string_view(&recieved_data_view[start], &recieved_data_view[parse_position]);
    ++parse_position; // skip '\n'
    if (version != "HTTP/1.1") {
      return boost::leaf::new_error(std::string("Unsupported http version"));
    }

    if (method != "GET") {
      return boost::leaf::new_error(std::string("Unsupported method"));
    }

    std::string response;

    auto route_it = config_.routes.find(std::string(uri));
    if (route_it != config_.routes.end()) {
      std::ifstream ifs{route_it->second, std::ios_base::in};
      if (!ifs.is_open()) {
        response = "HTTP/1.1 404 Not Found\r\n\r\n";
      } else {
        std::ostringstream ss;
        ss << ifs.rdbuf();
        std::string message_body = ss.str();
        response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Length: " + std::to_string(message_body.size()) + "\r\n";
        response += "\r\n";
        response += message_body;
      }
    } else {
      response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
    }

    BOOST_LEAF_CHECK(socket.send(response));
    recieved_data.erase(0, offset + 4); // 4 extra symbols for removing \r\n\r\n
    parse_position = 0;

  }

  return {};
}

} // namespace sws