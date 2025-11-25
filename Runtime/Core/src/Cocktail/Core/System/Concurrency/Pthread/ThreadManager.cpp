#include <unistd.h>
#include <sys/syscall.h>

#include <Cocktail/Core/System/Concurrency/Pthread/PthreadErrorCategory.hpp>
#include <Cocktail/Core/System/Concurrency/Pthread/ThreadManager.hpp>

namespace Ck::Detail::Pthread
{
    ThreadManager::ThreadManager()
    {
        if (int error = pthread_key_create(&mCurrentThreadKey, nullptr); error != 0)
            throw std::system_error(error, PthreadErrorCategory::Instance);

        mMainThread = MakeUnique<Thread>(pthread_self(), syscall(SYS_gettid), true);
        pthread_setspecific(mCurrentThreadKey, mMainThread.Get());

        std::lock_guard<std::mutex> lg(mThreadMutex);
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

        std::lock_guard<std::mutex> lg(mThreadMutex);
        mThreads.Add(thread);
    }

    void ThreadManager::Unregister(Thread* thread)
    {
        mThreads.Remove(thread);
        pthread_setspecific(mCurrentThreadKey, nullptr);
    }

    Thread* ThreadManager::GetCurrentThread()
    {
        return static_cast<Thread*>(pthread_getspecific(mCurrentThreadKey));
    }
}
