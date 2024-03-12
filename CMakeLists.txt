cmake_minimum_required(VERSION 3.27)
project(cyberkanojo)

set(CMAKE_CXX_STANDARD 17)

add_executable(cyberkanojo main.cpp)

find_package(CURL REQUIRED)
target_link_libraries(cyberkanojo CURL::libcurl)
