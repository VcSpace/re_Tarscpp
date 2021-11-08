#include "tc_epoll_server.h"

namespace tars
{
    NetThread::NetThread()
    {
        _sock
    }

    NetThread::~NetThread() {}

    void NetThread::bind(std::string &ip, int port)
    {
        int iSocketType = SOCK_STREAM;
        int iDomain = AF_INET;

        _sock = socket(AF_INET, SOCK_STREAM, 0);

        if(_sock < 0)
        {
            std::cerr << "bind _sock faild" << std::endl;
        }

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
    }
}
