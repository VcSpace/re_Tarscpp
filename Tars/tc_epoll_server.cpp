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

    int NetThread::bind(std::string &ip, int port)
    {
        int iSocketType = SOCK_STREAM;
        int iDomain = AF_INET;

        _bind_listen.createSocket(AF_INET, SOCK_STREAM);
        _bind_listen.bind(ip,port);
        _bind_listen.listen(1024);
        std::cout << "server alreay listen fd is " << _bind_listen.getfd() << std::endl;

        _bind_listen.setKeepAlive();
        _bind_listen.setTcpNoDelay();
        //不要设置close wait否则http服务回包主动关闭连接会有问题
        _bind_listen.setNoCloseWait();
        _bind_listen.setblock(false);

        return _bind_listen.getfd();
    }

    void NetThread::createEpoll(uint32_t iIndex)
    {
        int _total = 20000;
        _epoller.create(10240);

        for(uint32_t i = 1; i <= _total; i++)
        {
            _free.push_back(i);

            ++_free_size;
        }

        std::cout << "epoll create successful" << std::endl;
    }

    void NetThread::run()
    {
        std::cout << "Server run" << std::endl;

        while(true)
        {
            int iEvNum = _epoller.wait(2000);

            for(int i = 0; i < iEvNum; ++i)
            {
                const epoll_event &ev = _epoller.get(i);

                uint32_t h = ev.data.u64 >> 32;

                switch(h)
                {
                    case ET_LISTEN:
                        std::cout << "ET_LISTEN" << std::endl;

                        if(ev.events. & EPOLLIN)
                        {
                            bool ret = false;

                            do {
                                ret = accept(ev.data.u32);
                            } while(ret);
                        }
                        break;
                    case ET_CLOSE:
                        std::cout << "ET_CLOSE" << std::endl;
                        break;
                    case ET_NOTIFY:
                        processPipe();
                        break;
                }
            }
        }
    }

    void NetThread::processPipe()
    {
        uint32_t uid = _response.uid;
        int fd = _listen_connect_id[uid];
        std::cout << "processPipe uid is " << uid << " fd is " << fd << std::endl;

        int bytes = ::send(fd, _response.response.c_str(), _response.response.size(), 0);

        std::cout << "send byte is " << bytes << " response is " << _response.response << std::endl;
    }
}
