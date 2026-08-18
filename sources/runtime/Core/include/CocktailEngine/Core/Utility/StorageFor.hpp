#ifndef COCKTAILENGINE_CORE_UTILITY_STORAGEFOR_HPP
#define COCKTAILENGINE_CORE_UTILITY_STORAGEFOR_HPP

#include <CocktailEngine/Core/Cocktail.hpp>

namespace Ck
{
    template <std::size_t Alignof, std::size_t Sizeof>
    class StorageForSize
    {
    public:

        static_assert(Alignof > 0);
        static_assert(Sizeof > 0);

        StorageForSize() = default;

        void* GetAddress()
        {
            return mData;
        }

        const void* GetAddress() const
        {
            return mData;
        }

    protected:

        alignas(Alignof) Byte mData[Sizeof];
    };

    template <typename T, std::size_t Count = 1>
    using StorageFor = StorageForSize<alignof(T), sizeof(T) * Count>;
}

#endif // COCKTAILENGINE_CORE_UTILITY_STORAGEFOR_HPP
