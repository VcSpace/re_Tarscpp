//
// Created by Vcvc on 2021/11/1.
//

#include "tc_epoll_server.h"

int main(int argc, char **argv)
{
    std::cout << "main start" << std::endl;
    int port = 20999;
    std::string ip = "";

    if(argc > 2)
    {
        port = atoi(argv[1]);
        ip = argv[2];
    }

    tars::NetThread vnet;
    vnet.bind(ip, port);
    vnet.createEpoll(1);
    vnet.run();

    std::cout << "tars end" << std::endl;

    return 0;
}