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
    }

    ThreadManager::~ThreadManager()
    {
        TlsFree(mTlsIndex);
    }

    UniquePtr<Ck::Thread> ThreadManager::Create(Runnable* runnable, StringView name, unsigned int stackSize)
    {
        return MakeUnique<Thread>(runnable, name, stackSize);
    }

    Thread* ThreadManager::GetCurrentThread()
    {
        return static_cast<Thread*>(TlsGetValue(mTlsIndex));
    }

    DWORD ThreadManager::GetCurrentThreadKey() const
    {
        return mTlsIndex;
    }
}
