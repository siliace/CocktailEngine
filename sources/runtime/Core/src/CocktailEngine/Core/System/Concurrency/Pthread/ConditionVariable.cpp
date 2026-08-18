#include <cassert>
#include <cerrno>
#include <ctime>
#include <pthread.h>

#include <CocktailEngine/Core/System/Concurrency/ConditionVariable.hpp>
#include <CocktailEngine/Core/System/Concurrency/Pthread/PthreadErrorCategory.hpp>

namespace Ck
{
    static_assert(Detail::ConditionVariableHandleAlignas == alignof(pthread_cond_t));
    static_assert(Detail::ConditionVariableHandleSize == sizeof(pthread_cond_t));

    ConditionVariable::ConditionVariable() :
        mHandle{}
    {
        /// The default condition attributes use CLOCK_REALTIME, which is the clock
        /// Instant::Now() reads from. Both must stay in sync for WaitUntil to honor
        /// the deadlines it is given.
        const int result = pthread_cond_init(reinterpret_cast<pthread_cond_t*>(mHandle), nullptr);
        if (result != 0)
            throw std::system_error(result, Detail::Pthread::PthreadErrorCategory::Instance);
    }

    ConditionVariable::~ConditionVariable()
    {
        /// Only fails when the condition variable is invalid or still has waiters,
        /// both of which are caller errors that must not escape a destructor
        const int result = pthread_cond_destroy(reinterpret_cast<pthread_cond_t*>(mHandle));
        assert(result == 0);
        (void) result;
    }

    void ConditionVariable::Wait(Mutex& mutex)
    {
        const int result = pthread_cond_wait(reinterpret_cast<pthread_cond_t*>(mHandle), static_cast<pthread_mutex_t*>(mutex.GetSystemHandle()));

        if (result != 0)
            throw std::system_error(result, Detail::Pthread::PthreadErrorCategory::Instance);
    }

    bool ConditionVariable::WaitFor(Mutex& mutex, const Duration& timeout)
    {
        if (timeout.IsInfinite())
        {
            Wait(mutex);
            return true;
        }

        return WaitUntil(mutex, Instant::Now().After(timeout));
    }

    bool ConditionVariable::WaitUntil(Mutex& mutex, const Instant& instant)
    {
        const Instant now = Instant::Now();
        if (!instant.IsAfter(now))
            return false;

        /// Instant guarantees a normalized nanoseconds component, which
        /// pthread_cond_timedwait requires to be in the [0, 999999999] range
        timespec deadline;
        deadline.tv_sec = static_cast<time_t>(instant.GetSeconds());
        deadline.tv_nsec = static_cast<long>(instant.GetNanoseconds());

        /// pthread functions report failures through their return value and leave
        /// errno untouched, so SystemError::GetLastError() must not be used here
        const int result = pthread_cond_timedwait(reinterpret_cast<pthread_cond_t*>(mHandle), static_cast<pthread_mutex_t*>(mutex.GetSystemHandle()), &deadline);

        if (result != 0)
        {
            if (result != ETIMEDOUT)
                throw std::system_error(result, Detail::Pthread::PthreadErrorCategory::Instance);

            return false;
        }

        return true;
    }

    void ConditionVariable::NotifyOne()
    {
        /// Only fails on an invalid condition variable, which is a caller error
        const int result = pthread_cond_signal(reinterpret_cast<pthread_cond_t*>(mHandle));
        assert(result == 0);
        (void) result;
    }

    void ConditionVariable::NotifyAll()
    {
        /// Only fails on an invalid condition variable, which is a caller error
        const int result = pthread_cond_broadcast(reinterpret_cast<pthread_cond_t*>(mHandle));
        assert(result == 0);
        (void) result;
    }
}
