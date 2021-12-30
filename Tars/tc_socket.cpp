#include "tc_socket.h"

namespace tars
{

    TC_Socket::TC_Socket() : _sock(INVALID_SOCKET), _bOwner(true), _Domain(AF_INET) {
    }

    TC_Socket::~TC_Socket() {
        if(_bOwner)
        {
            close();
        }

    }

    void TC_Socket::createSocket(int Domain, int type)
    {
        assert(type == SOCK_STREAM || type == SOCK_DGRAM);

        close();
        _Socktype = type;
        _Domain = Domain;

        _sock = socket(_Domain, _Socktype, 0);
        if(_sock < 0)
        {
            _sock = INVALID_SOCKET;
            std::cout << "create socket error" << std::endl;
        }
    }

    void TC_Socket::close()
    {
        if (_sock != INVALID_SOCKET)
        {
            ::close(_sock);
            _sock = INVALID_SOCKET;
        }
    }

    void TC_Socket::bind(const std::string &ip, int port)
    {
        struct sockaddr_in serv_addr;
        bzero(&serv_addr, sizeof(serv_addr));

        serv_addr.sin_family = _Domain;
        serv_addr.sin_port = htons(port);
        if(ip == "")
        {
            serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        } else{
            parseaddr(ip, serv_addr.sin_addr);
        }

        try
        {
            bind((struct sockaddr*) &serv_addr, sizeof(serv_addr));
        }
        catch(...)
        {
            std::cout << "[TC_Socket::bind] bind error" << std::endl;
        }
    }

    void TC_Socket::bind(struct sockaddr *s_addr, socklen_t s_addr_len)
    {
        int reuse = 1;

        setSockOpt(SO_REUSEADDR, (const void *)&reuse, sizeof(int), SOL_SOCKET);

        if(::bind(_sock, s_addr, s_addr_len))
        {
            std::cout << "[TC_Socket::bind] bind error2" << std::endl;
        }
    }

    void TC_Socket::parseaddr(const std::string &ip, struct in_addr &s_addr)
    {
        int ret = inet_pton(_Domain, ip.c_str(), &s_addr);
        if(ret < 0)
        {
            std::cout << "[TC_Socket::parseAddr] inet_pton error" << std::endl;;
        }
        else if(ret == 0)
        {
            struct hostent stHostent;
            struct hostent *pstHostent;
            char buf[2048] = "\0";
            int iError;

            gethostbyname_r(ip.c_str(), &stHostent, buf, sizeof(buf), &pstHostent, &iError);

            if (pstHostent == nullptr)
            {
                std::cout << "[TC_Socket::parseAddr] gethostbyname_r error! :" << std::endl;
            }
            else
            {
                s_addr = *(struct in_addr *) pstHostent->h_addr;
            }
        }
    }

    int TC_Socket::setSockOpt(int opt, const void *pvOptVal, socklen_t optLen, int level) {
        return setsockopt(_sock, level, opt, pvOptVal, optLen);
    }

    void TC_Socket::listen(int conn)
    {
        if(::listen(_sock, conn) < 0)
        {
            std::cout << "[TC_Socket::listen] listen error" << std::endl;
        }
    }

    void TC_Socket::setKeepAlive()
    {
        int flag = 1;
        if(setSockOpt(SO_KEEPALIVE, (char *) &flag, int(sizeof(int)), SOL_SOCKET) == -1)
        {
            std::cout << "[TC_Socket::setKeepAlive] error" << std::endl;
        }
    }

    void TC_Socket::setTcpNoDelay()
    {
        int flag = 1;

        if(setSockOpt(TCP_NODELAY, (char*)&flag, int(sizeof(int)), IPPROTO_TCP) == -1)
        {
            std::cout << "[TC_Socket::setTcpNoDelay] error" << std::endl;
        }
    }

    void TC_Socket::setNoCloseWait()
    {
        linger stLinger;
        stLinger.l_onoff = 1;  //在close socket调用后, 但是还有数据没发送完毕的时候容许逗留
        stLinger.l_linger = 0; //容许逗留的时间为0秒

        if(setSockOpt(SO_LINGER, (const void *)&stLinger, sizeof(linger), SOL_SOCKET) == -1)
        {
            std::cout << "[TC_Socket::setNoCloseWait] error" << std::endl;
        }
    }

    void TC_Socket::setblock(bool flag)
    {
        assert(_sock != INVALID_SOCKET);
        setblock(_sock, flag);
    }

    void TC_Socket::setblock(int fd, bool flag)
    {
        int val = 0;

        if((val = fcntl(fd, F_GETFL, 0)) == -1)
        {
            std::cout << "[TC_Socket::setblock] fcntl [F_GETFL] error" << std::endl;
        }

        if(!flag)
        {
            val |= O_NONBLOCK;
        }
        else
        {
            val &= ~O_NONBLOCK;
        }

        if(fcntl(fd, F_SETFL, val) == -1)
        {
            std::cout << "[TC_Socket::setblock] fcntl [F_SETFL] error" << std::endl;
        }

    }

    void TC_Socket::init(int fd, bool bOwner, int iDomain)
    {
        if(_bOwner)
        {
            close();
        }

        _sock       = fd;
        _bOwner     = bOwner;
        _Domain    = iDomain;
    }

    int TC_Socket::accept(TC_Socket &tcSock, struct sockaddr *pstSockAddr, socklen_t &iSockLen)
    {
        assert(tcSock._sock == INVALID_SOCKET);

        int ifd;

        while ((ifd = ::accept(_sock, pstSockAddr, &iSockLen)) < 0 && errno == EINTR);

        tcSock._sock    = ifd;
        tcSock._Domain = _Domain;

        return tcSock._sock;
    }

    void TC_Socket::setCloseWaitDefault()
    {
        linger stLinger;
        stLinger.l_onoff  = 0;
        stLinger.l_linger = 0;

        if(setSockOpt(SO_LINGER, (const void *)&stLinger, sizeof(linger), SOL_SOCKET) == -1)
        {
            std::cout << "[TC_Socket::setCloseWaitDefault] error" << std::endl;
        }
    }
}
