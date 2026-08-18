#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_PTHREAD_THREADMANAGER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_PTHREAD_THREADMANAGER_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/System/Concurrency/Pthread/Thread.hpp>
#include <CocktailEngine/Core/System/Concurrency/ThreadManager.hpp>

namespace Ck::Detail::Pthread
{
    class ThreadManager : public Ck::ThreadManager
    {
    public:

        ThreadManager();
        ~ThreadManager() override;
        UniquePtr<Ck::Thread> Create(Runnable* runnable, StringView name, unsigned int stackSize) override;
        Thread* GetCurrentThread() override;
        pthread_key_t GetCurrentThreadKey() const;

    private:

        UniquePtr<Thread> mMainThread;
        pthread_key_t mCurrentThreadKey;
    };
}
#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_PTHREAD_THREADMANAGER_HPP
