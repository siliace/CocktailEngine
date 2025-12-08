#include <Cocktail/Core/Memory/Allocator/MallocAllocator.hpp>

namespace Ck
{
    void* MallocAllocator::Allocate(std::size_t size)
    {
        return std::malloc(size);
    }

    void* MallocAllocator::Reallocate(void* pointer, std::size_t size)
    {
        return std::realloc(pointer, size);
    }

    void MallocAllocator::Free(void* original, std::size_t)
    {
        std::free(original);
    }

    void* MallocAllocationHandler::Allocate(std::size_t size)
    {
        return ResolveFacadeInstance()->Allocate(size);
    }

    void* MallocAllocationHandler::Reallocate(void* pointer, std::size_t size)
    {
        return ResolveFacadeInstance()->Reallocate(pointer, size);
    }

    void MallocAllocationHandler::Free(void* pointer)
    {
        ResolveFacadeInstance()->Free(pointer, 0);
    }
}
