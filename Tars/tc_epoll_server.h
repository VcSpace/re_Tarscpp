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
#include <list>
#include <map>

#include "tc_epoller.h"

namespace tars
{
    class NetThread
    {
    public:
        enum
        {
            ET_LISTEN = 1,
            ET_CLOSE  = 2,
            ET_NOTIFY = 3,
            ET_NET    = 0,
        };

        struct
        {
            std::string response;
            uint32_t uid;
        }_response;

        NetThread();
        virtual ~NetThread();
        void bind(std::string &ip, int port);
        void run();
        void parseAddr(const std::string &ip, struct in_addr &seraddr);
        void createEpoll(uint32_t iIndex= 0);
        void processPipe();
        void processNet(const struct epoll_event &ev);

        bool accept(int fd);

    private:
        TC_Epoller _epoller;

        std::list<uint32_t> _free;
        volatile size_t _free_size;
        std::map<int,int> _listen_connect_id;

        std::string _recvbuffer;
        int ifd;
        int _sock;
        int _shutdown_sock;
        int _notify_sock;
    };
}

#endif //LEARN_TARSCPP_TC_EPOLL_SERVER_H
