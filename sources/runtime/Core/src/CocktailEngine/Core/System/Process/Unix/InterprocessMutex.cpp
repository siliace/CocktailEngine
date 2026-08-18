#include <cerrno>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <CocktailEngine/Core/System/Concurrency/Pthread/PthreadErrorCategory.hpp>
#include <CocktailEngine/Core/System/Process/InterprocessMutex.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>

namespace Ck
{
    namespace
    {
        struct SharedMutex
        {
            pthread_mutex_t Mutex;
        };

        struct InterprocessMutexHandle
        {
            AsciiString Name;
            SharedMutex* Shared = nullptr;
        };

        InterprocessMutexHandle& GetUnixHandle(InterprocessMutex& mutex)
        {
            return *static_cast<InterprocessMutexHandle*>(mutex.GetSystemHandle());
        }
    }

    static_assert(Detail::InterprocessMutexHandleAlignof == alignof(InterprocessMutexHandle));
    static_assert(Detail::InterprocessMutexHandleSizeof == sizeof(InterprocessMutexHandle));

    InterprocessMutex::InterprocessMutex(const String& name)
    {
        InterprocessMutexHandle& handle = GetUnixHandle(*this);
        handle.Name = AsciiString::Convert(name);

        int sharedMemory = shm_open(handle.Name.GetData(), O_CREAT | O_EXCL | O_RDWR, 0600);

        bool created = sharedMemory >= 0;
        if (!created && errno == EEXIST)
            sharedMemory = shm_open(handle.Name.GetData(), O_RDWR, 0600);

        if (sharedMemory < 0)
            throw SystemError::GetLastError();

        if (ftruncate(sharedMemory, sizeof(SharedMutex)) != 0)
        {
            int error = errno;

            shm_unlink(handle.Name.GetData());
            close(sharedMemory);

            throw std::system_error(error, SystemError::GetSystemErrorCategory());
        }

        void* mapping = mmap(nullptr, sizeof(SharedMutex), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemory, 0);

        close(sharedMemory);

        if (mapping == MAP_FAILED)
        {
            int error = errno;
            shm_unlink(handle.Name.GetData());

            throw std::system_error(error, SystemError::GetSystemErrorCategory());
        }

        handle.Shared = static_cast<SharedMutex*>(mapping);

        pthread_mutexattr_t attributes;
        pthread_mutexattr_init(&attributes);
        pthread_mutexattr_setpshared(&attributes, PTHREAD_PROCESS_SHARED);
        pthread_mutexattr_setrobust(&attributes, PTHREAD_MUTEX_ROBUST);

        int result = pthread_mutex_init(&handle.Shared->Mutex, &attributes);
        if (result != 0)
        {
            pthread_mutexattr_destroy(&attributes);
            munmap(handle.Shared, sizeof(SharedMutex));
            shm_unlink(handle.Name.GetData());

            throw std::system_error(result, Detail::Pthread::PthreadErrorCategory::Instance);
        }

        pthread_mutexattr_destroy(&attributes);
    }

    InterprocessMutex::~InterprocessMutex()
    {
        InterprocessMutexHandle& handle = GetUnixHandle(*this);
        if (handle.Shared)
        {
            munmap(handle.Shared, sizeof(SharedMutex));
            handle.Shared = nullptr;
        }
    }

    void InterprocessMutex::Lock()
    {
        InterprocessMutexHandle& handle = GetUnixHandle(*this);

        int result = pthread_mutex_lock(&handle.Shared->Mutex);

        if (result == EOWNERDEAD)
        {
            pthread_mutex_consistent(&handle.Shared->Mutex);
            return;
        }

        if (result != 0)
            throw std::system_error(result, Detail::Pthread::PthreadErrorCategory::Instance);
    }

    bool InterprocessMutex::TryLock()
    {
        InterprocessMutexHandle& handle = GetUnixHandle(*this);

        int result = pthread_mutex_trylock(&handle.Shared->Mutex);

        if (result == 0)
            return true;

        if (result == EBUSY)
            return false;

        throw std::system_error(result, Detail::Pthread::PthreadErrorCategory::Instance);
    }

    void InterprocessMutex::Unlock()
    {
        InterprocessMutexHandle& handle = GetUnixHandle(*this);

        int result = pthread_mutex_unlock(&handle.Shared->Mutex);
        if (result != 0)
            throw std::system_error(result, Detail::Pthread::PthreadErrorCategory::Instance);
    }
}
