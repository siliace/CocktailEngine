#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_MUTEX_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_MUTEX_HPP

#include <CocktailEngine/Core/Export.hpp>
#include <CocktailEngine/Core/Utility/StorageFor.hpp>

namespace Ck
{
    namespace Detail
    {
#ifdef COCKTAIL_OS_WINDOWS
        static constexpr unsigned int MutexHandleAlignof = 8;
        static constexpr unsigned int MutexHandleSizeof = 40;
#else
        static constexpr unsigned int MutexHandleAlignof = 8;
        static constexpr unsigned int MutexHandleSizeof = 40;
#endif
    }

    /**
     * \brief Grants one thread at a time access to the state it protects
     *
     * A mutex is the lock the rest of this module is built on: ConditionVariable waits
     * on one, and LockGuard is how it is normally taken, acquisition and release being
     * tied to a scope rather than left to every return path.
     *
     * It is a recursive lock on every platform, a CRITICAL_SECTION on Windows and a
     * pthread mutex of recursive type elsewhere. The thread already owning it may
     * therefore lock it again, each lock needing its own unlock, which is what lets a
     * guarded method call another one that guards the same lock.
     *
     * \note The lock is in-process only, and cheap to take when uncontended: on
     * Windows it spins briefly before blocking. It is not a named, cross-process
     * object, whatever its platform counterpart may be used for elsewhere.
     *
     * \warning Recursion and waiting do not mix. See the warning on ConditionVariable:
     * a waiting method releases a single level of ownership, so a mutex locked more
     * than once by the waiting thread stays owned while it blocks.
     *
     * \see LockGuard
     * \see ConditionVariable
     */
    class COCKTAILENGINE_CORE_API Mutex
    {
    public:

        /**
         * \brief Constructs an unlocked mutex
         */
        Mutex();

        /**
         * \brief Destroys the mutex
         *
         * \warning The mutex must be unlocked, and no thread may be blocked trying to
         * acquire it.
         */
        ~Mutex();

        /**
         * \brief Mutexes are not copyable: a copy would be a second lock, guarding
         *        nothing that the holders of the first know about
         */
        Mutex(Mutex const&) = delete;

        /**
         * \brief Mutexes are not copy assignable, for the reason they are not copyable
         *
         * \return Nothing, the operator being deleted
         */
        Mutex& operator=(const Mutex&) = delete;

        /**
         * \brief Mutexes are not movable: owners and waiters hold the address of the
         *        one they were given, which moving would leave behind
         */
        Mutex(Mutex&&) = delete;

        /**
         * \brief Mutexes are not move assignable, for the reason they are not movable
         *
         * \return Nothing, the operator being deleted
         */
        Mutex& operator=(Mutex&&) = delete;

        /**
         * \brief Acquires the mutex, blocking until it is free
         *
         * Returns immediately when the calling thread already owns it, the lock being
         * recursive. Every call must be matched by one call to Unlock.
         */
        void Lock();

        /**
         * \brief Acquires the mutex if it is free, without blocking
         *
         * Succeeds when the calling thread already owns it, the lock being recursive,
         * and a successful attempt must be matched by one call to Unlock like Lock is.
         *
         * \return True if the mutex was acquired, false if another thread owns it
         */
        bool TryLock();

        /**
         * \brief Releases one level of ownership of the mutex
         *
         * The mutex becomes free once released as many times as it was acquired.
         *
         * \warning Only the owning thread may release it, and only as many times as it
         * acquired it.
         */
        void Unlock();

        /**
         * \brief Returns the native object backing this mutex
         *
         * \return A pointer to the platform object, a CRITICAL_SECTION on Windows and
         *         a pthread_mutex_t elsewhere.
         *
         * \warning This exists for the platform back-ends of this module and for code
         * that has to hand the mutex to a native API. What it points to is not part of
         * this class's contract.
         */
        void* GetSystemHandle() const
        {
            return const_cast<Mutex*>(this)->mHandle.GetAddress();
        }

    private:

        StorageForSize<Detail::MutexHandleAlignof, Detail::MutexHandleSizeof> mHandle;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_MUTEX_HPP
