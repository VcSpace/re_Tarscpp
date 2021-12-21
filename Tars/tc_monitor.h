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

        void lock() const;
        void trylock() const;
        void unlock() const;
        void wait() const;
        void notify();
        void notifyAll();

        bool timedWait(int millsecond) const;

    private:
        mutable int     _nnotify;
        mutable U       _cond;
        T               _mutex;

    };
    typedef TC_Monitor<TC_ThreadMutex, TC_ThreadCond> TC_ThreadLock;

    template<typename T, typename U>
    TC_Monitor<T, U>::TC_Monitor() : _nnotify(0)
    {
    }

    template<typename T, typename U>
    TC_Monitor<T, U>::~TC_Monitor() {

    }

    template<typename T, typename U>
    void TC_Monitor<T, U>::lock() const
    {
        _mutex.lock();
        _nnotify = 0;
    }

    template<typename T, typename U>
    void TC_Monitor<T, U>::notifyAll()
    {
        _nnotify = -1;
    }

    template<typename T, typename U>
    void TC_Monitor<T, U>::wait() const {

    }

    template<typename T, typename U>
    void TC_Monitor<T, U>::unlock() const
    {
        //notifyImpl(_nnotify);
        _mutex.unlock();
    }

    template<typename T, typename U>
    bool TC_Monitor<T, U>::timedWait(int millsecond) const
    {
        //notifyImpl(_nnotify);

        bool rc;

        try
        {
            rc = _cond.timedWait(_mutex, millsecond);
        }
        catch(...)
        {
            _nnotify = 0;
            throw;
        }

        _nnotify = 0;
        return rc;
    }


} //tars


#endif //RETARS_TC_MONITOR_H
