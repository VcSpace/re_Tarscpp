#ifndef LEARN_TARSCPP_TC_EPOLLER_H
#define LEARN_TARSCPP_TC_EPOLLER_H

#include <sys/epoll.h>
#include <cassert>
#include <unistd.h>

namespace tars
{
    class TC_Epoller {
    public:
        TC_Epoller(bool bET = true);
        ~TC_Epoller();
        void create(int max_connections);

    private:
        int _iEpollfd;
        bool _et;
        struct epoll_event *_pevs;
        int    _max_connections;

    };
}

#endif //