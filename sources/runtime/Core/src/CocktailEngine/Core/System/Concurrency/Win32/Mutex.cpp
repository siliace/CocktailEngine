#include <CocktailEngine/Core/System/Concurrency/Mutex.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>

namespace Ck
{
    static_assert(Detail::MutexHandleAlignas == alignof(CRITICAL_SECTION));
    static_assert(Detail::MutexHandleSize == sizeof(CRITICAL_SECTION));

    Mutex::Mutex() :
        mHandle{}
    {
        InitializeCriticalSectionAndSpinCount(static_cast<CRITICAL_SECTION*>(mHandle.GetAddress()), 4000);
    }

    Mutex::~Mutex()
    {
        DeleteCriticalSection(static_cast<CRITICAL_SECTION*>(mHandle.GetAddress()));
    }

    void Mutex::Lock()
    {
        EnterCriticalSection(static_cast<CRITICAL_SECTION*>(mHandle.GetAddress()));
    }

    bool Mutex::TryLock()
    {
        return TryEnterCriticalSection(static_cast<CRITICAL_SECTION*>(mHandle.GetAddress())) != FALSE;
    }

    void Mutex::Unlock()
    {
        LeaveCriticalSection(static_cast<CRITICAL_SECTION*>(mHandle.GetAddress()));
    }
}
