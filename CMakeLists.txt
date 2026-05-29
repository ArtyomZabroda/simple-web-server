cmake_minimum_required(VERSION 3.20)

project(SimpleWebServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)
add_executable(SimpleWebServer main.cpp tcpsocket.cpp
    tcpserver.cpp
    httpserver.cpp
    config.cpp)
target_compile_options(SimpleWebServer PRIVATE -fno-exceptions)