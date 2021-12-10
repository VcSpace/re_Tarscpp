#include "tc_epoll_server.h"


namespace tars
{
    TC_EpollServer::TC_EpollServer()
    {
        _netThreads = new TC_EpollServer::NetThread(this);
    }
    TC_EpollServer::~TC_EpollServer()
    {

    }

    TC_EpollServer::NetThread::NetThread(TC_EpollServer *epollServer) : _epollServer(epollServer)
    {
    }

    TC_EpollServer::NetThread::~NetThread() {}

    void TC_EpollServer::NetThread::bind(std::string &ip, int port)
    {
        int Domain = AF_INET;
        int type = SOCK_STREAM;
        _bind_listen.createsock(Domain, type);
        _bind_listen.bind(ip, port);
    }


}