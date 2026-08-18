#include <pthread.h>

#include <CocktailEngine/Core/System/Concurrency/Mutex.hpp>

namespace Ck
{
    static_assert(Detail::MutexHandleAlignas == alignof(pthread_mutex_t));
    static_assert(Detail::MutexHandleSize == sizeof(pthread_mutex_t));

    Mutex::Mutex() :
        mHandle{}
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

        pthread_mutex_init(static_cast<pthread_mutex_t*>(mHandle.GetAddress()), &attr);

        pthread_mutexattr_destroy(&attr);
    }

    Mutex::~Mutex()
    {
        pthread_mutex_destroy(static_cast<pthread_mutex_t*>(mHandle.GetAddress()));
    }

    void Mutex::Lock()
    {
        pthread_mutex_lock(static_cast<pthread_mutex_t*>(mHandle.GetAddress()));
    }

    bool Mutex::TryLock()
    {
        return pthread_mutex_trylock(static_cast<pthread_mutex_t*>(mHandle.GetAddress())) == 0;
    }

    void Mutex::Unlock()
    {
        pthread_mutex_unlock(static_cast<pthread_mutex_t*>(mHandle.GetAddress()));
    }
}
