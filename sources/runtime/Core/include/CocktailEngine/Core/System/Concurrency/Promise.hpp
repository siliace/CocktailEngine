#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_PROMISE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_PROMISE_HPP

#include <CocktailEngine/Core/Exception.hpp>
#include <CocktailEngine/Core/Memory/SharedPtr.hpp>
#include <CocktailEngine/Core/System/Concurrency/ConditionVariable.hpp>
#include <CocktailEngine/Core/System/Concurrency/LockGuard.hpp>
#include <CocktailEngine/Core/Utility/Optional.hpp>

namespace Ck
{
    /**
     * \brief Thrown when a producer attempts to resolve a non-pending promise.
     *
     * A promise may be resolved exactly once. A promise that was fulfilled or
     * broken by destruction is no longer pending.
     */
    COCKTAIL_DECLARE_EXCEPTION_FROM(AlreadySatisfiedException, RuntimeException);

    /**
     * \brief Thrown when a future's producer was destroyed before supplying a value.
     */
    COCKTAIL_DECLARE_EXCEPTION_FROM(BrokenPromiseException, RuntimeException);

    template <typename T>
    class Future;

    namespace Detail
    {
        enum class PromiseStatus
        {
            Pending,
            Fulfilled,
            Broken
        };

        template <typename T>
        class PromiseState
        {
        public:

            template <typename TValue>
            void SetValue(TValue&& value)
            {
                {
                    LockGuard lg(mMutex);
                    if (mStatus != PromiseStatus::Pending)
                        throw AlreadySatisfiedException();

                    mValue.Emplace(Forward<TValue>(value));
                    mStatus = PromiseStatus::Fulfilled;
                }

                mNotifier.NotifyAll();
            }

            void Break()
            {
                bool notify = false;
                {
                    LockGuard lg(mMutex);
                    if (mStatus == PromiseStatus::Pending)
                    {
                        mStatus = PromiseStatus::Broken;
                        notify = true;
                    }
                }

                if (notify)
                    mNotifier.NotifyAll();
            }

            bool IsSatisfied()
            {
                LockGuard lg(mMutex);
                return mStatus == PromiseStatus::Fulfilled;
            }

            T GetValue()
            {
                LockGuard lg(mMutex);
                mNotifier.Wait(mMutex, [&]() {
                    return mStatus != PromiseStatus::Pending;
                });

                if (mStatus == PromiseStatus::Broken)
                    throw BrokenPromiseException();

                return mValue.Get();
            }

        private:

            Mutex mMutex;
            ConditionVariable mNotifier;
            PromiseStatus mStatus = PromiseStatus::Pending;
            Optional<T> mValue;
        };
    }

    /**
     * \class Promise
     *
     * \brief Single producer for a value observed through one or more Future instances
     *
     * A promise owns the producer role of a shared completion state. It can create
     * any number of futures with ToFuture(); each future observes the same terminal
     * result. A promise is move-only so that exactly one object owns that producer role.
     *
     * A pending promise becomes broken when its owner is destroyed or overwritten by
     * move assignment. Futures waiting on that state wake up and throw
     * BrokenPromiseException from Future::GetValue().
     *
     * \tparam T Type of value supplied to associated futures
     */
    template <typename T>
    class Promise
    {
    public:

        /**
         * \brief Creates a pending promise with no result.
         */
        Promise() :
            mState(MakeShared<Detail::PromiseState<T>>())
        {
            /// Nothing
        }

        Promise(const Promise&) = delete;
        Promise& operator=(const Promise&) = delete;

        /**
         * \brief Transfers the producer role to this promise
         *
         * The moved-from promise must only be destroyed or assigned a new value.
         *
         * \param other Promise whose shared state is transferred
         */
        Promise(Promise&& other) noexcept :
            mState(Move(other.mState))
        {
            /// Nothing
        }

        /**
         * \brief Breaks this promise's pending state and takes ownership of another producer role
         *
         * Any futures associated with this promise before the assignment observe a
         * broken promise if no value had been supplied.
         *
         * \param other Promise whose shared state is transferred
         *
         * \return This promise
         */
        Promise& operator=(Promise&& other) noexcept
        {
            if (this == &other)
                return *this;

            Break();
            mState = Move(other.mState);

            return *this;
        }

        /**
         * \brief Breaks the associated state if no value has been supplied
         *
         * Futures that are waiting for a result are notified and throw
         * BrokenPromiseException when retrieving it.
         */
        ~Promise()
        {
            Break();
        }

        /**
         * \brief Copies a value into the shared completion state
         *
         * \param value Value to publish to every associated future
         *
         * If copying the value throws, the promise remains pending and may be
         * resolved later.
         *
         * \throw AlreadySatisfiedException If the promise is already fulfilled or broken
         */
        void SetValue(const T& value)
        {
            mState->SetValue(value);
        }

        /**
         * \brief Moves a value into the shared completion state
         *
         * \param value Value to publish to every associated future
         *
         * If moving the value throws, the promise remains pending and may be
         * resolved later.
         *
         * \throw AlreadySatisfiedException If the promise is already fulfilled or broken
         */
        void SetValue(T&& value)
        {
            mState->SetValue(Move(value));
        }

        /**
         * \brief Creates a future that observes this promise's shared state
         *
         * This method may be called multiple times. Each returned future observes
         * the same value or broken-promise outcome independently.
         *
         * \return A future that becomes fulfilled when this promise receives a value
         */
        Future<T> ToFuture() const;

    private:

        void Break()
        {
            if (mState)
                mState->Break();
        }

        template <typename>
        friend class Future;

        SharedPtr<Detail::PromiseState<T>> mState;
    };
}

// Include after Promise is complete so Promise.hpp alone exposes ToFuture().
#include <CocktailEngine/Core/System/Concurrency/Future.hpp>

#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_PROMISE_HPP
