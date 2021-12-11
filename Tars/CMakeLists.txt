cmake_minimum_required(VERSION 3.16)
project(ReTars)

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-std=c++11 -W -Wall)

add_executable(${PROJECT_NAME}
        tc_epoll_server.cpp
        tc_epoller.cpp
        tc_socket.cpp
        main.cpp
        )
