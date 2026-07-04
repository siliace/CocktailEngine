#include <Cocktail/Core/Memory/Allocator/ThreadSafeAllocatorProxy.hpp>

namespace Ck
{
    ThreadSafeAllocatorProxy::ThreadSafeAllocatorProxy(UniquePtr<MemoryAllocator> innerAllocator) :
        mInnerAllocator(std::move(innerAllocator))
    {
        assert(mInnerAllocator != nullptr);
    }

    void* ThreadSafeAllocatorProxy::Allocate(std::size_t size, std::size_t alignment)
    {
        std::lock_guard lg(mLock);
        return mInnerAllocator->Allocate(size, alignment);
    }

    void* ThreadSafeAllocatorProxy::Reallocate(void* pointer, std::size_t size, std::size_t alignment)
    {
        std::lock_guard lg(mLock);
        return mInnerAllocator->Reallocate(pointer, size, alignment);
    }

    void ThreadSafeAllocatorProxy::Free(void* pointer)
    {
        std::lock_guard lg(mLock);
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
