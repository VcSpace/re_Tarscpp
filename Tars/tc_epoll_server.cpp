#include "tc_epoll_server.h"

namespace tars
{
    #define H64(x) (((uint64_t)x) << 32)

    NetThread::NetThread()
    {
        _shutdown.createSocket();
        _notify.createSocket();

        _response.response="";
        _response.uid = 0;
    }

    NetThread::~NetThread()
    {

    }

    void NetThread::bind(std::string &ip, int port)
    {
        int iSocketType = SOCK_STREAM;
        int iDomain = AF_INET;

        _bind_listen.createSocket(AF_INET, SOCK_STREAM);
        _bind_listen.bind(ip,port);
        _bind_listen.listen(1024);

    }
}
