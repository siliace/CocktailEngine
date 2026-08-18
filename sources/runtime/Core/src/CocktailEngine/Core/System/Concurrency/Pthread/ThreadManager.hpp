#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_PTHREAD_THREADMANAGER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_PTHREAD_THREADMANAGER_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/System/Concurrency/ThreadManager.hpp>
#include <CocktailEngine/Core/System/Concurrency/Pthread/Thread.hpp>

namespace Ck::Detail::Pthread
{
    class ThreadManager : public Ck::ThreadManager
    {
    public:

        ThreadManager();
        ~ThreadManager() override;
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
#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_PTHREAD_THREADMANAGER_HPP
