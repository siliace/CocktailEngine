#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILELOCK_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILELOCK_HPP

namespace Ck
{
    /**
     * \brief Provides a synchronization mechanism for file access
     *
     * FileLock defines an interface to control concurrent access to a file
     * or shared resource. It allows acquiring and releasing an exclusive lock,
     * either in a blocking or non-blocking manner.
     *
     * Implementations may rely on operating system primitives or custom
     * synchronization mechanisms.
     */
    class FileLock
    {
    public:

        /**
         * \brief Destructor
         */
        virtual ~FileLock() = default;

        /**
         * \brief Acquires the lock (blocking)
         *
         * Blocks the calling thread until the lock is successfully acquired.
         */
        virtual void Lock() = 0;

        /**
         * \brief Attempts to acquire the lock (non-blocking)
         *
         * Tries to acquire the lock without blocking.
         *
         * \return True if the lock was successfully acquired, false otherwise
         */
        virtual bool TryLock() = 0;

        /**
         * \brief Releases the lock
         *
         * Unlocks the resource so that other threads or processes can acquire it.
         */
        virtual void Unlock() = 0;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILELOCK_HPP
