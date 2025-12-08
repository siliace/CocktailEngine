#include <Cocktail/Core/Memory/Allocator/UseSystemAllocator.hpp>

namespace Ck
{
    void* UseSystemAllocator::operator new(std::size_t size)
    {
        return std::malloc(size);
    }

    void* UseSystemAllocator::operator new [](std::size_t size)
    {
        return std::malloc(size);
    }

    void UseSystemAllocator::operator delete(void* pointer) noexcept
    {
        std::free(pointer);
    }

    void UseSystemAllocator::operator delete [](void* pointer) noexcept
    {
        std::free(pointer);
    }
}
