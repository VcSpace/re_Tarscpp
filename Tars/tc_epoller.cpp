#include "tc_epoller.h"

namespace tars
{
    TC_Epoller::TC_Epoller(bool et) : _et(et), _iEpollfd(0), _max_connections(1024), _pevs(nullptr)
    {

    }

    TC_Epoller::~TC_Epoller()
    {
        if(_pevs != nullptr)
        {
            delete[] _pevs;
            _pevs = nullptr;
        }

        if(_iEpollfd != 0)
        {
            ::close(_iEpollfd);
        }

    }

    void TC_Epoller::createepoll(int conn)
    {
        _max_connections = conn;

        _iEpollfd = epoll_create(_max_connections + 1);

        _pevs = new epoll_event[_max_connections + 1];
    }

}


