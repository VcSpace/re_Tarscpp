#ifndef TARS_V_TC_EPOLL_SERVER_H
#define TARS_V_TC_EPOLL_SERVER_H


#include "tc_socket.h"

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
            NetThread(TC_EpollServer *epollServer);
            virtual ~NetThread();

            void bind(std::string &ip, int port);

        private:
            TC_EpollServer *_epollServer;

            TC_Socket _shutdown;
            TC_Socket _notify;
            TC_Socket _bind_listen;
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
