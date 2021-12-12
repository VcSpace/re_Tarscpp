#ifndef RETARS_TC_THREAD_LOCK_H
#define RETARS_TC_THREAD_LOCK_H

namespace tars
{
    template <typename T>
    class TC_LockT
    {
    public:

    private:
    };

    template <typename T>
    class TC_TryLockT : public TC_LockT<T>
    {
    public:

    private:
    };

    class TC_ThreadCond;
    class TC_ThreadMutex
    {
    public:
        TC_ThreadMutex();
        ~TC_ThreadMutex();

    private:

    protected:
        virtual void run() = 0;
    };

    class TC_ThreadCond
    {
    public:
        TC_ThreadCond();
        ~TC_ThreadCond();

    private:
    };

} //tars


#endif //RETARS_TC_THREAD_LOCK_H
