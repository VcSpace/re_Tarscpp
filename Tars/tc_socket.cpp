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

    void TC_Socket::createsock(int Domain, int type)
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
}
