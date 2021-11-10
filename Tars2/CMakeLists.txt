cmake_minimum_required(VERSION 3.16)
project(Tars_v)

add_executable(Tars_v
        tc_epoll_server.cpp
        tc_epoller.cpp
        main.cpp
        )
