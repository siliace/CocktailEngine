#ifndef COCKTAIL_CORE_MEMORY_ALLOCATOR_STLADAPTER_HPP
#define COCKTAIL_CORE_MEMORY_ALLOCATOR_STLADAPTER_HPP

#include <Cocktail/Core/Cocktail.hpp>

namespace Ck
{
    template <typename T, typename TSource>
    class StlAdapter
    {
        using SourceAllocatorType = typename TSource::template ForType<T>;

    public:

        template <class U>
        struct rebind
        {
            using other = StlAdapter<U, TSource>;
        };

        using value_type = typename SourceAllocatorType::ValueType;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using size_type = typename TSource::SizeType;
        using difference_type = std::ptrdiff_t;
        using is_always_equal = std::true_type;
        using propagate_on_container_copy_assignment = std::true_type;
        using propagate_on_container_move_assignment = std::true_type;
        using propagate_on_container_swap = std::true_type;

        StlAdapter() = default;
        StlAdapter(const StlAdapter& other) = default;
        StlAdapter(StlAdapter&& other) noexcept = default;
        StlAdapter& operator=(const StlAdapter& other) = default;
        StlAdapter& operator=(StlAdapter&& other) noexcept = default;

        template <class U>
        StlAdapter(const StlAdapter<U, TSource>&) noexcept
        {
        }

        pointer allocate(size_type size)
        {
            return mImplementation.Allocate(size);
        }

        void deallocate(pointer pointer, size_type n)
        {
            return mImplementation.Deallocate(pointer);
        }

        size_type max_size() const noexcept
        {
            return std::numeric_limits<size_type>::max() / sizeof(value_type);
        }

        bool operator==(const StlAdapter&) const noexcept
        {
            return true;
        }
        bool operator!=(const StlAdapter&) const noexcept
        {
            return false;
        }

    private:

        SourceAllocatorType mImplementation;
    };
}

#endif // COCKTAIL_CORE_MEMORY_ALLOCATOR_STLADAPTER_HPP
