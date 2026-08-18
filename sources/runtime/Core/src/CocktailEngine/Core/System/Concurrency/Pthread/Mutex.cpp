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

        pthread_mutex_init(reinterpret_cast<pthread_mutex_t*>(mHandle), &attr);

        pthread_mutexattr_destroy(&attr);
    }

    Mutex::~Mutex()
    {
        pthread_mutex_destroy(reinterpret_cast<pthread_mutex_t*>(mHandle));
    }

    void Mutex::Lock()
    {
        pthread_mutex_lock(reinterpret_cast<pthread_mutex_t*>(mHandle));
    }

    bool Mutex::TryLock()
    {
        return pthread_mutex_trylock(reinterpret_cast<pthread_mutex_t*>(mHandle)) == 0;
    }

    void Mutex::Unlock()
    {
        pthread_mutex_unlock(reinterpret_cast<pthread_mutex_t*>(mHandle));
    }
}
