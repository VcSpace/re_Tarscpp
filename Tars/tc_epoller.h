#ifndef LEARN_TARSCPP_TC_EPOLLER_H
#define LEARN_TARSCPP_TC_EPOLLER_H

#include "socket.h"

namespace tars
{
    class TC_Epoller {
    public:
        TC_Epoller(bool bET = true);
        ~TC_Epoller();
        void create(int max_connections);
        void ctrl(int fd, long long data, __uint32_t event, int op);
        void add(int fd, long long data, __uint32_t event);
        void mod(int fd, long long data, __uint32_t event);
        void del(int fd, long long data, __uint32_t event);
        int wait(int millsecond);

        struct epoll_event& get(int i) { assert(_pevs != 0); return _pevs[i]; }

    private:
        int _iEpollfd;
        bool _et;
        struct epoll_event *_pevs;
        int    _max_connections;

    };
}

#endif //