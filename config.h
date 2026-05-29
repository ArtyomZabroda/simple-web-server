#ifndef CONFIG_H_
#define CONFIG_H_

#include <vector>
#include <string>
#include <unordered_map>
#include "utils.h"

namespace sws {

struct ServerConfig {
  std::string host;
  int port;
  std::unordered_map<std::string, std::string> routes;
};

struct Config {
  static boost::leaf::result<Config> Parse(const std::string& config_path);
  std::vector<ServerConfig> servers;
};

}

#endif