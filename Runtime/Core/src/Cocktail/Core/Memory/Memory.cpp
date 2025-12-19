#include <cstring>

#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/Memory/Memory.hpp>
#include <Cocktail/Core/Memory/Allocator/BinnedAllocator.hpp>

namespace Ck
{
    std::mutex gAllocatorLock;
    static MemoryAllocator* gAllocator = nullptr;

    void Memory::Zero(void* destination, std::size_t size)
    {
        Set(destination, 0, size);
    }

    void Memory::Set(void* destination, Byte value, std::size_t size)
    {
        std::memset(destination, value, size);
    }

    void Memory::Copy(void* destination, const void* source, std::size_t size)
    {
        std::memcpy(destination, source, size);
    }

    void Memory::Move(void* destination, const void* source, std::size_t size)
    {
        std::memmove(destination, source, size);
    }

    void* Memory::Allocate(std::size_t size, std::size_t alignment)
    {
        std::lock_guard lg(gAllocatorLock);
        if (!gAllocator)
        {
            CreateGlobalAllocator();
            assert(gAllocator != nullptr);
        }

        return gAllocator->Allocate(size, alignment);
    }

    void* Memory::Reallocate(void* pointer, std::size_t size, std::size_t alignment)
    {
        std::lock_guard lg(gAllocatorLock);
        if (!gAllocator)
        {
            CreateGlobalAllocator();
            assert(gAllocator != nullptr);
        }

        return gAllocator->Reallocate(pointer, size, alignment);
    }

    void Memory::Free(void* pointer)
    {
        std::lock_guard lg(gAllocatorLock);
        if (!gAllocator)
        {
            CreateGlobalAllocator();
            assert(gAllocator != nullptr);
        }

        gAllocator->Free(pointer);
    }

    void Memory::CreateGlobalAllocator()
    {
        gAllocator = new BinnedAllocator<4>();
    }
}
