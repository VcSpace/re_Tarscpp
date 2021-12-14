//
// Created by Vcvc on 2021/12/10.
//

#include "tc_epoll_server.h"

int main(int argc, char **argv)
{
    int port = 8888;
    std::string ip = "";
    if(argc > 2)
    {
        ip = argv[1];
        port = atoi(argv[2]);
    }
    std::cout << "ReTars Start"<< std::endl;
    std::cout << "set ip = " << ip << "  " << "set port = " << port << std::endl;

    tars::TC_EpollServer *eserver = new tars::TC_EpollServer();
    tars::TC_EpollServer::NetThread* vNetThread = eserver->getNetThread();

    vNetThread->bind(ip,port);
    vNetThread->createEpoll(1);

    tars::TC_EpollServer::Handle handle;
    handle.setEpollServer(eserver);
    handle.start();

    std::cout << "close server " << std::endl;

    return 0;
}