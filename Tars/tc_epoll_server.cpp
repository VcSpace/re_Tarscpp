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

    }

    TC_EpollServer::NetThread::NetThread(TC_EpollServer *epollServer) : _epollServer(epollServer)
    {
        _shutdown.createsock();
        _notify.createsock();

        _response.response="";
        _response.uid = 0;
    }

    TC_EpollServer::NetThread::~NetThread() {}

    void TC_EpollServer::NetThread::bind(std::string &ip, int port)
    {
        int Domain = AF_INET;
        int type = SOCK_STREAM;
        _bind_listen.createsock(Domain, type);
        _bind_listen.bind(ip, port);
        _bind_listen.listen(1024);

        std::cout << "server alreay listen fd is " << _bind_listen.getfd() << std::endl;

        _bind_listen.setKeepAlive();
        _bind_listen.setTcpNoDelay();
        _bind_listen.setNoCloseWait();
        _bind_listen.setblock(false);
    }

    void TC_EpollServer::NetThread::createEpoll(uint32_t iIndex)
    {
        int _total = 200000;
        _epoller.createepoll(10240);

        _epoller.add(_shutdown.getfd(), H64(ET_CLOSE), EPOLLIN);

        _epoller.add(_notify.getfd(), H64(ET_NOTIFY), EPOLLIN);

        _epoller.add(_bind_listen.getfd(), H64(ET_LISTEN) | _bind_listen.getfd(), EPOLLIN);

        for(uint32_t i = 1; i <=_total; i++)
        {
            _free.push_back(i);

            ++_free_size;
        }

        std::cout << "epoll create successful" << std::endl;
    }

    TC_EpollServer::Handle::Handle() {

    }

    TC_EpollServer::Handle::~Handle() {

    }

    void TC_EpollServer::Handle::run()
    {
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
        }

    }
}