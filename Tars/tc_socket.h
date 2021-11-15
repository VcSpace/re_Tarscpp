#ifndef TARS_V_TC_SOCKET_H
#define TARS_V_TC_SOCKET_H

#include "socket.h"

namespace tars
{
    class TC_Socket
    {
    public:
        TC_Socket();
        virtual ~TC_Socket();

        void createSocket(int iSocketType = SOCK_STREAM, int iDomain = AF_INET);
        void close();
        void bind(struct sockaddr *addr, socklen_t socklen);
        void bind(const std::string &ip, int port);
        void listen(int connBackLog);
        int setSockOpt(int opt, const void *pvOptVal, socklen_t optLen, int level = SOL_SOCKET);
        void parseAddr(const std::string &ip, struct in_addr &seraddr);

    private:
        static const int INVALID_SOCKET = -1;
        int  _sock;
        int  _iDomain;

        bool _bOwner;
    };
}


#endif //TARS_V_TC_SOCKET_H
