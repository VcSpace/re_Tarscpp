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
        TC_ThreadQueue() : _size(0) {};
        virtual ~TC_ThreadQueue() {};

        typedef D queue_type;

    protected:
        queue_type          _queue;
        size_t              _size;

    };

} //tars
#endif //RETARS_TC_THREAD_QUEUE_H
