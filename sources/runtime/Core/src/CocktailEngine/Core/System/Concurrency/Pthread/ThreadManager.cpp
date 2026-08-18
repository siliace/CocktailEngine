#include <sys/syscall.h>
#include <unistd.h>

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
    }

    ThreadManager::~ThreadManager()
    {
        pthread_key_delete(mCurrentThreadKey);
    }

    UniquePtr<Ck::Thread> ThreadManager::Create(Runnable* runnable, StringView name, unsigned int stackSize)
    {
        return MakeUnique<Thread>(runnable, name, stackSize);
    }

    Thread* ThreadManager::GetCurrentThread()
    {
        return static_cast<Thread*>(pthread_getspecific(mCurrentThreadKey));
    }

    pthread_key_t ThreadManager::GetCurrentThreadKey() const
    {
        return mCurrentThreadKey;
    }
}
