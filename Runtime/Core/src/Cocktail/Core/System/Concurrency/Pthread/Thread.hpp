#ifndef COCKTAIL_CORE_SYSTEM_CONCURRENCY_PTHREAD_THREAD_HPP
#define COCKTAIL_CORE_SYSTEM_CONCURRENCY_PTHREAD_THREAD_HPP

#include <pthread.h>

#include <Cocktail/Core/System/Concurrency/Thread.hpp>

namespace Ck::Detail::Pthread
{
    class Thread : public Ck::Thread
    {
    public:

        /**
         * \brief Native thread entry point
         *
         * This static function is passed to \c pthread_create and acts as the
         * starting point of execution for the newly created thread
         *
         * \param arg Pointer to the thread object (\c Thread*)
         * \return
         */
        static void* Run(void* arg);

        /**
         * \brief Constructs a \c Thread object for an existing thread
         *
         * This constructor is typically used to wrap the main thread of the process
         *
         * \param handle The thread handle
         * \param id The unique thread identifier
         * \param isMain Indicates whether this thread is the main process thread
         */
        Thread(pthread_t handle, int id, bool isMain);

        /**
         * \brief Creates a new thread that executes the given runnable task
         *
         * \param runnable Pointer to the runnable object to execute
         * \param name Name of the thread (for debugging and profiling)
         * \param stackSize Stack size in bytes If \c 0, the system default is used
         */
        Thread(Runnable* runnable, StringView name, unsigned int stackSize);

        ~Thread() override;

        void Join() override;
        bool Join(const Duration& timeout) override;
        void Kill(bool waitForTermination) override;
        bool IsMain() const override;
        void SetPriority(Priority priority) override;
        void SetAffinity(unsigned affinityMask) override;
        void SetName(StringView name) override;
        String GetName() const override;
        unsigned GetId() const override;
        void* GetSystemHandle() const override;

    private:

        pthread_t mHandle;          /*!< pthread handle to the thread */
        int mId;                    /*!< Unique thread identifier */
        bool mMain;                 /*!< Indicates if this is the main thread */
        Runnable* mRunnable;        /*!< Runnable task associated with the thread */
        bool mJoined;               /*!< Indicated if this thread has been joined */
    };
}

#endif // COCKTAIL_CORE_SYSTEM_CONCURRENCY_PTHREAD_THREAD_HPP
