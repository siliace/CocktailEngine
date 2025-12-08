#include <pthread.h>

#include <Cocktail/Core/System/Concurrency/ThreadLocalStorage.hpp>

namespace Ck
{
    ThreadLocalStorage::ThreadLocalStorage()
    {
        pthread_key_create(&mHandle, nullptr);
    }

    ThreadLocalStorage::~ThreadLocalStorage()
    {
        pthread_key_delete(mHandle);
    }

    void* ThreadLocalStorage::Get() const
    {
        return pthread_getspecific(mHandle);
    }

    void ThreadLocalStorage::Set(void* pointer)
    {
        pthread_setspecific(mHandle, pointer);
    }
}
