cmake_minimum_required(VERSION 3.29)
project(pingpong)

set(CMAKE_CXX_STANDARD 20)

add_executable(client client.cpp)
add_executable(sever server.cpp)