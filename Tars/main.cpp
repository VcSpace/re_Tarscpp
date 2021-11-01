//
// Created by Vcvc on 2021/11/1.
//

#include "tc_epoll_server.h"

int main(int argc, char **argv)
{
    int port = 20999;
    std::string ip = "";

    if(argc > 2)
    {
        port = atoi(argv[1]);
        ip = argv[2];
    }

    tars::NetThread vnetThread = new tars::NetThread();


    return 0;
}