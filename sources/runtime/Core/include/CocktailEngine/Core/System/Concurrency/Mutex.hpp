#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_MUTEX_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_MUTEX_HPP

#include <CocktailEngine/Core/Export.hpp>

namespace Ck
{
    namespace Detail
    {
#ifdef COCKTAIL_OS_WINDOWS
        static constexpr unsigned int MutexHandleAlignas = 8;
        static constexpr unsigned int MutexHandleSize = 40;
#else
        static constexpr unsigned int MutexHandleAlignas = 8;
        static constexpr unsigned int MutexHandleSize = 40;
#endif
    }

    class COCKTAILENGINE_CORE_API Mutex
    {
    public:

        explicit Mutex();
        ~Mutex();

        /**
         * \brief
         */
        Mutex(Mutex const&) = delete;

        /**
         * \brief
         * \return
         */
        Mutex& operator=(const Mutex&) = delete;

        /**
         * \brief
         */
        Mutex(Mutex&&) = delete;

        /**
         * \brief
         * \return
         */
        Mutex& operator=(Mutex&&) = delete;

        void Lock();
        bool TryLock();
        void Unlock();

        void* GetSystemHandle() const
        {
            return const_cast<Mutex*>(this)->mHandle;
        }

    private:

        alignas(Detail::MutexHandleAlignas) Byte mHandle[Detail::MutexHandleSize];
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_MUTEX_HPP
