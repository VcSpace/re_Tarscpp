#ifndef RETARS_TC_EPOLLER_H
#define RETARS_TC_EPOLLER_H

#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>

namespace tars
{
    class TC_Epoller
    {
    public:
        TC_Epoller(bool et = true);
        ~TC_Epoller();

        void createepoll(int conn);
        /**
         * @brief 添加监听句柄.
         *
         * @param fd    句柄
         * @param data  辅助的数据, 可以后续在epoll_event中获取到
         * @param event 需要监听的事件EPOLLIN|EPOLLOUT
         *
         */
        void add(int fd, long long data, __uint32_t event);

        /**
         * @brief 修改句柄事件.
         *
         * @param fd    句柄
         * @param data  辅助的数据, 可以后续在epoll_event中获取到
         * @param event 需要监听的事件EPOLLIN|EPOLLOUT
         */
        void mod(int fd, long long data, __uint32_t event);

        /**
         * @brief 删除句柄事件.
         *
         * @param fd    句柄
         * @param data  辅助的数据, 可以后续在epoll_event中获取到
         * @param event 需要监听的事件EPOLLIN|EPOLLOUT
         */
        void del(int fd, long long data, __uint32_t event);

        void ctrl(int fd, long long int data, __uint32_t events, int mode);


    private:
        bool _et;

        int _max_connections;
        int _iEpollfd;

        struct epoll_event *_pevs;
    };
}

#endif //RETARS_TC_EPOLLER_H