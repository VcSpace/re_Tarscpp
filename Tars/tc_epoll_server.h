#ifndef TARS_V_TC_EPOLL_SERVER_H
#define TARS_V_TC_EPOLL_SERVER_H

#include <vector>
#include <map>
#include <list>

#include "tc_socket.h"
#include "tc_epoller.h"
#include "tc_thread.h"
#include "tc_monitor.h"

namespace tars
{
    class TC_EpollServer
    {
    public:
        TC_EpollServer();
        virtual ~TC_EpollServer();

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

            NetThread(TC_EpollServer *epollServer);
            virtual ~NetThread();

            void bind(std::string &ip, int port);
            void createEpoll(uint32_t iIndex = 0);

        private:
            TC_EpollServer *_epollServer;

            TC_Socket _shutdown;
            TC_Socket _notify;
            TC_Socket _bind_listen;

            TC_Epoller _epoller;

            std::map<int,int> _listen_connect_id;
            std::list<uint32_t> _free;
            volatile size_t _free_size;
        };

        class Handle : public TC_Thread, public TC_ThreadLock
        {
        public:
            Handle();
            virtual ~Handle();

            virtual void run();
            void setEpollServer(TC_EpollServer *eserv);

        private:
            TC_EpollServer *_pEpollServer;
        protected:
        };


    public:
        TC_EpollServer::NetThread *getNetThread()
        {
            return _netThreads;
        }

    private:
        NetThread *_netThreads;
    };

}

#endif //TARS_V_TC_EPOLL_SERVER_H