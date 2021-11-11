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

    NetThread::~NetThread() {}

    void NetThread::bind(std::string &ip, int port)
    {
        int iSocketType = SOCK_STREAM;
        int iDomain = AF_INET;

        _bind_listen.createSocket(AF_INET, SOCK_STREAM);
        _bind_listen.bind(ip,port);
        _bind_listen.listen(1024);


        struct sockaddr_in addr;
        bzero(&addr, sizeof(addr));

        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        if(ip == "")
        {
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
        } else{
            parseAddr(ip, addr.sin_addr);
        }

        int reuse = 1;
        setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, (const void *) &addr, sizeof(int));

        if(::bind(_sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
        {
            std::cerr << "bind error" << std::endl;
        }

        if(listen(_sock, 100) < 0)
        {
            std::cout << "listen error" << std::endl;
        }

        int flag = 1;
        if(setsockopt(_sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&flag, sizeof(int)) == -1)
        {
            std::cout << "setKeepAlive error" << std::endl;
        }

        flag=1;
        if(setsockopt(_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int)) == -1)
        {
            std::cout << "[TC_Socket::setTcpNoDelay] error" << std::endl;
        }

        linger stLinger;
        stLinger.l_onoff = 1; //在close socket调用后, 但是还有数据没发送完毕的时候容许逗留
        stLinger.l_linger = 0; //容许逗留的时间为0秒

        if(setsockopt(_sock, SOL_SOCKET, SO_LINGER, (const void *)&stLinger, sizeof(linger)) == -1)
        {
            std::cout << "[TC_Socket::setNoCloseWait] error" << std::endl;
        }

        //设置为非阻塞
        int val = 0;
        bool bBlock = false;

        if ((val = fcntl(_sock, F_GETFL, 0)) == -1)
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

        if (fcntl(_sock, F_SETFL, val) == -1)
        {
            std::cout << "fcntl nonblock error" << std::endl;
        }
        else
        {
            ;
        }
    }

    void NetThread::parseAddr(const std::string &ip, struct in_addr &seraddr)
    {
        int ret = inet_pton(AF_INET, ip.c_str(), &seraddr);
        if(ret < 0)
        {
            std::cerr << "set ip error" << std::endl;
        }
        else if(ret == 0) //域名
        {
            struct hostent stHostent;
            struct hostent *pstHostent;
            /*
             * http://xsh8637.blog.163.com/blog/static/24099666201081634847407/
             * struct hostent{
                char *h_name;  //official name
                char **h_aliases;  //alias list
                int  h_addrtype;  //host address type
                int  h_length;  //address lenght
                char **h_addr_list;  //address list
            }
             */
            char buf[4096] = "\0";
            int iError;

            gethostbyname_r(ip.c_str(), &stHostent, buf, sizeof(buf), &pstHostent, &iError);
            if (pstHostent == NULL)
            {
                std::cout << "gethostbyname_r error! :" << std::endl;
            }
            else
            {
                seraddr = *(struct in_addr *) pstHostent->h_addr;
            }
        }
    }

    void NetThread::createEpoll(uint32_t iIndex)
    {
        int _total = 200000;

        _epoller.create(10240);

        _epoller.add(_shutdown_sock, H64(ET_CLOSE), EPOLLIN);

        _epoller.add(_notify_sock, H64(ET_NOTIFY), EPOLLIN);

        _epoller.add(_sock, H64(ET_LISTEN) | _sock, EPOLLIN);

        for(uint32_t i = 1; i <= _total; i++)
        {
            _free.push_back(i);
            ++_free_size;
        }


        std::cout << "epoll create successful" << std::endl;
    }

    bool NetThread::accept(int fd)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        int iifd;
        while((iifd = ::accept(_sock, (struct sockaddr *) &client_addr, &client_addr_len)) < 0 && errno == EINTR);

        std::cout << "server accept successful fd is " << iifd << std::endl;
        if(iifd < 0)
        {
            return false;
        }
        else
        {
            std::string ip;
            uint16_t port;
            char sAddr[INET_ADDRSTRLEN] = "\0";

            struct sockaddr_in *p = (struct sockaddr_in *) &client_addr;

            inet_ntop(AF_INET, &p->sin_addr, sAddr, sizeof(sAddr));

            ip = sAddr;
            port = ntohs(p->sin_port);

            //setblock
            int val = 0;
            int bBlock = false;
            if ((val = fcntl(iifd, F_GETFL, 0)) == -1) {
                std::cout << "F_GETFL error" << std::endl;
            }

            if (!bBlock) {
                val |= O_NONBLOCK;
            } else {
                val &= ~O_NONBLOCK;
            }

            if (fcntl(iifd, F_SETFL, val) == -1) {
                std::cout << "F_SETFL error" << std::endl;
            }

            //keepAlive
            int flag = 1;
            if (setsockopt(iifd, SOL_SOCKET, SO_KEEPALIVE, (char *) &flag, int(sizeof(int))) == -1) {
                std::cout << "[TC_Socket::setKeepAlive] error" << std::endl;
            }

            //nodelay
            if (setsockopt(iifd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, int(sizeof(int))) == -1) {
                std::cout << "[TC_Socket::setTcpNoDelay] error" << std::endl;
            }

            //closeWait
            linger stLinger;
            stLinger.l_onoff = 0;
            stLinger.l_linger = 0;

            if (setsockopt(iifd, SOL_SOCKET, SO_LINGER, (const void *) &stLinger, sizeof(linger)) == -1) {
                std::cout << "[TC_Socket::setCloseWaitDefault] error" << std::endl;
            }

            uint32_t uid = _free.front();

            _free.pop_front();

            --_free_size;

            //保存connectid
            _listen_connect_id[uid] = iifd;

            //注册到epoll模型
            _epoller.add(iifd, uid, EPOLLIN | EPOLLOUT);
            return true;
        }
    }

    void NetThread::processPipe()
    {
        uint32_t uid = _response.uid;

        int fd = _listen_connect_id[uid];

        std::cout << "processPipe uid is " << uid << " fd is " << fd << std::endl;

        int bytes = ::send(fd, _response.response.c_str(), _response.response.size(), 0);

        std::cout << "send byte is " << bytes << std::endl;
        std::cout << "response is " << _response.response << std::endl;
    }

    void NetThread::processNet(const epoll_event &ev)
    {
        uint32_t uid = ev.data.u32;

        int fd = _listen_connect_id[uid];

        std::cout << "processNet uid is " << uid << " fd is " << fd << std::endl;

        if (ev.events & EPOLLERR || ev.events & EPOLLHUP)
        {
            std::cout << "should delete connection" << std::endl;
            return;
        }

        if(ev.events & EPOLLIN)
        {
            while(true)
            {
                char buffer[32 * 1024];
                int iBytesReceived = 0;

                iBytesReceived = ::read(fd, (void*)buffer, sizeof(buffer));
                std::cout << "server recieve " << iBytesReceived << " bytes buffer is " << buffer << std::endl;

                if(iBytesReceived < 0)
                {
                    if(errno == EAGAIN)
                    {
                        break;
                    }
                    else
                    {
                        std::cout << "client close" << std::endl;
                        return;
                    }
                }
                else if( iBytesReceived == 0 )
                {
                    std::cout << "1 client close" << std::endl;
                    return;
                }

                _recvbuffer.append(buffer, iBytesReceived);

                _response.response = "hello";
                _response.uid = uid;

                _epoller.mod(_notify_sock, H64(ET_NOTIFY), EPOLLOUT);

            }

            if (ev.events & EPOLLOUT)
            {
                //这里是处理上次未发送完的数据
                std::cout << "need to send data" << std::endl;
            }
        }
    }

    void NetThread::run()
    {
        std::cout << "NetThread run" << std::endl;

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
                        {
                            if(ev.events & EPOLLIN)
                            {
                                bool ret;
                                do
                                {
                                    ret = accept(ev.data.u32);
                                }while(ret);
                            }
                        }
                        break;
                    case ET_CLOSE:
                        std::cout << "ET_CLOSE" << std::endl;
                        break;
                    case ET_NOTIFY:
                        std::cout << "ET_NOTIFY" << std::endl;
                        processPipe();
                        break;
                    case ET_NET:
                        std::cout << "ET_NET" << std::endl;
                        processNet(ev);
                        break;
                    default:
                        assert(true);
                }
            }
        }
    }
}
