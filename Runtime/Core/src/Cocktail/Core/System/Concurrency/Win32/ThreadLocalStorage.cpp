#include <Cocktail/Core/System/Concurrency/ThreadLocalStorage.hpp>
#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck
{
    const unsigned int ThreadLocalStorage::InvalidHandle = TLS_OUT_OF_INDEXES;

    ThreadLocalStorage::ThreadLocalStorage()
    {
        mHandle = TlsAlloc();
        if (mHandle == InvalidHandle)
            throw SystemError::GetLastError();
    }

    ThreadLocalStorage::~ThreadLocalStorage()
    {
        TlsFree(mHandle);
    }

    ThreadLocalStorage::ThreadLocalStorage(ThreadLocalStorage&& other) noexcept :
        mHandle(std::exchange(other.mHandle, InvalidHandle))
    {
        /// Nothing
    }

    ThreadLocalStorage& ThreadLocalStorage::operator=(ThreadLocalStorage&& other) noexcept
    {
        if (this == &other)
            return *this;

        mHandle = std::exchange(other.mHandle, InvalidHandle);

        return *this;
    }

    void* ThreadLocalStorage::Get() const
    {
        return TlsGetValue(mHandle);
    }

    void ThreadLocalStorage::Set(void* pointer)
    {
        TlsSetValue(mHandle, pointer);
    }

    ThreadLocalStorage::Handle ThreadLocalStorage::GetSystemHandle() const
    {
        return mHandle;
    }
}
