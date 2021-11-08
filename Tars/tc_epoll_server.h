#ifndef LEARN_TARSCPP_TC_EPOLL_SERVER_H
#define LEARN_TARSCPP_TC_EPOLL_SERVER_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <fcntl.h>


#include "tc_epoller.h"

namespace tars
{
    class NetThread
    {
    public:
        NetThread();
        ~NetThread();
        void bind(std::string &ip, int port);
        //void createepoll(1);
        void run();
        void parseAddr(const std::string &ip, struct in_addr &seraddr);
        void createEpoll(uint32_t iIndex= 0);

    private:
        TC_Epoller _epoller;
        int _sock;

    };
}

#endif //LEARN_TARSCPP_TC_EPOLL_SERVER_H
