#ifndef COCKTAILENGINE_CORE_MEMORY_ALLOCATOR_SIZETYPE_HPP
#define COCKTAILENGINE_CORE_MEMORY_ALLOCATOR_SIZETYPE_HPP

#include <CocktailEngine/Core/Cocktail.hpp>

namespace Ck
{
    template <int>
    struct SizeTypeFromIndexBits
    {
    };

    template <>
    struct SizeTypeFromIndexBits<8>
    {
        using Type = Uint8;
    };

    template <>
    struct SizeTypeFromIndexBits<16>
    {
        using Type = Uint16;
    };

    template <>
    struct SizeTypeFromIndexBits<32>
    {
        using Type = Uint32;
    };

    template <>
    struct SizeTypeFromIndexBits<64>
    {
        using Type = Uint64;
    };

    template <int Size>
    using SizeTypeOf = typename SizeTypeFromIndexBits<Size>::Type;
}

#endif // COCKTAILENGINE_CORE_MEMORY_ALLOCATOR_SIZETYPE_HPP
