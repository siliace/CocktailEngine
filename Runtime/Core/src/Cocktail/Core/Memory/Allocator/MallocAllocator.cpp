#include <Cocktail/Core/Memory/Allocator/MallocAllocator.hpp>

namespace Ck
{
    void* MallocAllocator::Allocate(std::size_t size, std::size_t)
    {
        return std::malloc(size);
    }

    void* MallocAllocator::Reallocate(void* pointer, std::size_t size, std::size_t)
    {
        return std::realloc(pointer, size);
    }

    void MallocAllocator::Free(void* original)
    {
        std::free(original);
    }

    void* MallocAllocationHandler::Allocate(std::size_t size)
    {
        return ResolveFacadeInstance()->Allocate(size, 0);
    }

    void* MallocAllocationHandler::Reallocate(void* pointer, std::size_t size)
    {
        return ResolveFacadeInstance()->Reallocate(pointer, size, 0);
    }

    void MallocAllocationHandler::Free(void* pointer)
    {
        ResolveFacadeInstance()->Free(pointer);
    }
}
