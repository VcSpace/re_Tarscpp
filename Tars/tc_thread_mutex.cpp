#include "tc_thread_mutex.h"

#include <iostream>

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
            std::cout << "[TC_ThreadMutex::~TC_ThreadMutex] pthread_mutex_destroy error:" << string(strerror(rc)) << std::endl;
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

    TC_ThreadCond::TC_ThreadCond() {

    }

    TC_ThreadCond::~TC_ThreadCond() {

    }
} //tars
