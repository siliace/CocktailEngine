#ifndef COCKTAIL_CORE_SYSTEM_CONCURRENCY_WIN32_THREAD_HPP
#define COCKTAIL_CORE_SYSTEM_CONCURRENCY_WIN32_THREAD_HPP

#include <string_view>

#include <Cocktail/Core/System/Concurrency/Thread.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
    /**
     * \class Thread
     * \brief Windows-specific implementation of the \c Ck::Thread interface
     *
     * This class provides a concrete implementation of the \c Ck::Thread abstraction
     * using native Win32 threading APIs It allows creating, managing, suspending,
     * resuming, and controlling the behavior of threads at the system level
     *
     * \note This class is not intended to be used directly by high-level code
     * Use the platform-independent \c Ck::Thread API instead
     */
    class Thread : public Ck::Thread
    {
    public:

        /**
         * \brief Native thread entry point
         *
         * This static function is passed to \c CreateThread and acts as the
         * starting point of execution for the newly created thread
         *
         * \param arg Pointer to the thread object (\c Thread*)
         * \return The thread's exit code
         */
        static DWORD WINAPI Run(LPVOID arg);

        /**
         * \brief Constructs a \c Thread object for an existing thread
         *
         * This constructor is typically used to wrap the main thread of the process
         *
         * \param handle The thread handle
         * \param id The unique thread identifier
         * \param isMain Indicates whether this thread is the main process thread
         */
        Thread(HANDLE handle, unsigned int id, bool isMain);

        /**
         * \brief Creates a new thread that executes the given runnable task
         *
         * \param runnable Pointer to the runnable object to execute
         * \param name Name of the thread (for debugging and profiling)
         * \param stackSize Stack size in bytes If \c 0, the system default is used
         */
        Thread(Runnable* runnable, StringView name, unsigned int stackSize);

        /**
         * \brief Destructor
         *
         * Ensures that system resources associated with the thread
         * (such as the thread handle) are properly released
         */
        ~Thread() override;

        /**
         * \brief Waits for the thread to complete execution
         *
         * Blocks the calling thread until the target thread terminates
         * Internally uses \c WaitForSingleObject
         */
        void Join() override;

        /**
         * \brief Waits for the thread to complete execution
         *
         * Blocks the calling thread until the target thread terminates
         * Internally uses \c WaitForSingleObject
         *
         * \param timeout The duration to wait the thread for completion
         *
         * \return True if the thread execution has been completed, false otherwise
         */
        bool Join(const Duration& timeout) override;

        /**
         * \brief Terminates the thread
         *
         * \param waitForTermination If \c true, waits until the thread
         * is fully terminated before returning
         *
         * \warning Forcefully killing a thread may cause resource leaks or
         * leave shared data in an inconsistent state
         */
        void Kill(bool waitForTermination) override;

        /**
         * \brief Checks if this thread represents the process's main thread
         *
         * \return \c true if this is the main thread, otherwise \c false
         */
        bool IsMain() const override;

        /**
         * \brief Sets the scheduling priority of the thread
         *
         * Maps \c Ck::Thread::Priority to the corresponding Win32 priority levels
         */
        void SetPriority(Priority priority) override;

        /**
         * \brief Sets the CPU affinity mask for the thread
         *
         * Uses the Win32 API \c SetThreadAffinityMask to bind the thread to specific CPU cores
         *
         * \param affinityMask Bitmask representing the allowed cores
         */
        void SetAffinity(unsigned affinityMask) override;

        void SetName(StringView name) override;
        String GetName() const override;

        /**
         * \brief Returns the unique thread identifier
         *
         * \return The thread ID as an unsigned integer
         */
        unsigned GetId() const override;

        /**
         * \brief Returns the native thread handle
         *
         * \return A Win32 \c HANDLE representing the thread
         */
        void* GetSystemHandle() const override;

    private:

        DWORD mId;           /*!< Unique thread identifier */
        bool mMain;        /*!< Indicates if this is the main thread */
        HANDLE mHandle;      /*!< Win32 handle to the thread */
        Runnable* mRunnable; /*!< Runnable task associated with the thread */
    };
}

#endif // COCKTAIL_CORE_SYSTEM_CONCURRENCY_WIN32_THREAD_HPP
