#include <Cocktail/Core/Memory/Allocator/MallocAllocationHandler.hpp>

namespace Ck
{
    void* MallocAllocationHandler::Allocate(std::size_t size, std::size_t alignment)
    {
        return ResolveFacadeInstance()->Allocate(size, alignment);
    }

    void* MallocAllocationHandler::Reallocate(void* pointer, std::size_t size, std::size_t alignment)
    {
        return ResolveFacadeInstance()->Reallocate(pointer, size, alignment);
    }

    void MallocAllocationHandler::Free(void* pointer)
    {
        ResolveFacadeInstance()->Free(pointer);
    }
}