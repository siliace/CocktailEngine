#ifndef COCKTAIL_CORE_MEMORY_ALLOCATOR_THREADLOCALALLOCATORPROXY_HPP
#define COCKTAIL_CORE_MEMORY_ALLOCATOR_THREADLOCALALLOCATORPROXY_HPP

#include <Cocktail/Core/Memory/Allocator/MemoryAllocator.hpp>
#include <Cocktail/Core/System/Concurrency/ThreadLocal.hpp>

namespace Ck
{
    /**
     * \class ThreadLocalAllocatorProxy
     *
     * \brief Per-thread allocator proxy backed by thread-local storage
     *
     * Provides each thread with its own independent instance of
     * \p TAllocator by storing it in thread-local storage. Allocation
     * requests are transparently forwarded to the allocator instance
     * associated with the calling thread, eliminating contention between
     * threads while preserving the \c MemoryAllocator interface.
     *
     * Each thread constructs its allocator lazily upon first use and
     * destroys it automatically when the thread exits, according to the
     * lifetime semantics of \c ThreadLocal.
     *
     * \tparam TAllocator Concrete allocator type to instantiate for each
     *         thread. Must derive from \c MemoryAllocator.
     */
    template <typename TAllocator, typename = std::enable_if_t<std::is_base_of_v<MemoryAllocator, TAllocator>>>
    class ThreadLocalAllocatorProxy : public MemoryAllocator
    {
    public:

        /**
         * \brief Allocates a block of memory using the calling thread's allocator
         *
         * Retrieves the allocator instance associated with the current thread
         * and forwards the allocation request to it.
         *
         * \param size Size of the memory block to allocate, in bytes
         * \param alignment Required alignment of the allocated memory
         *
         * \return Pointer to the allocated memory block, or \c nullptr if the
         *         allocation fails
         */
        void* Allocate(std::size_t size, std::size_t alignment) override
        {
            return GetInnerAllocator()->Allocate(size, alignment);
        }

        /**
         * \brief Resizes a memory block using the calling thread's allocator
         *
         * Retrieves the allocator instance associated with the current thread
         * and forwards the reallocation request to it.
         *
         * \param pointer Pointer to the memory block to resize, or \c nullptr
         * \param size New requested size in bytes
         * \param alignment Required alignment of the memory block
         *
         * \return Pointer to the resized memory block, or \c nullptr if the
         *         operation fails or \p size is 0, depending on the wrapped
         *         allocator's behavior
         */
        void* Reallocate(void* pointer, std::size_t size, std::size_t alignment) override
        {
            return GetInnerAllocator()->Reallocate(pointer, size, alignment);
        }

        /**
         * \brief Frees a memory block using the calling thread's allocator
         *
         * Retrieves the allocator instance associated with the current thread
         * and forwards the deallocation request to it.
         *
         * \param pointer Pointer to the memory block to free
         */
        void Free(void* pointer) override
        {
            GetInnerAllocator()->Free(pointer);
        }

        /**
         * \brief Reports whether the current thread's allocator is thread-safe
         *
         * Forwards the query to the allocator instance associated with the
         * calling thread.
         *
         * \return The value returned by the current thread's allocator
         */
        bool IsThreadSafe() const override
        {
            return GetInnerAllocator()->IsThreadSafe();
        }

        /**
         * \brief Indicates whether this proxy itself is thread-local
         *
         * Although each thread owns an independent allocator instance, the
         * proxy object itself may safely be shared between threads. For this
         * reason it reports \c false.
         *
         * \return Always \c false
         */
        bool IsThreadLocal() const override
        {
            return false;
        }

    private:

        /**
         * \brief Retrieves the allocator instance for the calling thread
         *
         * Returns the allocator stored in the thread-local storage associated
         * with the current thread. The allocator is constructed on first access
         * if it does not already exist for the calling thread.
         *
         * \return Pointer to the current thread's allocator instance
         */
        MemoryAllocator* GetInnerAllocator() const
        {
            return mInnerAllocator.Get();
        }

        mutable ThreadLocal<TAllocator> mInnerAllocator; /*!< Lazily constructed thread-local allocator instance, one per thread */
    };
}

#endif // COCKTAIL_CORE_MEMORY_ALLOCATOR_THREADLOCALALLOCATORPROXY_HPP
