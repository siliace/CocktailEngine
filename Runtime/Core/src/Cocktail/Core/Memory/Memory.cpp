#include <cstring>

#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/Memory/Memory.hpp>
#include <Cocktail/Core/Memory/Allocator/MallocAllocator.hpp>

void* operator new(std::size_t size)
{
    return Ck::Memory::Allocate(size);
}

void operator delete(void* pointer) noexcept
{
    Ck::Memory::Free(pointer, 0);
}

void* operator new[](std::size_t size)
{
    return Ck::Memory::Allocate(size);
}

void operator delete[](void* pointer) noexcept
{
    Ck::Memory::Free(pointer, 0);
}

namespace Ck
{
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

    void* Memory::Allocate(std::size_t size)
    {
        if (ServiceFacade<MemoryAllocator>::GetApplicationFacade() == nullptr)
            return MallocAllocator().Allocate(size);

        return ServiceFacade<MemoryAllocator>::ResolveFacadeInstance()->Allocate(size);
    }

    void* Memory::Reallocate(void* pointer, std::size_t size)
    {
        if (ServiceFacade<MemoryAllocator>::GetApplicationFacade() == nullptr)
            return MallocAllocator().Reallocate(pointer, size);

        return ServiceFacade<MemoryAllocator>::ResolveFacadeInstance()->Reallocate(pointer, size);
    }

    void Memory::Free(void* pointer, std::size_t size)
    {
        if (ServiceFacade<MemoryAllocator>::GetApplicationFacade() == nullptr)
            return MallocAllocator().Free(pointer, size);

        ServiceFacade<MemoryAllocator>::ResolveFacadeInstance()->Free(pointer, size);
    }
}
