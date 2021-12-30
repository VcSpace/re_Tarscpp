#ifndef RETARS_TC_SOCKET_H
#define RETARS_TC_SOCKET_H

#include <iostream>
#include <cstring>
#include <cassert>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <fcntl.h>

namespace tars
{
    class TC_Socket
    {
    public:
        TC_Socket();
        virtual ~TC_Socket();

        void createSocket(int Domain = AF_INET, int type = SOCK_STREAM);
        void bind(const std::string &ip, int port);
        void bind(struct sockaddr *s_addr, socklen_t s_addr_len);
        void parseaddr(const std::string &ip, struct in_addr &s_addr);
        void listen(int conn);
        void setKeepAlive();
        void setTcpNoDelay();
        void setNoCloseWait();
        void setblock(bool flag);
        void setblock(int fd, bool flag);
        void setOwner(bool bOwner)  { _bOwner = bOwner; }
        void init(int fd, bool bOwner, int iDomain = AF_INET);
        void setCloseWaitDefault();

        int setSockOpt(int opt, const void *pvOptVal, socklen_t optLen, int level = SOL_SOCKET);
        int accept(TC_Socket &tcSock, struct sockaddr *pstSockAddr, socklen_t &iSockLen);
        int getfd() const { return _sock; }

    private:
        void close();
        bool _bOwner;

        static const int INVALID_SOCKET = -1;
        int _Socktype;
        int _Domain;
        int _sock;
    };
}

#endif //RETARS_TC_SOCKET_H
