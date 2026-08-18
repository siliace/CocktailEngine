#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_LOCKGUARD_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_LOCKGUARD_HPP

#include <CocktailEngine/Core/System/Concurrency/Lockable.hpp>

namespace Ck
{
    template <Lockable TLockable>
    class LockGuard
    {
    public:
        explicit LockGuard(TLockable& mutex)
            : mMutex(mutex)
        {
            mMutex.Lock();
        }

        ~LockGuard()
        {
            mMutex.Unlock();
        }

        /**
         * \brief
         */
        LockGuard(LockGuard const&) = delete;

        /**
         * \brief
         * \return
         */
        LockGuard& operator=(const LockGuard&) = delete;

        /**
         * \brief
         */
        LockGuard(LockGuard&&) = delete;

        /**
         * \brief
         * \return
         */
        LockGuard& operator=(LockGuard&&) = delete;

    private:
        TLockable& mMutex;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_LOCKGUARD_HPP
