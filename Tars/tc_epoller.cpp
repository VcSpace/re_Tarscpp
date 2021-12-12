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

    void TC_Epoller::add(int fd, long long int data, __uint32_t event)
    {
        ctrl(fd, data, event, EPOLL_CTL_ADD);
    }

    void TC_Epoller::mod(int fd, long long int data, __uint32_t event)
    {
        ctrl(fd, data, event, EPOLL_CTL_MOD);
    }

    void TC_Epoller::del(int fd, long long int data, __uint32_t event)
    {
        ctrl(fd, data, event, EPOLL_CTL_DEL);
    }

    void TC_Epoller::ctrl(int fd, long long int data, __uint32_t events, int mode)
    {
        struct epoll_event ev;
        ev.data.u64 = data;

        if(_et) //et
        {
            ev.events = events | EPOLLET;
        }
        else
        {
            ev.events = events;
        }

        epoll_ctl(_iEpollfd, mode, fd, &ev);
    }

} //tars


