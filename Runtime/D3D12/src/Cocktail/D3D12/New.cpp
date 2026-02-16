#include <Cocktail/Core/Memory/Memory.hpp>

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
    return Ck::Memory::Allocate(size, static_cast<std::size_t>(alignment));
}

void* operator new[](std::size_t size, std::align_val_t alignment)
{
    return Ck::Memory::Allocate(size, static_cast<std::size_t>(alignment));
}

void* operator new(std::size_t size, const std::nothrow_t&) noexcept
{
    return Ck::Memory::Allocate(size);
}

void* operator new[](std::size_t size, const std::nothrow_t&) noexcept
{
    return Ck::Memory::Allocate(size);
}

void* operator new(std::size_t size, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    return Ck::Memory::Allocate(size, static_cast<std::size_t>(alignment));
}

void* operator new[](std::size_t size, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    return Ck::Memory::Allocate(size, static_cast<std::size_t>(alignment));
}

void operator delete(void* pointer) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete[](void* pointer) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete(void* pointer, std::size_t size) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete[](void* pointer, std::size_t size) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete(void* pointer, std::size_t size, std::align_val_t alignment) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete[](void* pointer, std::size_t size, std::align_val_t alignment) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete(void* pointer, std::align_val_t alignment) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete[](void* pointer, std::align_val_t alignment) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete(void* pointer, const std::nothrow_t&) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete[](void* pointer, const std::nothrow_t&) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete(void* pointer, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    Ck::Memory::Free(pointer);
}

void operator delete[](void* pointer, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    Ck::Memory::Free(pointer);
}