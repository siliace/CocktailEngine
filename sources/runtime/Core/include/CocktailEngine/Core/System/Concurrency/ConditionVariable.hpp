#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_CONDITIONVARIABLE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_CONDITIONVARIABLE_HPP

#include <CocktailEngine/Core/System/Concurrency/Mutex.hpp>
#include <CocktailEngine/Core/Utility/Time/Duration.hpp>
#include <CocktailEngine/Core/Utility/Time/Instant.hpp>

namespace Ck
{
    namespace Detail
    {
#ifdef COCKTAIL_OS_WINDOWS
        static constexpr unsigned int ConditionVariableHandleAlignas = 8;
        static constexpr unsigned int ConditionVariableHandleSize = 8;
#else
        static constexpr unsigned int ConditionVariableHandleAlignas = 8;
        static constexpr unsigned int ConditionVariableHandleSize = 48;
#endif
    }

    /**
     * \brief Coordinates threads waiting for a change to state protected by a Mutex
     *
     * A condition variable does not retain notifications or protect state itself. The
     * caller must modify and inspect the associated state while holding the same Mutex.
     * A waiting method atomically releases that Mutex while blocked and reacquires it
     * before returning. A wake-up without a predicate does not guarantee that the
     * awaited state is true; use the predicate overloads to handle spurious wake-ups.
     *
     * \note The supplied Mutex must be locked by the calling thread before every call
     * to Wait, WaitFor, or WaitUntil.
     */
    class COCKTAILENGINE_CORE_API ConditionVariable
    {
    public:

        /**
         * \brief Constructs an un-signaled condition variable
         */
        ConditionVariable();

        /**
         * \brief Destroys the condition variable
         *
         * No thread may be waiting on this condition variable when it is destroyed.
         */
        ~ConditionVariable();

        /**
         * \brief Waits until the condition variable is notified
         *
         * The mutex is released atomically while waiting and is locked again before
         * this method returns.
         *
         * \param mutex Locked mutex protecting the state associated with this wait
         */
        void Wait(Mutex& mutex);

        /**
         * \brief Waits until notified or until a relative timeout expires
         *
         * The mutex is released atomically while waiting and is locked again before
         * this method returns. A successful return can result from a spurious wake-up.
         *
         * \param mutex Locked mutex protecting the state associated with this wait
         * \param timeout Maximum time to wait. Duration::Infinite() waits indefinitely
         *
         * \return true if the wait ended before timing out, false if the timeout expired
         */
        bool WaitFor(Mutex& mutex, const Duration& timeout);

        /**
         * \brief Waits until a predicate is satisfied or a relative timeout expires
         *
         * The predicate is evaluated with \p mutex locked before waiting and after every
         * wake-up. This overload handles spurious wake-ups internally
         *
         * \tparam TProducer Callable that returns whether the awaited state is satisfied
         *
         * \param mutex Locked mutex protecting the state inspected by \p producer
         * \param timeout Maximum time to wait. Duration::Infinite() waits indefinitely
         * \param producer Predicate evaluated while \p mutex is locked
         *
         * \return true if \p producer returned true, false if the timeout expired first
         */
        template <Producer<bool> TProducer>
        bool WaitFor(Mutex& mutex, const Duration& timeout, TProducer producer)
        {
            if (timeout.IsInfinite())
            {
                while (!producer())
                    Wait(mutex);

                return true;
            }

            const Instant deadline = Instant::Now().After(timeout);
            return WaitUntil(mutex, deadline, producer);
        }

        /**
         * \brief Waits until notified or until an absolute instant is reached
         *
         * The mutex is released atomically while waiting and is locked again before
         * this method returns. A successful return can result from a spurious wake-up.
         *
         * \param mutex Locked mutex protecting the state associated with this wait
         * \param instant Absolute deadline for the wait
         *
         * \return true if the wait ended before the deadline, false if it was reached
         */
        bool WaitUntil(Mutex& mutex, const Instant& instant);

        /**
         * \brief Waits until a predicate is satisfied or until an absolute instant is reached
         *
         * The predicate is evaluated with \p mutex locked before waiting and after every
         * wake-up. This overload handles spurious wake-ups internally.
         *
         * \tparam TProducer Callable that returns whether the awaited state is satisfied
         *
         * \param mutex Locked mutex protecting the state inspected by \p producer
         * \param instant Absolute deadline for the wait
         * \param producer Predicate evaluated while \p mutex is locked
         *
         * \return true if \p producer returned true, false if the deadline was reached first
         */
        template <Producer<bool> TProducer>
        bool WaitUntil(Mutex& mutex, const Instant& instant, TProducer producer)
        {
            while (!producer())
            {
                if (!instant.IsAfter(Instant::Now()))
                    return false;

                if (!WaitUntil(mutex, instant))
                    return producer();
            }

            return true;
        }

        /**
         * \brief Wakes at least one thread waiting on this condition variable.
         *
         * This notification is not retained if no thread is waiting.
         */
        void NotifyOne();

        /**
         * \brief Wakes all threads waiting on this condition variable.
         *
         * This notification is not retained if no thread is waiting.
         */
        void NotifyAll();

    private:

        alignas(Detail::ConditionVariableHandleAlignas) Byte mHandle[Detail::ConditionVariableHandleSize];
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_CONDITIONVARIABLE_HPP
