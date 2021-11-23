#ifndef LEARN_TARSCPP_TC_EPOLL_SERVER_H
#define LEARN_TARSCPP_TC_EPOLL_SERVER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <netinet/in.h>

#include "tc_epoller.h"
#include "tc_socket.h"

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
        void run();
        void createEpoll(uint32_t iIndex= 0);
        void processPipe();
        void processNet(const epoll_event &ev);

        int bind(std::string &ip, int port);

        bool accept(int fd);

    private:
        std::list<uint32_t> _free;
        std::map<int,int> _listen_connect_id;

        std::string _recvbuffer;
        std::string                    response;

        int ifd;
        volatile size_t _free_size;

        TC_Socket _shutdown;
        TC_Socket _notify;
        TC_Socket _bind_listen;

        TC_Epoller _epoller;
    };
}

#endif //LEARN_TARSCPP_TC_EPOLL_SERVER_H
