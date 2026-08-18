#include <cassert>
#include <cerrno>
#include <ctime>
#include <pthread.h>

#include <CocktailEngine/Core/System/Concurrency/Event.hpp>
#include <CocktailEngine/Core/System/Concurrency/Pthread/PthreadErrorCategory.hpp>
#include <CocktailEngine/Core/Utility/Time/Instant.hpp>

namespace Ck
{
    namespace Detail
    {
        /**
         * \brief What an event is made of on this platform
         *
         * POSIX has no event object, so one is assembled from the flag the event
         * carries and the mutex and condition variable guarding it. The flag is what
         * makes a signal outlive the absence of a waiter, which a bare condition
         * variable does not do.
         */
        struct EventHandle
        {
            bool Signaled;
            pthread_mutex_t Mutex;
            pthread_cond_t Condition;
        };
    }

    /// The handle is reached through opaque storage of a size the header has to state
    /// without seeing the struct, so the two have to be checked against each other.
    static_assert(Detail::EventHandleAlignof == alignof(Detail::EventHandle));
    static_assert(Detail::EventHandleSizeof == sizeof(Detail::EventHandle));

    namespace
    {
        Detail::EventHandle& GetEventHandle(const Event& event)
        {
            return *static_cast<Detail::EventHandle*>(event.GetSystemHandle());
        }

        /**
         * \brief Holds the mutex of an event handle for the lifetime of the guard
         *
         * The handle owns a raw pthread_mutex_t rather than a Ck::Mutex, which
         * LockGuard would need, and every waiting method here has a path that throws.
         * Releasing the mutex from a destructor is what keeps those paths from leaving
         * the event locked.
         */
        class EventLock
        {
        public:

            explicit EventLock(Detail::EventHandle& handle) :
                mHandle(handle)
            {
                const int result = pthread_mutex_lock(&mHandle.Mutex);
                assert(result == 0);
                (void) result;
            }

            ~EventLock()
            {
                const int result = pthread_mutex_unlock(&mHandle.Mutex);
                assert(result == 0);
                (void) result;
            }

            EventLock(const EventLock&) = delete;
            EventLock(EventLock&&) = delete;
            EventLock& operator=(const EventLock&) = delete;
            EventLock& operator=(EventLock&&) = delete;

        private:

            Detail::EventHandle& mHandle;
        };
    }

    Event::Event(ResetMode resetMode, bool initiallySignaled) :
        mResetMode(resetMode),
        mHandle{}
    {
        Detail::EventHandle& handle = GetEventHandle(*this);
        handle.Signaled = initiallySignaled;

        int result = pthread_mutex_init(&handle.Mutex, nullptr);
        if (result != 0)
            throw std::system_error(result, Detail::Pthread::PthreadErrorCategory::Instance);

        /// The default condition attributes use CLOCK_REALTIME, which is the clock
        /// Instant::Now() reads from. Both must stay in sync for WaitUntil to honor
        /// the deadlines it is given.
        result = pthread_cond_init(&handle.Condition, nullptr);
        if (result != 0)
        {
            pthread_mutex_destroy(&handle.Mutex);
            throw std::system_error(result, Detail::Pthread::PthreadErrorCategory::Instance);
        }
    }

    Event::~Event()
    {
        Detail::EventHandle& handle = GetEventHandle(*this);

        /// Both only fail when the object is invalid or still has waiters, which are
        /// caller errors that must not escape a destructor
        int result = pthread_cond_destroy(&handle.Condition);
        assert(result == 0);

        result = pthread_mutex_destroy(&handle.Mutex);
        assert(result == 0);
        (void) result;
    }

    void Event::Set()
    {
        Detail::EventHandle& handle = GetEventHandle(*this);
        EventLock lock(handle);

        handle.Signaled = true;

        /// An Auto event hands the signal to one thread, so waking a second one would
        /// only have it find the flag already consumed and wait again.
        const int result = mResetMode == ResetMode::Manual ? pthread_cond_broadcast(&handle.Condition) : pthread_cond_signal(&handle.Condition);

        assert(result == 0);
        (void) result;
    }

    void Event::Reset()
    {
        Detail::EventHandle& handle = GetEventHandle(*this);
        EventLock lock(handle);

        handle.Signaled = false;
    }

    void Event::Wait()
    {
        Detail::EventHandle& handle = GetEventHandle(*this);
        EventLock lock(handle);

        while (!handle.Signaled)
        {
            const int result = pthread_cond_wait(&handle.Condition, &handle.Mutex);
            if (result != 0)
                throw std::system_error(result, Detail::Pthread::PthreadErrorCategory::Instance);
        }

        if (mResetMode == ResetMode::Auto)
            handle.Signaled = false;
    }

    bool Event::WaitUntil(const Instant& instant)
    {
        /// Instant guarantees a normalized nanoseconds component, which
        /// pthread_cond_timedwait requires to be in the [0, 999999999] range
        timespec deadline;
        deadline.tv_sec = static_cast<time_t>(instant.GetSeconds());
        deadline.tv_nsec = static_cast<long>(instant.GetNanoseconds());

        Detail::EventHandle& handle = GetEventHandle(*this);
        EventLock lock(handle);

        /// The flag is tested before the deadline, so an event that is already signaled
        /// reports it whatever the deadline, and a deadline in the past polls instead
        /// of failing outright.
        while (!handle.Signaled)
        {
            /// pthread functions report failures through their return value and leave
            /// errno untouched, so SystemError::GetLastError() must not be used here
            const int result = pthread_cond_timedwait(&handle.Condition, &handle.Mutex, &deadline);
            if (result == 0)
                continue;

            if (result != ETIMEDOUT)
                throw std::system_error(result, Detail::Pthread::PthreadErrorCategory::Instance);

            /// A signal can land together with the timeout, the mutex being reacquired
            /// before the call returns either way.
            if (!handle.Signaled)
                return false;

            break;
        }

        if (mResetMode == ResetMode::Auto)
            handle.Signaled = false;

        return true;
    }

    bool Event::WaitFor(const Duration& timeout)
    {
        if (timeout.IsInfinite())
        {
            Wait();
            return true;
        }

        return WaitUntil(Instant::Now().After(timeout));
    }

    Event::ResetMode Event::GetResetMode() const
    {
        return mResetMode;
    }
}
