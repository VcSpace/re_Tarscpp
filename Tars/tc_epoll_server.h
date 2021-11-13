#ifndef LEARN_TARSCPP_TC_EPOLL_SERVER_H
#define LEARN_TARSCPP_TC_EPOLL_SERVER_H

#include <cstring>
#include <list>
#include <map>

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
        void bind(std::string &ip, int port);
        void run();
        void parseAddr(const std::string &ip, struct in_addr &seraddr);
        void createEpoll(uint32_t iIndex= 0);
        void processPipe();
        void processNet(const struct epoll_event &ev);

        bool accept(int fd);

    private:
        std::list<uint32_t> _free;
        std::map<int,int> _listen_connect_id;

        std::string _recvbuffer;

        int ifd;
        volatile size_t _free_size;

        TC_Socket _shutdown;
        TC_Socket _notify;
        TC_Socket _bind_listen;

        TC_Epoller _epoller;
    };
}

#endif //LEARN_TARSCPP_TC_EPOLL_SERVER_H
