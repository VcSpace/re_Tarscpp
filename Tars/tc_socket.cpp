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

    }

    void TC_Socket::listen(int connBackLog)
    {

    }

}