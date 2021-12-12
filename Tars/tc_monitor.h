#ifndef RETARS_TC_MONITOR_H
#define RETARS_TC_MONITOR_H

#include "tc_thread.h"

namespace tars
{
    template <typename T, typename U>
    class TC_Monitor
    {
    public:
        TC_Monitor();
        virtual ~TC_Monitor();

    private:

    };

    typedef TC_Monitor<TC_ThreadMutex, TC_ThreadCond> TC_ThreadLock;

} //tars


#endif //RETARS_TC_MONITOR_H
