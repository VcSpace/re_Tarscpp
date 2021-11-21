#include "tc_socket.h"

namespace tars
{
    TC_Socket::TC_Socket() : _sock(INVALID_SOCKET), _bOwner(true), _iDomain(AF_INET)
    {

    }

    TC_Socket::~TC_Socket()
    {

    }

    void TC_Socket::close()
    {
        if (_sock != INVALID_SOCKET)
        {
            ::close(_sock);
            _sock = INVALID_SOCKET;
        }
    }

    void TC_Socket::createSocket(int iSocketType, int iDomain)
    {
        assert(iSocketType == SOCK_STREAM || iSocketType == SOCK_DGRAM);
        close();

        _iDomain = iDomain;
        _sock = socket(_iDomain, iSocketType, 0);

        if(_sock < 0)
        {
            _sock = INVALID_SOCKET;
            std::cout << "create sock error " << std::endl;
        }
    }

    void TC_Socket::bind(const std::string &ip, int port)
    {
        struct sockaddr_in saddr;
        bzero(&saddr, sizeof(saddr));

        saddr.sin_family = _iDomain;
        saddr.sin_port = htons(port);

        if(ip == " ")
        {
            saddr.sin_addr.s_addr = htonl(INADDR_ANY);
        }
        else
        {
            parseAddr(ip, saddr.sin_addr);
        }

        try
        {
            bind((struct sockaddr *)(&saddr), sizeof(saddr));
        }
        catch(...)
        {
            std::cout << "[TC_Socket::bind] bind error" << std::endl;
        }
    }

    void TC_Socket::bind(struct sockaddr *addr, socklen_t socklen)
    {
        //如果服务器终止后,服务器可以第二次快速启动而不用等待一段时间
        int iReuseAddr = 1;

        //设置
        setSockOpt(SO_REUSEADDR, (const void *)&iReuseAddr, sizeof(int), SOL_SOCKET);

        if(::bind(_sock, addr, socklen) < 0)
        {
            std::cout << "[TC_Socket::bind] bind error" << std::endl;
        }
    }

    int TC_Socket::setSockOpt(int opt, const void *pvOptVal, socklen_t optLen, int level)
    {
        return setsockopt(_sock, level, opt, pvOptVal, optLen);
    }


    void TC_Socket::parseAddr(const std::string &ip, struct in_addr &seraddr)
    {
        int iRet = inet_pton(AF_INET, ip.c_str(), &seraddr);
        if(iRet < 0)
        {
            std::cout << "[TC_Socket::parseAddr] inet_pton error" << std::endl;;
        }
        else if(iRet == 0)
        {
            struct hostent stHostent;
            struct hostent *pstHostent;

            char buf[2048] = "\0";
            int iError;

            gethostbyname_r(ip.c_str(), &stHostent, buf, sizeof(buf), &pstHostent, &iError);

            if(pstHostent == nullptr)
            {
                std::cout << "[TC_Socket::parseAddr] gethostbyname_r error! :" << std::endl;
            }
            else
            {
                seraddr = *(struct in_addr *)pstHostent->h_addr;
            }
        }
    }

    void TC_Socket::listen(int iConnBackLog)
    {
        if (::listen(_sock, iConnBackLog) < 0)
        {
            std::cout << "[TC_Socket::listen] listen error" << std::endl;
        }
    }

    void TC_Socket::setKeepAlive()
    {
        int flag = 1;
        if(setSockOpt(SO_KEEPALIVE, (char*)&flag, int(sizeof(int)), SOL_SOCKET) == -1)
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

    void TC_Socket::setblock(bool bBlock)
    {
        assert(_sock != INVALID_SOCKET);

        setblock(_sock, bBlock);
    }

    void TC_Socket::setblock(int fd, bool bBlock)
    {
        int val = 0;

        if ((val = fcntl(fd, F_GETFL, 0)) == -1)
        {
            std::cout << "[TC_Socket::setblock] fcntl [F_GETFL] error" << std::endl;
        }

        if(!bBlock)
        {
            val |= O_NONBLOCK;
        }
        else
        {
            val &= ~O_NONBLOCK;
        }

        if (fcntl(fd, F_SETFL, val) == -1)
        {
            std::cout << "[TC_Socket::setblock] fcntl [F_SETFL] error" << std::endl;
        }
    }

    void TC_Socket::init(int _fd, bool bOwner, int iDomain)
    {
        if(_bOwner)
        {
            close();
        }

        _sock = _fd;
        _bOwner = bOwner;
        _iDomain = iDomain;
    }

    int TC_Socket::accept(TC_Socket &tcSock, struct sockaddr *pstSockAddr, socklen_t &iSockLen)
    {
        assert(tcSock._sock == INVALID_SOCKET);

        int ifd;

        while((ifd = ::accept(_sock, pstSockAddr, &iSockLen)) < 0 && errno == EINTR);

        tcSock._sock    = ifd;
        tcSock._iDomain = _iDomain;

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