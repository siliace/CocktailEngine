#include <cstring>

#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/Memory/Memory.hpp>
#include <Cocktail/Core/Memory/Allocator/BinnedAllocator.hpp>

void* operator new(std::size_t size)
{
    return Ck::Memory::Allocate(size);
}

void* operator new[](std::size_t size)
{
    return Ck::Memory::Allocate(size);
}

void* operator new(std::size_t size, std::align_val_t alignment)
{
    return Ck::Memory::Allocate(size);
}

void* operator new[](std::size_t size, std::align_val_t alignment)
{
    return Ck::Memory::Allocate(size);
}

void* operator new(std::size_t size, const std::nothrow_t&)
{
    return Ck::Memory::Allocate(size);
}

void* operator new[](std::size_t size, const std::nothrow_t&)
{
    return Ck::Memory::Allocate(size);
}

void* operator new(std::size_t size, std::align_val_t alignment, const std::nothrow_t&)
{
    return Ck::Memory::Allocate(size);
}

void* operator new[](std::size_t size, std::align_val_t alignment, const std::nothrow_t&)
{
    return Ck::Memory::Allocate(size);
}

void operator delete(void* pointer) noexcept
{
    return Ck::Memory::Free(pointer);
}

void operator delete[](void* pointer) noexcept
{
    return Ck::Memory::Free(pointer);
}

void operator delete(void* pointer, std::size_t size) noexcept
{
    return Ck::Memory::Free(pointer);
}

void operator delete[](void* pointer, std::size_t size) noexcept
{
    return Ck::Memory::Free(pointer);
}

void operator delete(void* pointer, std::size_t size, std::align_val_t alignment) noexcept
{
    return Ck::Memory::Free(pointer);
}

void operator delete[](void* pointer, std::size_t size, std::align_val_t alignment) noexcept
{
    return Ck::Memory::Free(pointer);
}

void operator delete(void* pointer, std::align_val_t alignment) noexcept
{
    return Ck::Memory::Free(pointer);
}

void operator delete[](void* pointer, std::align_val_t alignment) noexcept
{
    return Ck::Memory::Free(pointer);
}

void operator delete(void* pointer, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    return Ck::Memory::Free(pointer);
}

void operator delete[](void* pointer, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    return Ck::Memory::Free(pointer);
}

namespace Ck
{
    thread_local MemoryAllocator* gAllocator;

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
            CreateGlobalAllocator();
            assert(gAllocator != nullptr);
        }

        return gAllocator->Allocate(size, alignment);
    }

    void* Memory::Reallocate(void* pointer, std::size_t size, std::size_t alignment)
    {
        if (!gAllocator)
        {
            CreateGlobalAllocator();
            assert(gAllocator != nullptr);
        }

        return gAllocator->Reallocate(pointer, size, alignment);
    }

    void Memory::Free(void* pointer)
    {
        if (!gAllocator)
        {
            CreateGlobalAllocator();
            assert(gAllocator != nullptr);
        }

        gAllocator->Free(pointer);
    }

    void Memory::CreateGlobalAllocator()
    {
        gAllocator = new BinnedAllocator<8, 32>();
    }
}
