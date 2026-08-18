#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_WIN32_THREADMANAGER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_WIN32_THREADMANAGER_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/System/Concurrency/Mutex.hpp>
#include <CocktailEngine/Core/System/Concurrency/ThreadManager.hpp>
#include <CocktailEngine/Core/System/Concurrency/Win32/Thread.hpp>

namespace Ck::Detail::Win32
{
    class ThreadManager : public Ck::ThreadManager
    {
    public:

        ThreadManager();
        ~ThreadManager() override;
        UniquePtr<Ck::Thread> Create(Runnable* runnable, StringView name, unsigned int stackSize) override;
        Thread* GetCurrentThread() override;
        DWORD GetCurrentThreadKey() const;

    private:

        UniquePtr<Thread> mMainThread;
        DWORD mTlsIndex;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_WIN32_THREADMANAGER_HPP
