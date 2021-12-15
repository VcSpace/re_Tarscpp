#ifndef RETARS_TC_THREAD_MUTEX_H
#define RETARS_TC_THREAD_MUTEX_H

#include <unistd.h>
#include <pthread.h>
#include <mutex>
#include <signal.h>

namespace tars
{
    class TC_ThreadCond;
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

    private:
    };

} //tars

#endif //RETARS_TC_THREAD_MUTEX_H
