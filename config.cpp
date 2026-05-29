#include "config.h"
#include "toml.hpp"
#include <format>

namespace sws {
  
boost::leaf::result<Config> Config::Parse(const std::string& config_path) {
  using namespace std::literals;

  Config config;

  auto toml_config = toml::parse_file(config_path);
  if (!toml_config) {
    return boost::leaf::new_error(std::format("Failed to parse config file {}: {}\n", config_path, toml_config.error().description()));
  }

  if (toml::array* servers_arr = toml_config["servers"].as_array()) {
    for (auto&& node : *servers_arr) {
      if (toml::table* server_tbl = node.as_table()) {
        ServerConfig server;
        if (auto host_opt = (*server_tbl)["host"].value<std::string>()) {
          server.host = *host_opt;
        }
        if (auto port_opt = (*server_tbl)["port"].value<int>()) {
          server.port = *port_opt;
        }
        if (toml::array* routes_arr = (*server_tbl)["routes"].as_array()) {
          for (auto&& route_node : *routes_arr) {
            if (toml::table* route_tbl = route_node.as_table()) {
              std::string path;
              std::string target;
              if (auto path_opt = (*route_tbl)["path"].value<std::string>()) {
                path = *path_opt;
              }
              if (auto target_opt = (*route_tbl)["target"].value<std::string>()) {
                target = *target_opt;
              }
              if (!path.empty()) {
                server.routes[std::move(path)] = std::move(target);
              }
            }
          }
        }
        config.servers.push_back(server);
      }
    }
  }

  return config;
}

}