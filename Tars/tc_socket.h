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

        void init(int fd, bool bOwner, int iDomain = AF_INET);
        void createSocket(int iSocketType = SOCK_STREAM, int iDomain = AF_INET);
        void close();
        void bind(struct sockaddr *addr, socklen_t socklen);
        void bind(const std::string &ip, int port);
        void listen(int connBackLog);
        void parseAddr(const std::string &ip, struct in_addr &seraddr);
        void setKeepAlive();
        void setTcpNoDelay();
        void setNoCloseWait();
        void setCloseWaitDefault();
        void setblock(bool block);
        void setblock(int fd, bool block);
        void setOwner(bool bOwner)  { _bOwner = bOwner; }

        int setSockOpt(int opt, const void *pvOptVal, socklen_t optLen, int level = SOL_SOCKET);
        int getfd() const { return _sock; }
        int accept(TC_Socket &tcSock, struct sockaddr *pstSockAddr, socklen_t &iSockLen);

    private:
        static const int INVALID_SOCKET = -1;
        int  _sock;
        int  _iDomain;

        bool _bOwner;
    };
}


#endif //TARS_V_TC_SOCKET_H
