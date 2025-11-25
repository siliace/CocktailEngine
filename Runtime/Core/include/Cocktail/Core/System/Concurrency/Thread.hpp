#ifndef COCKTAIL_CORE_SYSTEM_CONCURRENCY_THREAD_HPP
#define COCKTAIL_CORE_SYSTEM_CONCURRENCY_THREAD_HPP

#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/System/Concurrency/ThreadManager.hpp>
#include <Cocktail/Core/Utility/Time/Duration.hpp>

namespace Ck
{
    /**
     * \class Thread
     * \brief Represents a system-level thread with portable control operations
     *
     * The \c Thread class provides a high-level abstraction for managing the
     * execution of a native thread, including priority control, CPU affinity
     * configuration, and simple synchronization primitives
     *
     * It acts as a facade on top of platform-specific implementations
     * (via \c ThreadManager), ensuring consistent behavior across
     * supported operating systems
     *
     * Example:
     * \code
     * class MyTask : public Runnable
     * {
     * public:
     *     void Run() override
     *     {
     *         // Code executed inside the thread
     *     }
     * };
     *
     * MyTask task;
     * auto thread = Thread::Create(&task, "WorkerThread");
     * thread->SetPriority(Thread::Priority::AboveNormal);
     * thread->Join(); // Wait until the thread finishes
     * \endcode
     */
    class COCKTAIL_CORE_API Thread : public ServiceFacade<ThreadManager>
    {
    public:

        /**
         * \enum Priority
         * \brief Describes available thread priority levels
         *
         * The exact mapping of these priority levels depends on the underlying platform
         * For example, \c TimeCritical may correspond to the maximum allowable
         * scheduling priority on some systems
         */
        enum class Priority
        {
            Normal,              /*!< Default priority level */
            AboveNormal,         /*!< Higher than normal */
            BelowNormal,         /*!< Lower than normal */
            Highest,             /*!< High priority (non-critical) */
            Lowest,              /*!< Lowest available priority */
            SlightlyBelowNormal, /*!< Slightly lower than normal  */
            TimeCritical,        /*!< Highest possible priority, time-critical */
        };

        /**
         * \brief Returns the number of logical CPU cores available on the system
         *
         * This includes both physical cores and logical threads (eg, via Hyper-Threading)
         *
         * \return The total number of logical CPU cores
         */
        static unsigned int GetCoreCount();

        /**
         * \brief Suspends the current thread for a given duration
         * 
         * \param duration The amount of time to sleep
         */
        static void SleepFor(const Duration& duration);

        /**
         * \brief Suspends the current thread until a specified time point
         * 
         * \param instant The time point at which the thread should wake up
         * \param duration Polling granularity
         * 
         * \note Useful for precise synchronization with time-based events
         */
        static void SleepUntil(const Instant& instant, const Duration& duration = Duration::Milliseconds(1));

        /**
         * \brief Waits until the given predicate returns \c true
         * 
         * \tparam Callable A callable type returning \c bool
         * 
         * \param callable Predicate to be evaluated in a loop
         * \param duration Polling granularity (default: 1 ms)
         *
         * \warning This function uses active polling, which may consume CPU cycles,
         *          consider more efficient synchronization primitives when appropriate
         */
        template <typename Callable>
        static void WaitUntil(Callable callable, const Duration& duration = Duration::Milliseconds(1))
        {
            while (!callable())
                SleepFor(duration);
        }

        /**
         * \brief Yields the execution of the current thread to allow other threads to run.
         *
         * This function voluntarily gives up the remainder of the CPU time slice allocated
         * to the calling thread, allowing the operating system scheduler to run another
         * thread that is ready to execute. If no other thread of equal or higher priority
         * is ready, the calling thread may continue execution without blocking.
         *
         * \note This function does not guarantee that the current thread will actually
         *       yield to another thread. It only provides a hint to the scheduler.
         */
        static void Yield();

        /**
         * \brief Creates and starts a new thread to run the given task
         *
         * \param runnable Pointer to the \c Runnable object to execute in the new thread
         * \param name A name for the thread (useful for debugging and profiling)
         * \param stackSize Stack size in bytes for the new thread. If \c 0, the system default is used
         * \return A unique pointer to the created \c Thread object
         *
         * \note The thread starts execution immediately after creation
         */
        static UniquePtr<Thread> Create(Runnable* runnable, StringView name = CK_TEXT(""), unsigned int stackSize = 0);

        /**
         * \brief Returns a pointer to the currently executing thread
         * 
         * \return Pointer to the current thread
         */
        static Thread* GetCurrentThread();

        /**
         * \brief Virtual default destructor
         * 
         * Waits for the thread to end before closing it
         */
        virtual ~Thread() = default;

        /**
         * \brief Waits for the thread to complete its execution
         * 
         * Blocks the calling thread until the target thread has finished running
         */
        virtual void Join() = 0;

        /**
         * \brief Waits for the thread to complete its execution or the specified time elapses
         * 
         * Blocks the calling thread until the target thread has finished running for a \p timeout duration
         * 
         * \param timeout The duration to wait the thread for completion
         *
         * \return True if the thread execution has been completed, false otherwise
         */
        virtual bool Join(const Duration& timeout) = 0;

        /**
         * \brief Terminates the thread.
         *
         * \param waitForTermination If \c true, waits until the thread has fully terminated
         *                           before returning.
         *
         * \warning Forcing a thread to terminate may cause resource leaks or leave the
         *          program in an inconsistent state. Use with caution.
         */
        virtual void Kill(bool waitForTermination) = 0;

        /**
         * \brief Tell whether the thread is the main thread of the process
         * 
         * \return True if the thread is the main one, false otherwise
         */
        virtual bool IsMain() const = 0;

        /**
         * \brief Sets the scheduling priority level of the thread
         *
         * \param priority Desired priority level
         *
         * \note The effect of this operation may vary depending on platform restrictions
         *       and user privileges
         */
        virtual void SetPriority(Priority priority) = 0;

        /**
         * \brief Sets the CPU affinity mask for this thread
         * 
         * \param affinityMask Bitmask of allowed CPU cores
         */
        virtual void SetAffinity(unsigned int affinityMask) = 0;

        /**
         * \brief Sets the name of the thread
         * The effect of this method may vary depending on the platform.
         * For example, on Windows the name may be visible in debuggers such as Visual Studio.
         *
         * \param name The new name to assign to the thread
         *
         * \note Thread names are typically used for debugging and profiling purposes
         */
        virtual void SetName(StringView name) = 0;

        /**
         * \brief Returns the name of the thread
         *
         * \return A constant reference to the thread's name string
         *
         * \note This name is purely descriptive and does not affect thread scheduling
         * or execution behavior.
         */
        virtual String GetName() const = 0;

        /**
         * \brief Returns the unique thread identifier
         * 
         * \return Thread ID
         */
        virtual unsigned int GetId() const = 0;

        /**
         * \brief Returns the native system handle for this thread
         * 
         * \return Platform-specific thread handle
         */
        virtual void* GetSystemHandle() const = 0;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_CONCURRENCY_THREAD_HPP
