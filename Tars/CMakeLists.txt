cmake_minimum_required(VERSION 3.16)
project(ReTars)

set(CMAKE_BUILD_TYPE Release)

add_compile_options(-std=c++11 -W -Wall)

add_executable(${PROJECT_NAME}
        tc_epoll_server.cpp
        tc_epoller.cpp
        tc_socket.cpp
        tc_thread.cpp
        tc_thread_mutex.cpp
        tc_ex.cpp
        tc_clientsocket.cpp
        main.cpp
        )

#TARGET_LINK_LIBRARIES(${PROJECT_NAME} -pthread -lmysqlclient)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -pthread)