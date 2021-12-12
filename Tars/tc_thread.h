#ifndef RETARS_TC_THREAD_H
#define RETARS_TC_THREAD_H

#include <unistd.h>
#include <pthread.h>
#include <mutex>

namespace tars
{
    class TC_Thread
    {
    public:
        TC_Thread();
        virtual ~TC_Thread();

    private:
    };

    class TC_ThreadMutex
    {
    public:
        TC_ThreadMutex();
        ~TC_ThreadMutex();

    private:
    };

    class TC_ThreadCond
    {
    public:
        TC_ThreadCond();
        ~TC_ThreadCond();

    private:
    };
}

#endif //RETARS_TC_THREAD_H
