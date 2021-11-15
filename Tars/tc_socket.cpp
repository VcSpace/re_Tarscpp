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

    void TC_Socket::bind(struct sockaddr &addr, int socklen)
    {

    }

    void TC_Socket::listen(int connBackLog)
    {

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

}