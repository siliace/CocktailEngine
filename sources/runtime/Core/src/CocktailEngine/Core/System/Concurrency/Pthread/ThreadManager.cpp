#include <sys/syscall.h>
#include <unistd.h>

#include <CocktailEngine/Core/System/Concurrency/LockGuard.hpp>
#include <CocktailEngine/Core/System/Concurrency/Pthread/PthreadErrorCategory.hpp>
#include <CocktailEngine/Core/System/Concurrency/Pthread/ThreadManager.hpp>

namespace Ck::Detail::Pthread
{
    ThreadManager::ThreadManager() :
        mCurrentThreadKey()
    {
        if (int error = pthread_key_create(&mCurrentThreadKey, nullptr); error != 0)
            throw std::system_error(error, PthreadErrorCategory::Instance);

        mMainThread = MakeUnique<Thread>(pthread_self(), syscall(SYS_gettid), true);
        mMainThread->SetName(CK_TEXT("Main"));

        pthread_setspecific(mCurrentThreadKey, mMainThread.Get());

        LockGuard<Mutex> lg(mThreadMutex);
        mThreads.Add(mMainThread.Get());
    }

    ThreadManager::~ThreadManager()
    {
        pthread_key_delete(mCurrentThreadKey);
    }

    void ThreadManager::Register(Thread* thread)
    {
        assert(thread->GetId() == syscall(SYS_gettid));

        pthread_setspecific(mCurrentThreadKey, thread);

        LockGuard<Mutex> lg(mThreadMutex);
        mThreads.Add(thread);
    }

    void ThreadManager::Unregister(Thread* thread)
    {
        assert(thread->GetId() == syscall(SYS_gettid));

        pthread_setspecific(mCurrentThreadKey, nullptr);

        LockGuard<Mutex> lg(mThreadMutex);
        mThreads.Remove(thread);
    }

    Thread* ThreadManager::GetCurrentThread()
    {
        return static_cast<Thread*>(pthread_getspecific(mCurrentThreadKey));
    }
}
