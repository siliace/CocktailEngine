#include <pthread.h>

#include <CocktailEngine/Core/System/Concurrency/ConditionVariable.hpp>

namespace Ck
{
    static_assert(Detail::ConditionVariableHandleAlignas == alignof(pthread_cond_t));
    static_assert(Detail::ConditionVariableHandleSize == sizeof(pthread_cond_t));

    ConditionVariable::ConditionVariable()
    {
        pthread_cond_init(reinterpret_cast<pthread_cond_t*>(mHandle), nullptr);
    }

    ConditionVariable::~ConditionVariable()
    {
        pthread_cond_destroy(reinterpret_cast<pthread_cond_t*>(mHandle));
    }

    void ConditionVariable::Wait(Mutex& mutex)
    {
        pthread_cond_wait(reinterpret_cast<pthread_cond_t*>(mHandle), static_cast<pthread_mutex_t*>(mutex.GetSystemHandle()));
    }

    bool ConditionVariable::WaitFor(Mutex& mutex, const Duration& timeout)
    {
        return false;
    }

    bool ConditionVariable::WaitUntil(Mutex& mutex, const Instant& instant)
    {
        return false;
    }

    void ConditionVariable::NotifyOne()
    {
        pthread_cond_signal(reinterpret_cast<pthread_cond_t*>(mHandle));
    }

    void ConditionVariable::NotifyAll()
    {
        pthread_cond_broadcast(reinterpret_cast<pthread_cond_t*>(mHandle));
    }
}
