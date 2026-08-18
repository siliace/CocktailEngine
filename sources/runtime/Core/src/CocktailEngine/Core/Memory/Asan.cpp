#ifdef CK_ASAN
#include <sanitizer/asan_interface.h>
#endif

#include <CocktailEngine/Core/Memory/Asan.hpp>

namespace Ck
{
    void Asan::Poison(void* pointer, Uint64 size)
    {
#ifdef CK_ASAN
        if (size)
            __asan_poison_memory_region(pointer, size);
#endif
    }

    bool Asan::IsPoisoned(void* pointer)
    {
#ifdef CK_ASAN
        return __asan_address_is_poisoned(pointer);
#endif
    }

    bool Asan::IsRangePoisoned(void* pointer, Uint64 size)
    {
#ifdef CK_ASAN
        if (size)
            return __asan_region_is_poisoned(pointer, size);
#endif

        return false;
    }

    void Asan::Unpoison(void* pointer, Uint64 size)
    {
#ifdef CK_ASAN
        if (size)
            __asan_unpoison_memory_region(pointer, size);
#endif
    }

    void Asan::AnnotateContiguousContainer(void* pointer, Uint64 elementSize, Uint64 range, Uint64 previous, Uint64 next)
    {
#ifdef CK_ASAN
        if (elementSize)
        {
            __sanitizer_annotate_contiguous_container(
                pointer,
                static_cast<Byte*>(pointer) + elementSize * range,
                static_cast<Byte*>(pointer) + elementSize * previous,
                static_cast<Byte*>(pointer) + elementSize * next
            );
        }
#endif
    }
}
