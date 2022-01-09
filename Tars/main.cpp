//
// Created by Vcvc on 2021/12/10.
//

#include "tc_epoll_server.h"

int main(int argc, char **argv)
{
    int port = 20999;
    std::string ip = "";
    if(argc > 3)
    {
        ip = argv[1];
        port = atoi(argv[2]);
    }
    std::cout << "-----------------\nReTars Start"<< std::endl;

    std::cout << "set ip = " << ip << "  " << "set port = " << port << std::endl;

    tars::TC_EpollServerPtr _epollServer = std::make_shared<tars::TC_EpollServer>();
    tars::TC_EpollServer::NetThread* vNetThread = _epollServer->getNetThread();

    tars::TC_EpollServer::BindAdapterPtr lsPtr = std::make_shared<tars::TC_EpollServer::BindAdapter>(_epollServer.get());
    lsPtr->setEndpoint(ip,port);
    _epollServer->bind(lsPtr);

    vNetThread->createEpoll(1);

    std::vector<tars::TC_EpollServer::HandlePtr> handles;
    int handleNum = 4;

    for (int32_t i = 0; i < handleNum; ++i)
    {
        tars::TC_EpollServer::HandlePtr handle = std::make_shared<tars::TC_EpollServer::Handle>();
        handle->setEpollServer(_epollServer.get());
        handles.push_back(handle);
    }

    for(auto& handle : handles)
    {
        handle->start();
    }

    vNetThread->run();

    std::cout << "-----------------\nReTars Exit" << std::endl;

    return 0;
}