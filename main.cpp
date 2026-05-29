#include "leaf.hpp"
#include "httpserver.h"
#include "config.h"
#include <cstdlib>
#include <iostream>
#include <exception>

namespace boost {
#ifdef BOOST_LEAF_NO_EXCEPTIONS
[[noreturn]] void throw_exception(std::exception const & e) {
    std::cerr << "Fatal exception: " << e.what() << std::endl;
    std::terminate();
}
#endif
}


int main(int argc, char *argv[]) {
  boost::leaf::try_handle_all(
    [argc, argv]() -> boost::leaf::result<void> {
      std::string config_path = "config.toml";
      if (argc > 1) {
        config_path = argv[1];
      }

      auto parse_res = sws::Config::Parse(config_path);
      if (!parse_res && argc == 1) {
        // Try fallback to build/config.toml
        parse_res = sws::Config::Parse("build/config.toml");
      }

      BOOST_LEAF_AUTO(config, std::move(parse_res));
      
      if (config.servers.empty()) {
        return boost::leaf::new_error(std::string("No servers configured"));
      }

      std::cout << "Starting server on " << config.servers[0].host
                << ":" << config.servers[0].port << "..." << std::endl;

      BOOST_LEAF_AUTO(server, sws::HttpServer::Create(config.servers[0]));
      server.Run();
      return {};
    },
    [](std::error_code err_code) {
       std::cerr << "Failed to start server: " << err_code.message()
              << std::endl;
       exit(EXIT_FAILURE);
    },
    [](std::string const& err_msg) {
       std::cerr << "Error: " << err_msg << std::endl;
       exit(EXIT_FAILURE);
    },
    []() {
      std::cerr << "Failed to start server"
              << std::endl;
      exit(EXIT_FAILURE);
    });

  return EXIT_SUCCESS;
}