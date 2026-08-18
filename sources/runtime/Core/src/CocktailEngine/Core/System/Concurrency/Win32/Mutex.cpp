#include <CocktailEngine/Core/System/Concurrency/Mutex.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>

namespace Ck
{
    static_assert(Detail::MutexHandleAlignas == alignof(CRITICAL_SECTION));
    static_assert(Detail::MutexHandleSize == sizeof(CRITICAL_SECTION));

    Mutex::Mutex() :
        mHandle{}
    {
        InitializeCriticalSectionAndSpinCount(reinterpret_cast<CRITICAL_SECTION*>(mHandle), 4000);
    }

    Mutex::~Mutex()
    {
        DeleteCriticalSection(reinterpret_cast<CRITICAL_SECTION*>(mHandle));
    }

    void Mutex::Lock()
    {
        EnterCriticalSection(reinterpret_cast<CRITICAL_SECTION*>(mHandle));
    }

    bool Mutex::TryLock()
    {
        return TryEnterCriticalSection(reinterpret_cast<CRITICAL_SECTION*>(mHandle)) != FALSE;
    }

    void Mutex::Unlock()
    {
        LeaveCriticalSection(reinterpret_cast<CRITICAL_SECTION*>(mHandle));
    }
}
