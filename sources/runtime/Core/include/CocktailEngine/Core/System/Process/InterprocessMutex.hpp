#ifndef COCKTAILENGINE_CORE_SYSTEM_PROCESS_INTERPROCESSMUTEX_HPP
#define COCKTAILENGINE_CORE_SYSTEM_PROCESS_INTERPROCESSMUTEX_HPP

#include <CocktailEngine/Core/Export.hpp>
#include <CocktailEngine/Core/String.hpp>
#include <CocktailEngine/Core/Utility/StorageFor.hpp>

namespace Ck
{
    namespace Detail
    {
#ifdef COCKTAIL_OS_WINDOWS
        static constexpr unsigned int InterprocessMutexHandleAlignof = 8;
        static constexpr unsigned int InterprocessMutexHandleSizeof = 8;
#else
        static constexpr unsigned int InterprocessMutexHandleAlignof = 8;
        static constexpr unsigned int InterprocessMutexHandleSizeof = 32;
#endif
    }

    /**
     * \brief Represents a named mutex used to synchronize access to a shared resource between processes
     *
     * Multiple processes can synchronize access to a shared resource by opening a mutex
     * with the same name
     *
     * Mutex ownership is thread-specific and must be released by the thread that acquired it
     */
    class COCKTAILENGINE_CORE_API InterprocessMutex
    {
    public:

        /**
         * \brief Creates or opens a named interprocess mutex
         *
         * \param name Name used to identify the mutex across processes
         */
        explicit InterprocessMutex(const String& name);

        /**
         * \brief Closes the system handle associated with the mutex
         */
        ~InterprocessMutex();

        /**
         * \brief Acquires exclusive ownership of the mutex
         *
         * Blocks until the mutex is acquired
         *
         * If the previous owner terminated without releasing the mutex, ownership
         * is still acquired and the protected resource may require recovery
         */
        void Lock();

        /**
         * \brief Attempts to acquire exclusive ownership of the mutex without waiting
         *
         * \return True if the mutex was acquired, false if it is already owned by another thread
         */
        bool TryLock();

        /**
         * \brief Releases ownership of the mutex
         *
         * Must be called by the thread that currently owns the mutex
         */
        void Unlock();

        /**
         * \brief Returns the address of the storage reserved for the system handle
         *
         * \return Pointer to the internal system handle storage
         *
         * The returned pointer is intended for internal platform-specific implementations
         * and must not be used to access the handle outside the owning implementation
         */
        void* GetSystemHandle() const
        {
            return const_cast<InterprocessMutex*>(this)->mHandle.GetAddress();
        }

    private:

        StorageForSize<Detail::InterprocessMutexHandleAlignof, Detail::InterprocessMutexHandleSizeof> mHandle;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_PROCESS_INTERPROCESSMUTEX_HPP
