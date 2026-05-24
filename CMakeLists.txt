cmake_minimum_required(VERSION 3.20)

project(SimpleWebServer)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED)
add_executable(SimpleWebServer main.cpp socket.cpp)