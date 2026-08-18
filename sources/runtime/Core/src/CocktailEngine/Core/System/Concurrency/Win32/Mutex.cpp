#include <CocktailEngine/Core/System/Concurrency/Mutex.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>

namespace Ck
{
    static_assert(Detail::MutexHandleAlignas == alignof(CRITICAL_SECTION));
    static_assert(Detail::MutexHandleSize == sizeof(CRITICAL_SECTION));

    Mutex::Mutex() :
        mHandle{}
    {
        InitializeCriticalSectionAndSpinCount((CRITICAL_SECTION*)mHandle, 4000);
    }

    Mutex::~Mutex()
    {
        DeleteCriticalSection((CRITICAL_SECTION*)mHandle);
    }

    void Mutex::Lock()
    {
        EnterCriticalSection((CRITICAL_SECTION*)mHandle);
    }

    bool Mutex::TryLock()
    {
        return TryEnterCriticalSection((CRITICAL_SECTION*)mHandle) != FALSE;
    }

    void Mutex::Unlock()
    {
        LeaveCriticalSection((CRITICAL_SECTION*)mHandle);
    }
}
