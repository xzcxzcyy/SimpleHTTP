cmake_minimum_required(VERSION 3.0)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(src main.cpp config/config.cpp config/config.h response/response.cpp response/response.h io_util/socket_stream.cpp io_util/socket_stream.h logger/logger.cpp logger/logger.h)

TARGET_LINK_LIBRARIES(src pthread)