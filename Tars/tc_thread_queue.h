#ifndef RETARS_TC_THREAD_QUEUE_H
#define RETARS_TC_THREAD_QUEUE_H

#include <deque>
#include <vector>
#include <cassert>

#include "tc_monitor.h"
#include "tc_thread_lock.h"

namespace tars
{
    template<typename T, typename D = std::deque<T> >
    class TC_ThreadQueue : protected TC_ThreadLock
    {
    public:
        typedef D queue_type;

        TC_ThreadQueue() : _size(0) {};
        virtual ~TC_ThreadQueue() {};

        void push_back(const T& t);
        void push_back(const queue_type &qt);
        void push_front(const T& t);
        void push_front(const queue_type &qt);

        bool pop_front(T& t, size_t millsecond = 0);

    protected:
        queue_type          _queue;
        size_t              _size;

    };

    template<typename T, typename D>
    bool TC_ThreadQueue<T, D>::pop_front(T &t, size_t millsecond)
    {
        Lock lock(*this);

        if (_queue.empty())
        {
            if(millsecond == 0)
            {
                return false;
            }
            if(millsecond == (size_t)-1)
            {
                std::cout << "pop_front wait begin" << std::endl;
                wait();
                std::cout << "pop_front wait end" << std::endl;
            }
            else
            {
                //超时了
                std::cout << "pop_front timedWait begin" << std::endl;
                if(!timedWait(millsecond))
                {
                    return false;
                }
            }
        }

        if(_queue.empty()) //deque
        {
            return false;
        }
        t = _queue.front();
        _queue.pop_front();
        assert(_size > 0);
        --_size;

        return true;

    }

    template<typename T, typename D>
    void TC_ThreadQueue<T, D>::push_back(const T &t) {

    }

} //tars
#endif //RETARS_TC_THREAD_QUEUE_H
