#include "tc_thread_mutex.h"

#include <cstring>
#include <cassert>
#include <iostream>
#include <stdint.h>
#include <sys/time.h>

namespace tars
{

    TC_ThreadMutex::TC_ThreadMutex()
    {
        int rc;
        pthread_mutexattr_t attr;
        rc = pthread_mutexattr_init(&attr);
        assert(rc == 0);

        rc = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
        assert(rc == 0);

        rc = pthread_mutex_init(&_mutex, &attr);
        assert(rc == 0);

        rc = pthread_mutexattr_destroy(&attr);
        assert(rc == 0);

        if(rc != 0)
        {
            std::cout << "[TC_ThreadMutex::TC_ThreadMutex] pthread_mutexattr_init error" << std::endl;
        }
    }

    TC_ThreadMutex::~TC_ThreadMutex()
    {
        int rc = 0;
        rc = pthread_mutex_destroy(&_mutex);
        if(rc != 0)
        {
            std::cout << "[TC_ThreadMutex::~TC_ThreadMutex] pthread_mutex_destroy error:" << std::string(strerror(rc)) << std::endl;
        }
    }

    void TC_ThreadMutex::lock() const
    {
        int rc = pthread_mutex_lock(&_mutex);
        if(rc != 0)
        {
            if(rc == EDEADLK)
            {
                std::cout << "[TC_ThreadMutex::lock] pthread_mutex_lock dead lock error" << rc << std::endl;
            }
            else
            {
                std::cout << "[TC_ThreadMutex::lock] pthread_mutex_lock error" << rc << std::endl;
            }
        }

    }

    void TC_ThreadMutex::unlock() const {

    }

    int TC_ThreadMutex::count() const
    {
        return 0;
    }

    void TC_ThreadMutex::count(int c) const
    {
    }

    TC_ThreadCond::TC_ThreadCond()
    {
        int rc;

        pthread_condattr_t attr;
        rc = pthread_condattr_init(&attr);
        if(rc != 0)
        {
            throw TC_ThreadCond_Exception("[TC_ThreadCond::TC_ThreadCond] pthread_condattr_init error", errno);
        }

        rc = pthread_cond_init(&_cond, &attr);
        if(rc != 0)
        {
            throw TC_ThreadCond_Exception("[TC_ThreadCond::TC_ThreadCond] pthread_cond_init error", errno);
        }

        rc = pthread_condattr_destroy(&attr);
        if(rc != 0)
        {
            throw TC_ThreadCond_Exception("[TC_ThreadCond::TC_ThreadCond] pthread_condattr_destroy error", errno);
        }
    }

    TC_ThreadCond::~TC_ThreadCond()
    {
        int rc = 0;
        rc = pthread_cond_destroy(&_cond);
        if(rc != 0)
        {
            std::cout << "[TC_ThreadCond::~TC_ThreadCond] pthread_cond_destroy error:" << std::string(strerror(rc)) << std::endl;
        }
    }

    timespec TC_ThreadCond::abstime(int millsecond) const
    {
        struct timeval tv;

        gettimeofday(&tv, 0);
        //TC_TimeProvider::getInstance()->getNow(&tv);

        int64_t it  = tv.tv_sec * (int64_t)1000000 + tv.tv_usec + (int64_t)millsecond * 1000;

        tv.tv_sec   = it / (int64_t)1000000;
        tv.tv_usec  = it % (int64_t)1000000;

        timespec ts;
        ts.tv_sec   = tv.tv_sec;
        ts.tv_nsec  = tv.tv_usec * 1000;

        return ts;
    }
} //tars
