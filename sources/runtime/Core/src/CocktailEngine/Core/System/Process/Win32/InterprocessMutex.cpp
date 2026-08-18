#include <CocktailEngine/Core/System/Process/InterprocessMutex.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>

namespace Ck
{
    namespace
    {
        HANDLE& GetWin32Handle(InterprocessMutex& mutex)
        {
            return *static_cast<HANDLE*>(mutex.GetSystemHandle());
        }
    }

    static_assert(Detail::InterprocessMutexHandleAlignof == alignof(HANDLE));
    static_assert(Detail::InterprocessMutexHandleSizeof == sizeof(HANDLE));

    InterprocessMutex::InterprocessMutex(const String& name) :
        mHandle{}
    {
        HANDLE& handle = GetWin32Handle(*this);
        handle = CreateMutex(nullptr, FALSE, name.GetData());
        if (handle == INVALID_HANDLE_VALUE)
            throw SystemError::GetLastError();
    }

    InterprocessMutex::~InterprocessMutex()
    {
        HANDLE& handle = GetWin32Handle(*this);
        if (handle != INVALID_HANDLE_VALUE)
            CloseHandle(handle);
    }

    void InterprocessMutex::Lock()
    {
        HANDLE& handle = GetWin32Handle(*this);
        DWORD result = WaitForSingleObject(handle, INFINITE);
        if (result != WAIT_OBJECT_0 && result != WAIT_ABANDONED)
            throw SystemError::GetLastError();
    }

    bool InterprocessMutex::TryLock()
    {
        HANDLE& handle = GetWin32Handle(*this);
        DWORD result = WaitForSingleObject(handle, 0);

        if (result == WAIT_TIMEOUT)
            return false;

        if (result != WAIT_OBJECT_0 && result != WAIT_ABANDONED)
            throw SystemError::GetLastError();

        return true;
    }

    void InterprocessMutex::Unlock()
    {
        ReleaseMutex(GetWin32Handle(*this));
    }
}
