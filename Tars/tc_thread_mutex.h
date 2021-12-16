#ifndef RETARS_TC_THREAD_MUTEX_H
#define RETARS_TC_THREAD_MUTEX_H

#include <unistd.h>
#include <pthread.h>
#include <mutex>
#include <signal.h>
#include <cstring>

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

    private:

    protected:
    };

    class TC_ThreadCond
    {
    public:
        TC_ThreadCond();
        ~TC_ThreadCond();

        void signal();
        void broadcast();

        template<typename Mutex>
        void wait(const Mutex &mutex) const;




    private:
    };

} //tars

#endif //RETARS_TC_THREAD_MUTEX_H
