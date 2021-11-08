#include "tc_epoller.h"

namespace tars
{
    TC_Epoller::TC_Epoller(bool bET) : _iEpollfd(0), _et(bET), _pevs(nullptr), _max_connections(1024)
    {

    }

    TC_Epoller::~TC_Epoller()
    {
        if(_pevs != nullptr)
        {
            delete[] _pevs;
            _pevs = nullptr;
        }

        if(_iEpollfd > 0)
        {
            close(_iEpollfd);
        }
    }

    void TC_Epoller::create(int max_connections)
    {
        _max_connections = max_connections;
        _iEpollfd = epoll_create(_max_connections + 1);
        delete[] _pevs;
        _pevs = new epoll_event[_max_connections + 1];
    }

}