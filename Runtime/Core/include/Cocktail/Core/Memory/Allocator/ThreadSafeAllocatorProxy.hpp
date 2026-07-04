#ifndef COCKTAIL_CORE_MEMORY_ALLOCATOR_THREADSAFEALLOCATORPROXY_HPP
#define COCKTAIL_CORE_MEMORY_ALLOCATOR_THREADSAFEALLOCATORPROXY_HPP

#include <mutex>

#include <Cocktail/Core/Memory/Allocator/MemoryAllocator.hpp>
#include <Cocktail/Core/Memory/UniquePtr.hpp>

namespace Ck
{
    /**
     * \class ThreadSafeAllocatorProxy
     *
     * \brief Thread-safe wrapper around another memory allocator
     *
     * Serializes all allocation operations performed on an underlying
     * \c MemoryAllocator by protecting every public entry point with a
     * single mutex. This allows allocators that are not inherently
     * thread-safe to be safely shared between multiple threads without
     * modifying their implementation.
     *
     * The proxy owns the wrapped allocator and forwards all allocation,
     * reallocation, and deallocation requests to it while holding the
     * internal lock.
     */
    class COCKTAIL_CORE_API ThreadSafeAllocatorProxy : public MemoryAllocator
    {
    public:

        /**
         * \brief Constructs a thread-safe proxy around an existing allocator
         *
         * Takes ownership of \p innerAllocator, which will receive all future
         * allocation requests through this proxy.
         *
         * \param innerAllocator Allocator to wrap; ownership is transferred to
         *                       the proxy
         */
        explicit ThreadSafeAllocatorProxy(UniquePtr<MemoryAllocator> innerAllocator);

        /**
         * \brief Allocates a block of memory in a thread-safe manner
         *
         * Acquires the internal mutex, forwards the allocation request to the
         * wrapped allocator, then releases the mutex before returning.
         *
         * \param size Size of the memory block to allocate, in bytes
         * \param alignment Required alignment of the allocated memory
         *
         * \return Pointer to the allocated memory block, or \c nullptr if the
         *         allocation fails
         */
        void* Allocate(std::size_t size, std::size_t alignment) override;

        /**
         * \brief Resizes an existing memory block in a thread-safe manner
         *
         * Acquires the internal mutex, forwards the reallocation request to the
         * wrapped allocator, then releases the mutex before returning.
         *
         * \param pointer Pointer to the memory block to resize, or \c nullptr
         * \param size New requested size in bytes
         * \param alignment Required alignment of the memory block
         *
         * \return Pointer to the resized memory block, or \c nullptr if the
         *         operation fails or \p size is 0, depending on the wrapped
         *         allocator's behavior
         */
        void* Reallocate(void* pointer, std::size_t size, std::size_t alignment) override;

        /**
         * \brief Frees a memory block in a thread-safe manner
         *
         * Acquires the internal mutex, forwards the deallocation request to the
         * wrapped allocator, then releases the mutex once the operation has
         * completed.
         *
         * \param pointer Pointer to the memory block to free
         */
        void Free(void* pointer) override;

        /**
         * \brief Tell whether the allocator needs to be externalized synchronized
         *
         * \return True if the allocators needs external synchronization, false otherwise
         */
        bool IsThreadSafe() const override;

        /**
         * \brief Tell whether the allocator should be instantiated per thread
         *
         * \return True if the allocators is per thread, false otherwise
         */
        bool IsThreadLocal() const override;

    private:

        std::mutex mLock; /*!< Mutex serializing all accesses to the wrapped allocator */
        UniquePtr<MemoryAllocator> mInnerAllocator; /*!< Owned allocator receiving all forwarded allocation requests */
    };
}

#endif // COCKTAIL_CORE_MEMORY_ALLOCATOR_THREADSAFEALLOCATORPROXY_HPP
