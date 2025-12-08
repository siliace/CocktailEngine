#ifndef COCKTAIL_CORE_MEMORY_ALLOCATOR_USESYSTEMALLOCATOR_HPP
#define COCKTAIL_CORE_MEMORY_ALLOCATOR_USESYSTEMALLOCATOR_HPP

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
    /**
     * \brief Helper class to inherit from to override internal allocators
     *
     * Every class inheriting this one will be allocated using low level allocators instead of
     * the custom system provided by the runtime.
     *
     * This class is recommended to be used only if an object's lifetime is beyond the one of the
     * memory allocator system.
     */
    class COCKTAIL_CORE_API UseSystemAllocator
    {
    public:

        void* operator new(std::size_t size);

        void* operator new[](std::size_t size);

        void operator delete(void* pointer) noexcept;

        void operator delete[](void* pointer) noexcept;
    };
}

#endif //COCKTAIL_CORE_MEMORY_ALLOCATOR_USESYSTEMALLOCATOR_HPP