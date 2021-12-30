#include "tc_epoll_server.h"


namespace tars
{
#define H64(x) (((uint64_t)x) << 32)

    TC_EpollServer::TC_EpollServer()
    {
        _netThreads = new TC_EpollServer::NetThread(this);
    }

    TC_EpollServer::~TC_EpollServer()
    {
        delete _netThreads;
    }

    void TC_EpollServer::send(unsigned int uid, const std::string &s, const std::string &ip, uint16_t port, int fd)
    {
        _netThreads->send(uid, s, ip, port);
    }

    int TC_EpollServer::bind(TC_EpollServer::BindAdapterPtr &lsPtr)
    {
        int ret = 0;
        ret = _netThreads->bind(lsPtr);
        return ret;
    }

    TC_EpollServer::NetThread::NetThread(TC_EpollServer *epollServer) : _epollServer(epollServer)
    {
        _shutdown.createSocket();
        _notify.createSocket();

        _response.response="";
        _response.uid = 0;
    }

    TC_EpollServer::NetThread::~NetThread()
    {
    }

//    void TC_EpollServer::NetThread::bind(std::string &ip, int port)
//    {
//        int Domain = AF_INET;
//        int type = SOCK_STREAM;
//        _bind_listen.createsock(Domain, type);
//        _bind_listen.bind(ip, port);
//        _bind_listen.listen(1024);
//
//        std::cout << "server alreay listen fd is " << _bind_listen.getfd() << std::endl;
//
//        _bind_listen.setKeepAlive();
//        _bind_listen.setTcpNoDelay();
//        _bind_listen.setNoCloseWait();
//        _bind_listen.setblock(false);
//    }

    void TC_EpollServer::NetThread::createEpoll(uint32_t iIndex)
    {
        int _total = 200000;
        _epoller.createepoll(10240);

        _epoller.add(_shutdown.getfd(), H64(ET_CLOSE), EPOLLIN);

        _epoller.add(_notify.getfd(), H64(ET_NOTIFY), EPOLLIN);

        _epoller.add(_bind_listen.getfd(), H64(ET_LISTEN) | _bind_listen.getfd(), EPOLLIN);

        for (const auto& kv : _listeners)
        {
            _epoller.add(kv.first, H64(ET_LISTEN) | kv.first, EPOLLIN);
        }

        for(uint32_t i = 1; i <=_total; i++)
        {
            _free.push_back(i);
            ++_free_size;
        }

        std::cout << "epoll create successful" << std::endl;
    }

    bool TC_EpollServer::NetThread::waitForRecvQueue(TC_EpollServer::tagRecvData *&recv, uint32_t iWaitTime)
    {
//        std::cout << "NetThread::waitForRecvQueue" << std::endl;

        bool bRet = false;

        bRet = _rbuffer.pop_front(recv, iWaitTime);

        if(!bRet)
        {
            return bRet;
        }

        return bRet;
    }

    void TC_EpollServer::NetThread::send(unsigned int uid, const std::string &s, const std::string &ip, uint16_t port)
    {
        tagSendData* send = new tagSendData();

        send->uid = uid;

        send->cmd = 's';

        send->buffer = s;

        send->ip = ip;

        send->port = port;

        _sbuffer.push_back(send);

        //通知epoll响应, 有数据要发送
        _epoller.mod(_notify.getfd(), H64(ET_NOTIFY), EPOLLOUT);
    }

    void TC_EpollServer::NetThread::run()
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

    int TC_EpollServer::NetThread::accept(int fd)
    {
        struct sockaddr_in stSockAddr;

        socklen_t iSockAddrSize = sizeof(sockaddr_in);

        TC_Socket cs;
        cs.setOwner(false);

        //接收连接
        TC_Socket s;
        s.init(fd, false, AF_INET);

        int iRetCode = s.accept(cs, (struct sockaddr *) &stSockAddr, iSockAddrSize);

        if (iRetCode > 0)
        {
            std::string  ip;

            uint16_t port;

            char sAddr[INET_ADDRSTRLEN] = "\0";

            struct sockaddr_in *p = (struct sockaddr_in *)&stSockAddr;

            inet_ntop(AF_INET, &p->sin_addr, sAddr, sizeof(sAddr));

            ip      = sAddr;
            port    = ntohs(p->sin_port);

            std::cout << "accept ip is " << ip << " port is " << port << std::endl;

            cs.setblock(false);
            cs.setKeepAlive();
            cs.setTcpNoDelay();
            cs.setCloseWaitDefault();

            uint32_t uid = _free.front();

            _free.pop_front();

            --_free_size;

            _listen_connect_id[uid] = cs.getfd();

            std::cout << "server accept successful fd is " << cs.getfd() << std::endl;

            _epoller.add(cs.getfd(), uid, EPOLLIN | EPOLLOUT);

        }
        else
        {
            if(errno == EAGAIN)
            {
                return false;
            }
            return true;
        }
        return true;
    }

    void TC_EpollServer::NetThread::processPipe()
    {
        send_queue::queue_type deSendData;
        _sbuffer.swap(deSendData);
        send_queue::queue_type::iterator it = deSendData.begin();
        send_queue::queue_type::iterator itEnd = deSendData.end();

        while(it != itEnd)
        {
            switch((*it)->cmd)
            {
                case 's':
                {
                    uint32_t uid = (*it)->uid;

                    int fd = _listen_connect_id[uid];

                    std::cout << "processPipe uid is " << uid << " fd is " << fd << std::endl;

                    int bytes = ::send(fd, (*it)->buffer.c_str(), (*it)->buffer.size(), 0);

                    std::cout << "send byte is " << bytes << std::endl;

                    break;
                }
                default:
                    assert(false);
            }
            delete(*it);
            ++it;
        }
    }

    void TC_EpollServer::NetThread::processNet(const epoll_event &ev) {
        uint32_t uid = ev.data.u32;
        int fd = _listen_connect_id[uid];
        std::cout << "processNet uid is " << uid << " fd is " << fd << std::endl;

        if (ev.events & EPOLLERR || ev.events & EPOLLHUP) {
            std::cout << "should delete connection" << std::endl;
            return;
        }

        if (ev.events & EPOLLIN)
        {
            recv_queue::queue_type vRecvData;

            while(true)
            {
                char buffer[32*1024];
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
                        return ;
                    }
                }
                else if( iBytesReceived == 0 )
                {
                    std::cout << "1 client close" << std::endl;
                    return ;
                }

                _recvbuffer.append(buffer, iBytesReceived);
            }
            if(!_recvbuffer.empty())
            {
                tagRecvData* recv = new tagRecvData();
                recv->buffer           = std::move(_recvbuffer);
                recv->ip               = "";
                recv->port             = 0;
                recv->recvTimeStamp    = 0;
                recv->uid              = uid;
                recv->isOverload       = false;
                recv->isClosed         = false;
                recv->fd               = fd;

                vRecvData.push_back(recv);
            }

            if(!vRecvData.empty())
            {
                std::cout << "insertRecvQueue" << std::endl;
                insertRecvQueue(vRecvData);
            }
        }

        if (ev.events & EPOLLOUT)
        {
            std::cout<< "need to send data" << std::endl;
        }

    }

    void TC_EpollServer::NetThread::insertRecvQueue(const std::deque<TC_EpollServer::tagRecvData *> &vtRecvData, bool bPushBack)
    {
        {
            if (bPushBack)
            {
                _rbuffer.push_back(vtRecvData);
            }
            else
            {
                _rbuffer.push_front(vtRecvData);
            }
        }

        TC_ThreadLock::Lock lock(monitor);

        monitor.notify();
    }

    int TC_EpollServer::NetThread::bind(TC_EpollServer::BindAdapterPtr &lsPtr)
    {
        const TC_Endpoint &ep = lsPtr->getEndpoint();

        TC_Socket& s = lsPtr->getSocket();

        bind(ep, s);

        _listeners[s.getfd()] = lsPtr;

        return s.getfd();
    }

    void TC_EpollServer::NetThread::bind(const TC_Endpoint &ep, TC_Socket &s)
    {
        int type = AF_INET;

        s.createSocket(type, SOCK_STREAM);

        s.bind(ep.getHost(), ep.getPort());

        s.listen(1024);
        s.setKeepAlive();
        s.setTcpNoDelay();
        //不要设置close wait否则http服务回包主动关闭连接会有问题
        s.setNoCloseWait();

        s.setblock(false);
    }

    TC_Endpoint TC_EpollServer::BindAdapter::getEndpoint() const
    {
        return _ep;
    }

    void TC_EpollServer::BindAdapter::setEndpoint(const std::string &str, const int &port)
    {
        TC_ThreadLock::Lock lock(*this);
        _ep.init(str, port);
    }

    TC_Socket &TC_EpollServer::BindAdapter::getSocket()
    {
        return _s;
    }

    TC_EpollServer::BindAdapter::BindAdapter(TC_EpollServer *pEpollServer) :_pEpollServer(pEpollServer)
    {

    }

    TC_EpollServer::BindAdapter::~BindAdapter() {

    }

    TC_EpollServer::Handle::Handle() : _pEpollServer(NULL), _iWaitTime(100)
    {

    }

    TC_EpollServer::Handle::~Handle() {

    }

    void TC_EpollServer::Handle::run()
    {
        std::cout << "handle run" << std::endl;

        initialize();
        handleImp();
    }

    void TC_EpollServer::Handle::setEpollServer(TC_EpollServer *eserv)
    {
        TC_ThreadLock::Lock lock(*this);
        //std::cout << "class name = " << typeid(*this).name() << std::endl;
        _pEpollServer = eserv;
    }

    void TC_EpollServer::Handle::handleImp()
    {
        std::cout << "Handle::handleImp" << std::endl;
        tagRecvData *recv = nullptr;
        while(true)
        {
            {
                TC_EpollServer::NetThread* netThread = _pEpollServer->getNetThread();
                TC_ThreadLock::Lock lock(netThread->monitor);
                netThread->monitor.timedWait(100);
            }

            while(waitForRecvQueue(recv, 0))
            {
                std::cout << "handleImp recv uid  is " << recv->uid << std::endl;
                _pEpollServer->send(recv->uid,recv->buffer, "0", 0, 0);

            }
        }

    }

    bool TC_EpollServer::Handle::waitForRecvQueue(TC_EpollServer::tagRecvData *&recv, uint32_t iWaitTime)
    {
        TC_EpollServer::NetThread *pNetThread = _pEpollServer->getNetThread();

        return pNetThread->waitForRecvQueue(recv, iWaitTime);
    }
}