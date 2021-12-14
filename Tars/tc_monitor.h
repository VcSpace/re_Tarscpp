#ifndef RETARS_TC_MONITOR_H
#define RETARS_TC_MONITOR_H

#include "tc_thread_lock.h"
#include "tc_thread_mutex.h"

namespace tars
{
    template <typename T, typename U>
    class TC_Monitor
    {
    public:
        typedef TC_LockT<TC_Monitor<T, U> > Lock;
        typedef TC_TryLockT<TC_Monitor<T, U> > TryLock;

        TC_Monitor();
        virtual ~TC_Monitor();

        virtual void lock() const;

    private:

    };

    template<typename T, typename U>
    TC_Monitor<T, U>::TC_Monitor() {

    }

    template<typename T, typename U>
    TC_Monitor<T, U>::~TC_Monitor() {

    }

    template<typename T, typename U>
    void TC_Monitor<T, U>::lock() const {

    }

    typedef TC_Monitor<TC_ThreadMutex, TC_ThreadCond> TC_ThreadLock;

} //tars


#endif //RETARS_TC_MONITOR_H
