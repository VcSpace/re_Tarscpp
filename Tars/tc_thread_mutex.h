#ifndef RETARS_TC_THREAD_MUTEX_H
#define RETARS_TC_THREAD_MUTEX_H

#include <unistd.h>
#include <pthread.h>
#include <mutex>
#include <signal.h>
#include <cstring>
#include <cassert>

#include "tc_ex.h"

namespace tars
{
    class TC_ThreadCond;

    struct TC_ThreadCond_Exception : public TC_Exception
    {
        TC_ThreadCond_Exception(const std::string &buffer) : TC_Exception(buffer){};
        TC_ThreadCond_Exception(const std::string &buffer, int err) : TC_Exception(buffer, err){};
        ~TC_ThreadCond_Exception() throw() {};
    };
    class TC_ThreadMutex
    {
    public:
        TC_ThreadMutex();
        ~TC_ThreadMutex();

        void lock() const;
        void unlock() const;

        bool trylock() const;
        bool willUnlock() const
        {
            return true;
        }

    private:
    protected:
        // noncopyable
        TC_ThreadMutex(const TC_ThreadMutex&);
        void operator=(const TC_ThreadMutex&);

        int count() const;
        void count(int c) const;

        friend class TC_ThreadCond;

        mutable pthread_mutex_t _mutex;

    };

    class TC_ThreadCond
    {
    public:
        TC_ThreadCond();
        ~TC_ThreadCond();

        void signal();
        void broadcast();

        timespec abstime(int millsecond) const;

        template<typename Mutex>
        void wait(const Mutex &mutex) const
        {}

        template<typename Mutex>
        bool timedWait(const Mutex& mutex, int millsecond) const
        {
            int c = mutex.count();

            timespec ts = abstime(millsecond);

            int rc = pthread_cond_timedwait(&_cond, &mutex._mutex, &ts);

            mutex.count(c);

            if(rc != 0)
            {
                //cout<<"pthread_cond_timedwait rc is "<<rc<<endl;
                if(rc != ETIMEDOUT)
                {
                    throw TC_ThreadCond_Exception("[TC_ThreadCond::timedWait] pthread_cond_timedwait error", errno);
                }
                return false;
            }
            return true;
        }

    private:
        mutable pthread_cond_t _cond;
    };

} //tars

#endif //RETARS_TC_THREAD_MUTEX_H
