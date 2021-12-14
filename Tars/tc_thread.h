#ifndef RETARS_TC_THREAD_H
#define RETARS_TC_THREAD_H

#include <unistd.h>
#include <pthread.h>
#include <mutex>

#include "tc_monitor.h"

namespace tars
{
    class TC_ThreadControl
    {
    public:
        TC_ThreadControl();
        explicit TC_ThreadControl(pthread_t);

    private:

        pthread_t _thread;
    };

    class TC_Runable
    {
    public:
        virtual ~TC_Runable(){};
        virtual void run() = 0;
    };

    class TC_Thread : public TC_Runable
    {
    public:
        TC_Thread();
        virtual ~TC_Thread();


    private:

    protected:
        virtual void run() = 0;
    };


}

#endif //RETARS_TC_THREAD_H
