#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_BLOCKINGQUEUE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_BLOCKINGQUEUE_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/System/Concurrency/ConditionVariable.hpp>
#include <CocktailEngine/Core/System/Concurrency/LockGuard.hpp>
#include <CocktailEngine/Core/Utility/Optional.hpp>

namespace Ck
{
    /**
     * \brief
     * \tparam T
     */
    template <typename T>
    class BlockingQueue
    {
    public:

        /**
         * \brief
         * \return
         */
        T Take()
        {
            LockGuard<Mutex> lock(mLock);
            mNotifier.Wait(mLock, [&] {
                return !mQueue.IsEmpty();
            });

            return mQueue.PopFirst();
        }

        /**
         * \brief
         * \param timeout
         * \return
         */
        Optional<T> Take(const Duration& timeout)
        {
            LockGuard<Mutex> lock(mLock);
            bool completed = mNotifier.WaitFor(mLock, timeout, [&] {
                return !mQueue.IsEmpty();
            });

            if (!completed)
                return Optional<T>::Empty();

            T value = mQueue.PopFirst();
            return Optional<T>::Of(Move(value));
        }

        /**
         * \brief
         * \return
         */
        Optional<T> TryTake()
        {
            LockGuard<Mutex> lock(mLock);
            if (mQueue.IsEmpty())
                return Optional<T>::Empty();

            T value = mQueue.PopFirst();
            return Optional<T>::Of(Move(value));
        }

        /**
         * \brief
         *
         * \tparam TPredicate
         *
         * \param predicate
         *
         * \return
         */
        template <typename TPredicate>
        Optional<T> TryTake(TPredicate predicate)
        {
            LockGuard<Mutex> lock(mLock);
            mNotifier.Wait(mLock, [&] {
                return !mQueue.IsEmpty() || predicate();
            });

            if (mQueue.IsEmpty())
                return Optional<T>::Empty();

            T value = mQueue.PopFirst();
            return Optional<T>::Of(Move(value));
        }

        /**
         * \brief
         * \return
         */
        bool IsEmpty() const
        {
            LockGuard<Mutex> lock(mLock);
            return mQueue.IsEmpty();
        }

        /**
         * \brief
         * \param value
         */
        void Push(const T& value)
        {
            {
                LockGuard<Mutex> lock(mLock);
                mQueue.push(value);
            }
            mNotifier.NotifyOne();
        }

        /**
         * \brief
         * \param value
         */
        void Push(T&& value)
        {
            {
                LockGuard<Mutex> lock(mLock);
                mQueue.push(Forward<T>(value));
            }
            mNotifier.NotifyOne();
        }

        /**
         * \brief
         * \tparam Args
         * \param args
         */
        template <typename... Args>
        void Emplace(Args&&... args)
        {
            {
                LockGuard<Mutex> lock(mLock);
                mQueue.Emplace(Forward<Args>(args)...);
            }
            mNotifier.NotifyOne();
        }

        /**
         * \brief Manually trigger notification of all pending wait operation
         */
        void NotifyAll()
        {
            mNotifier.NotifyAll();
        }

    private:

        mutable Mutex mLock;
        ConditionVariable mNotifier;
        Array<T> mQueue;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_BLOCKINGQUEUE_HPP
