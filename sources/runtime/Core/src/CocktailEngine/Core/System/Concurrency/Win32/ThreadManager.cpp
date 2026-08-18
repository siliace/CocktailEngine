#include <CocktailEngine/Core/System/Concurrency/LockGuard.hpp>
#include <CocktailEngine/Core/System/Concurrency/Win32/ThreadManager.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>

namespace Ck::Detail::Win32
{
    ThreadManager::ThreadManager()
    {
        mTlsIndex = TlsAlloc();
        if (mTlsIndex == 0)
        {
            DWORD error = ::GetLastError();
            if (error != NO_ERROR)
                throw std::system_error(static_cast<int>(error), SystemError::GetSystemErrorCategory());
        }

        mMainThread = MakeUnique<Thread>(::GetCurrentThread(), ::GetCurrentThreadId(), true);
        mMainThread->SetName(CK_TEXT("Main"));

        TlsSetValue(mTlsIndex, mMainThread.Get());

        LockGuard<Mutex> lg(mThreadMutex);
        mThreads.Add(mMainThread.Get());
    }

    ThreadManager::~ThreadManager()
    {
        TlsFree(mTlsIndex);
    }

    void ThreadManager::Register(Thread* thread)
    {
        assert(thread->GetId() == ::GetCurrentThreadId());

        TlsSetValue(mTlsIndex, thread);

        LockGuard<Mutex> lg(mThreadMutex);
        mThreads.Add(thread);
    }

    void ThreadManager::Unregister(Thread* thread)
    {
        assert(thread->GetId() == ::GetCurrentThreadId());

        TlsSetValue(mTlsIndex, nullptr);

        LockGuard<Mutex> lg(mThreadMutex);
        mThreads.Remove(thread);
    }

    Thread* ThreadManager::GetCurrentThread()
    {
        return static_cast<Thread*>(TlsGetValue(mTlsIndex));
    }
}
