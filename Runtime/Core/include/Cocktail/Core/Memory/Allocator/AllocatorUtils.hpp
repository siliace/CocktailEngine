#ifndef COCKTAIL_CORE_MEMORY_ALLOCATOR_ALLOCATORUTILS_HPP
#define COCKTAIL_CORE_MEMORY_ALLOCATOR_ALLOCATORUTILS_HPP

#include <type_traits>
#include <utility>

namespace Ck
{
    class AllocatorUtils
    {
    public:

        /**
         * \brief Selects the allocator instance used when copy-constructing a container
         *
         * This function defines how the allocator is transferred or replaced during
         * container copy construction.
         *
         * If \c AllocatorElementType::PropagateOnContainerCopy is true, the allocator
         * from the source container is copy-constructed and reused by the new container.
         *
         * Otherwise, a new default-constructed allocator instance is created, ensuring
         * that the destination container has its own independent allocation context.
         *
         * \param other Allocator instance owned by the source container
         *
         * \return Allocator instance to associate with the newly constructed container
         */
        template <typename TTypedAllocator>
        static TTypedAllocator CopyPropagateAllocator(const TTypedAllocator& other)
        {
            if constexpr (TTypedAllocator::PropagateOnContainerCopy)
            {
                static_assert(std::is_copy_constructible_v<TTypedAllocator>);
                return other;
            }
            else
            {
                static_assert(std::is_default_constructible_v<TTypedAllocator>);
                return TTypedAllocator{};
            }
        }

        /**
         * \brief Selects the allocator instance used when move-constructing a container
         *
         * This function defines how the allocator is transferred or replaced during
         * container move construction.
         *
         * If \c AllocatorElementType::PropagateOnContainerMove is true, ownership of the
         * allocator is transferred from the source container to the destination container.
         *
         * Otherwise, a new default-constructed allocator instance is created, and the
         * destination container remains independent from the source allocator.
         *
         * \param other Allocator instance owned by the source container
         *
         * \return Allocator instance to associate with the newly constructed container
         */
        template <typename TTypedAllocator>
        static TTypedAllocator MovePropagateAllocator(TTypedAllocator& other)
        {
            if constexpr (TTypedAllocator::PropagateOnContainerMove)
            {
                static_assert(std::is_move_constructible_v<TTypedAllocator>);
                return std::move(other);
            }
            else
            {
                static_assert(std::is_default_constructible_v<TTypedAllocator>);
                return TTypedAllocator{};
            }
        }
    };
}

#endif // COCKTAILENGINE_ALLOCATORUTILS_HPP
