#ifndef COCKTAIL_CORE_SYSTEM_CONCURRENCY_PTHREAD_THREADMANAGER_HPP
#define COCKTAIL_CORE_SYSTEM_CONCURRENCY_PTHREAD_THREADMANAGER_HPP

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/System/Concurrency/ThreadManager.hpp>
#include <Cocktail/Core/System/Concurrency/Pthread/Thread.hpp>

namespace Ck::Detail::Pthread
{
    class ThreadManager : public Ck::ThreadManager
    {
    public:

        ThreadManager();
        ~ThreadManager();
        void Register(Thread* thread);
        void Unregister(Thread* thread);
        Thread* GetCurrentThread() override;

    private:

        UniquePtr<Thread> mMainThread;
        Array<Thread*> mThreads;
        std::mutex mThreadMutex;
        pthread_key_t mCurrentThreadKey;
    };
}
#endif // COCKTAIL_CORE_SYSTEM_CONCURRENCY_PTHREAD_THREADMANAGER_HPP
