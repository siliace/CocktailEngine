#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_THREADMANAGER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_THREADMANAGER_HPP

#include <CocktailEngine/Core/String.hpp>

namespace Ck
{
    class Runnable;
    class Thread;

    /**
     * \brief Interface for creating and managing threads
     *
     * Provides methods to spawn threads, access the current thread,
     * and subscribe to thread lifecycle events.
     */
    class ThreadManager
    {
    public:

        /**
         * \brief Default destructor
         */
        virtual ~ThreadManager() = default;

        /**
         * \brief Creates and starts a new thread to run the given task
         *
         * \param runnable Pointer to the \c Runnable object to execute in the new thread
         * \param name A name for the thread (useful for debugging and profiling)
         * \param stackSize Stack size in bytes for the new thread. If \c 0, the system default is used
         *
         * \return A unique pointer to the created \c Thread object
         *
         * \note The thread starts execution immediately after creation
         */
        virtual UniquePtr<Thread> Create(Runnable* runnable, StringView name, unsigned int stackSize) = 0;

        /**
         * \brief Returns a pointer to the currently executing thread
         *
         * \return Pointer to the current thread
         */
        virtual Thread* GetCurrentThread() = 0;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_THREADMANAGER_HPP
