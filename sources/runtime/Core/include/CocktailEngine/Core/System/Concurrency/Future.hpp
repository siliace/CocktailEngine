#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_FUTURE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_FUTURE_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/System/Concurrency/Promise.hpp>

namespace Ck
{
    /**
     * \class Future
     *
     * \brief Consumer handle for a value produced by a Promise
     *
     * Futures are copyable handles to a shared completion state. Copies and futures
     * returned by repeated Promise::ToFuture() calls all observe the same result.
     * The state stays alive while at least one promise or future owns it.
     *
     * GetValue() blocks until the state is fulfilled, failed, or broken. It returns
     * an owned copy rather than a reference to shared storage, so the result remains
     * valid after the future is destroyed. Consequently, retrieving a value requires
     * T to be copy constructible. A failed state rethrows the exception that the
     * promise published.
     *
     * \tparam T Type of value provided by the associated promise
     */
    template <typename T>
    class Future
    {
    public:

        /**
         * \brief Waits for each future in a range and collects their values
         *
         * Futures are retrieved in range order. The method blocks until each future
         * is fulfilled; an empty range returns an empty array immediately. If a
         * future has a broken promise or a published exception, retrieval stops and
         * propagates that exception. Futures after the failing future are not read.
         *
         * \tparam TIterator Input iterator whose elements provide GetValue() for T
         * \tparam TAllocator Allocator used by the returned array
         *
         * \param begin Iterator to the first future to retrieve
         * \param end Iterator past the last future to retrieve
         *
         * \return An array containing the values in the same order as the futures
         *
         * \throw BrokenPromiseException If a promise is broken before supplying a value
         * \throw Any exception published with Promise::SetException()
         */
        template <typename TIterator, typename TAllocator = HeapAllocator>
        static Array<T, TAllocator> WaitAll(TIterator begin, TIterator end)
        {
            Array<T, TAllocator> values;

            for (auto it = begin; it != end; ++it)
                values.Emplace((*it).GetValue());

            return values;
        }

        /**
         * \brief Tests whether the associated promise has supplied a value
         *
         * \return `true` when the shared state is fulfilled; `false` while pending,
         *         after a published exception, or after the producer has broken the promise
         */
        bool IsSatisfied() const
        {
            return mState->IsSatisfied();
        }

        /**
         * \brief Waits for and returns a copy of the produced value
         *
         * This method blocks while the promise is pending. Multiple futures may
         * retrieve the same fulfilled value independently.
         *
         * \return A copy of the value supplied by Promise::SetValue()
         *
         * \throw BrokenPromiseException If the promise is destroyed before supplying a value
         * \throw Any exception published with Promise::SetException()
         */
        T GetValue() const
        {
            return mState->GetValue();
        }

    private:

        template <typename>
        friend class Promise;

        explicit Future(SharedPtr<Detail::PromiseState<T>> state) :
            mState(Move(state))
        {
            /// Nothing
        }

        SharedPtr<Detail::PromiseState<T>> mState;
    };

    template <typename T>
    Future<T> Promise<T>::ToFuture() const
    {
        return Future<T>(mState);
    }
}

#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_FUTURE_HPP
