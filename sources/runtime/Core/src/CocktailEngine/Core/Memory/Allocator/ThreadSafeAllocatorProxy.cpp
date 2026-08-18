#include <CocktailEngine/Core/Memory/Allocator/ThreadSafeAllocatorProxy.hpp>
#include <CocktailEngine/Core/System/Concurrency/LockGuard.hpp>

namespace Ck
{
    ThreadSafeAllocatorProxy::ThreadSafeAllocatorProxy(UniquePtr<MemoryAllocator> innerAllocator) :
        mInnerAllocator(Move(innerAllocator))
    {
        assert(mInnerAllocator != nullptr);
    }

    void* ThreadSafeAllocatorProxy::Allocate(std::size_t size, std::size_t alignment)
    {
        LockGuard<Mutex> lg(mLock);
        return mInnerAllocator->Allocate(size, alignment);
    }

    void* ThreadSafeAllocatorProxy::Reallocate(void* pointer, std::size_t size, std::size_t alignment)
    {
        LockGuard<Mutex> lg(mLock);
        return mInnerAllocator->Reallocate(pointer, size, alignment);
    }

    void ThreadSafeAllocatorProxy::Free(void* pointer)
    {
        LockGuard<Mutex> lg(mLock);
        return mInnerAllocator->Free(pointer);
    }

    bool ThreadSafeAllocatorProxy::IsThreadSafe() const
    {
        return true;
    }

    bool ThreadSafeAllocatorProxy::IsThreadLocal() const
    {
        return false;
    }
}
