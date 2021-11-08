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

    void TC_Epoller::ctrl(int fd, long long data, __uint32_t event, int op)
    {
        struct epoll_event ev;
        ev.data.u64 = data;

        if(_et)
        {
            ev.events = event | EPOLLET;
        }
        else
        {
            ev.events = event;
        }
        epoll_ctl(_iEpollfd, op, fd, &ev);
    }

    void TC_Epoller::add(int fd, long long data, __uint32_t event) //_sock, h64(close), EPOLLIN
    {
        crrl(fd, data, event, EPOLL_CTL_ADD);
    }

    void TC_Epoller::mod(int fd, long long data, __uint32_t event)
    {
        crrl(fd, data, event, EPOLL_CTL_MOD);
    }

    void TC_Epoller::del(int fd, long long data, __uint32_t event)
    {
        crrl(fd, data, event, EPOLL_CTL_DEL);
    }

    int TC_Epoller::wait(int millsecond)
    {
        return epoll_wait(_iEpollfd, _pevs, _max_connections, millsecond);
    }

}