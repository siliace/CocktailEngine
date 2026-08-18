#ifndef COCKTAILENGINE_CORE_MEMORY_ALLOCATOR_USESYSTEMALLOCATOR_HPP
#define COCKTAILENGINE_CORE_MEMORY_ALLOCATOR_USESYSTEMALLOCATOR_HPP

#include <CocktailEngine/Core/Export.hpp>

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
    class COCKTAILENGINE_CORE_API UseSystemAllocator
    {
    public:

        void* operator new(std::size_t size);

        void* operator new[](std::size_t size);

        void operator delete(void* pointer) noexcept;

        void operator delete[](void* pointer) noexcept;
    };
}

#endif //COCKTAILENGINE_CORE_MEMORY_ALLOCATOR_USESYSTEMALLOCATOR_HPP