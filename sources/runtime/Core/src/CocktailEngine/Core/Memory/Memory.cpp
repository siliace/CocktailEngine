#include <cstring>

#include <CocktailEngine/Core/Application/ServiceFacade.hpp>
#include <CocktailEngine/Core/Memory/Allocator/BinnedAllocator.hpp>
#include <CocktailEngine/Core/Memory/Allocator/ThreadLocalAllocatorProxy.hpp>
#include <CocktailEngine/Core/Memory/Allocator/ThreadSafeAllocatorProxy.hpp>
#include <CocktailEngine/Core/Memory/Memory.hpp>
#include <CocktailEngine/Core/System/Concurrency/Thread.hpp>
#include <CocktailEngine/Core/System/SystemMemory.hpp>

namespace Ck
{
    namespace
    {
        MemoryAllocator* gAllocator;

        template <typename TAllocator, typename = std::enable_if_t<std::is_base_of_v<MemoryAllocator, TAllocator>>>
        void CreateGlobalAllocator()
        {
            assert(!gAllocator);

            UniquePtr<MemoryAllocator> allocator = MakeUnique<TAllocator>();
            if (!allocator->IsThreadSafe())
            {
                gAllocator = new ThreadSafeAllocatorProxy(Move(allocator));
            }
            else if (allocator->IsThreadLocal())
            {
                gAllocator = new ThreadLocalAllocatorProxy<TAllocator>();
            }
            else
            {
                gAllocator = allocator.Release();
            }

            assert(gAllocator && gAllocator->IsThreadSafe() && !gAllocator->IsThreadLocal());
        }
    }

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
        if (!gAllocator)
        {
            CreateGlobalAllocator<BinnedAllocator>();
            assert(gAllocator);
        }

        return gAllocator->Allocate(size, alignment);
    }

    void* Memory::Reallocate(void* pointer, std::size_t size, std::size_t alignment)
    {
        if (!gAllocator)
        {
            CreateGlobalAllocator<BinnedAllocator>();
            assert(gAllocator);
        }

        return gAllocator->Reallocate(pointer, size, alignment);
    }

    void Memory::Free(void* pointer)
    {
        if (!gAllocator)
        {
            CreateGlobalAllocator<BinnedAllocator>();
            assert(gAllocator);
        }

        gAllocator->Free(pointer);
    }
}
