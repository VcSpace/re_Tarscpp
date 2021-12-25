#ifndef RETARS_TC_THREAD_LOCK_H
#define RETARS_TC_THREAD_LOCK_H

namespace tars
{
    template <typename T>
    class TC_LockT
    {
    public:
        TC_LockT(const T &mutex);
        virtual ~TC_LockT();

    private:
        const T &_mutex;

        mutable bool _acquired;
    };

    template<typename T>
    TC_LockT<T>::TC_LockT(const T &mutex) : _mutex(mutex)
    {
        //std::cout << "1class name = " << typeid(mutex).name() << std::endl;
        //std::cout << "2class name = " << typeid(*this).name() << std::endl;
        _mutex.lock();
        _acquired = true;
    }

    template<typename T>
    TC_LockT<T>::~TC_LockT()
    {
        if (_acquired)
        {
            _mutex.unlock();
        }
    }

    //--------
    template <typename T>
    class TC_TryLockT : public TC_LockT<T>
    {
    public:
        TC_TryLockT();
        virtual ~TC_TryLockT();

    private:
    };


} //tars


#endif //RETARS_TC_THREAD_LOCK_H
