#ifndef LEARN_TARSCPP_TC_EPOLL_SERVER_H
#define LEARN_TARSCPP_TC_EPOLL_SERVER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <netinet/in.h>

#include "tc_epoller.h"

namespace tars
{
    class NetThread
    {
    public:
        NetThread();
        ~NetThread();
        void bind(std::string &ip, int port);
        void createepoll(1);
        void run();
        void parseAddr(const std::sring &ip, struct in_addr &seraddr);

    private:
        int _sock;

    };
}

#endif //LEARN_TARSCPP_TC_EPOLL_SERVER_H
