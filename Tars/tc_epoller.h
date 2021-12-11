#ifndef RETARS_TC_EPOLLER_H
#define RETARS_TC_EPOLLER_H

#include <unistd.h>
#include <sys/epoll.h>

namespace tars
{
    class TC_Epoller
    {
    public:
        TC_Epoller(bool et = true);
        ~TC_Epoller();

        void createepoll(int conn);

        void add();
        void mod();
        void del();

    private:
        bool _et;

        int _max_connections;
        int _iEpollfd;

        struct epoll_event *_pevs;
    };
}

#endif //RETARS_TC_EPOLLER_H