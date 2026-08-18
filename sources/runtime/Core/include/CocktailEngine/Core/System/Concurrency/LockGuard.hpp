#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_LOCKGUARD_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_LOCKGUARD_HPP

#include <CocktailEngine/Core/StaticArray.hpp>
#include <CocktailEngine/Core/System/Concurrency/Lockable.hpp>

namespace Ck
{
    template <typename... TLockables>
    class LockGuard;

    /**
     * \brief Acquires one lock for the lifetime of the guard.
     *
     * The guard does not own the lock. The supplied lock must outlive the guard and must not
     * be manually unlocked while the guard exists. The lock is acquired during construction
     * and released when the guard is destroyed.
     *
     * \tparam TLockable Type of lock to guard.
     */
    template <Lockable TLockable>
    class LockGuard<TLockable>
    {
    public:

        /**
         * \brief Acquires the supplied lock.
         *
         * \param mutex Lock to hold until this guard is destroyed.
         */
        explicit LockGuard(TLockable& mutex) :
            mMutex(mutex)
        {
            mMutex.Lock();
        }

        /**
         * \brief Releases the guarded lock.
         *
         * \warning TLockable::Unlock must not throw. An exception during destruction causes
         * program termination.
         */
        ~LockGuard()
        {
            mMutex.Unlock();
        }

        /**
         * \brief Guards cannot be copied because they uniquely release their locks.
         */
        LockGuard(const LockGuard&) = delete;

        /**
         * \brief Guards cannot be copied because they uniquely release their locks.
         */
        LockGuard& operator=(const LockGuard&) = delete;

        /**
         * \brief Guards cannot be moved because they retain references to their locks.
         */
        LockGuard(LockGuard&&) = delete;

        /**
         * \brief Guards cannot be moved because they retain references to their locks.
         */
        LockGuard& operator=(LockGuard&&) = delete;

    private:

        TLockable& mMutex;
    };

    /**
     * \brief Acquires several locks in a global order for the lifetime of the guard.
     *
     * Every lock must have the same type. Their addresses are sorted before acquisition, so
     * guards constructed with the same distinct locks acquire them in the same order and
     * release them in reverse order. This prevents ABBA deadlocks only when all code that
     * takes these locks together follows the same protocol.
     *
     * Each argument must refer to a distinct lock. Repeating a non-recursive lock attempts
     * to acquire it twice and can deadlock.
     *
     * \tparam TLockable Type shared by every guarded lock.
     * \tparam TOtherLockables Types of the remaining guarded locks. Each must be
     * \ref std::same_as TLockable.
     */
    template <Lockable TLockable, Lockable... TOtherLockables>
        requires(sizeof...(TOtherLockables) > 0 && (std::same_as<TLockable, TOtherLockables> && ...))
    class LockGuard<TLockable, TOtherLockables...>
    {
    public:

        /**
         * \brief Acquires the supplied locks in ascending address order.
         *
         * If acquiring a lock throws, the guard attempts to release every lock acquired
         * beforehand and then rethrows the exception.
         *
         * \param first First lock to guard.
         * \param others Additional locks to guard. Every lock must be distinct from
         * \p first and from every other argument.
         */
        explicit LockGuard(TLockable& first, TOtherLockables&... others) :
            mMutexes{ std::addressof(first), std::addressof(others)... }
        {
            std::sort(mMutexes.GetIterator(), mMutexes.GetEndIterator(), std::less<TLockable*>{});

            try
            {
                for (; mLockedCount < mMutexes.GetSize(); ++mLockedCount)
                    mMutexes[mLockedCount]->Lock();
            }
            catch (...)
            {
                UnlockAll();
                throw;
            }
        }

        /**
         * \brief Releases every guarded lock in reverse acquisition order.
         *
         * \warning TLockable::Unlock must not throw. An exception during destruction causes
         * program termination.
         */
        ~LockGuard()
        {
            UnlockAll();
        }

        /**
         * \brief Guards cannot be copied because they uniquely release their locks.
         */
        LockGuard(const LockGuard&) = delete;

        /**
         * \brief Guards cannot be copied because they uniquely release their locks.
         */
        LockGuard& operator=(const LockGuard&) = delete;

        /**
         * \brief Guards cannot be moved because they retain references to their locks.
         */
        LockGuard(LockGuard&&) = delete;

        /**
         * \brief Guards cannot be moved because they retain references to their locks.
         */
        LockGuard& operator=(LockGuard&&) = delete;

    private:

        /**
         * \brief Releases every lock acquired by this guard in reverse order.
         */
        void UnlockAll()
        {
            while (mLockedCount != 0)
                mMutexes[--mLockedCount]->Unlock();
        }

        StaticArray<TLockable*, sizeof...(TOtherLockables) + 1> mMutexes;
        std::size_t mLockedCount = 0;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_LOCKGUARD_HPP
