#ifndef COCKTAIL_CORE_MEMORY_ALLOCATOR_BINNEDALLOCATOR_HPP
#define COCKTAIL_CORE_MEMORY_ALLOCATOR_BINNEDALLOCATOR_HPP

#include <Cocktail/Core/Memory/Allocator/MemoryAllocator.hpp>
#include <Cocktail/Core/StaticArray.hpp>

namespace Ck
{
    /**
     * \class BinnedAllocator
     *
     * \brief Fixed-bin slab allocator for small to medium allocations
     *
     * Manages a small number of size classes (bins). Each bin holds a list of
     * fixed-size slabs (one or more contiguous OS pages). Allocations that fit
     * within the largest bin are served from a pre-carved free list in O(1);
     * allocations that exceed the largest bin fall back to a direct OS allocation.
     *
     * The allocator imposes no external fragmentation within a slab and recycles
     * fully-empty slabs back to the OS automatically.
     */
    class BinnedAllocator : public MemoryAllocator
    {
    public:

        /**
         * \brief Number of size-class bins managed by this allocator
         */
        static constexpr unsigned int BinCount = 5;

        /**
         * \brief Constructs the allocator and initializes all bin descriptors
         *
         * Reads the compile-time \c BinConfigurations table and populates each
         * \c PageIndex with the corresponding block size and slab geometry.
         * No OS memory is reserved at construction time.
         */
        BinnedAllocator();

        /**
         * \brief Destroys the allocator and releases all outstanding slab memory
         *
         * Walks every bin's page list and returns each slab to the OS via
         * \c SystemMemory::Free, regardless of whether individual blocks within
         * those slabs are still logically "in use".
         */
        ~BinnedAllocator() override;

        /**
         * \brief Allocates a block of memory
         *
         * If the required block size (including \c BlockHeader overhead and
         * alignment padding) fits within a bin, a block is popped from that
         * bin's free list in O(1). A new slab is carved from the OS if the
         * bin has no available page. Requests that exceed the largest bin are
         * forwarded directly to \c SystemMemory::Allocate.
         *
         * \param userSize Size of the memory block to allocate, in bytes
         * \param alignment Alignment constraint of the memory to allocate
         *
         * \return Pointer to the allocated, suitably aligned memory block,
         *         or \c nullptr on failure
         */
        void* Allocate(std::size_t userSize, std::size_t alignment) override;

        /**
         * \brief Resizes an existing memory block, preserving its contents
         *
         * If the new size still fits within the block's current bin slot, the
         * resize is performed in-place by updating the stored size field and
         * returning the same pointer. Otherwise, a new block is allocated, the
         * old contents are copied, and the old block is freed.
         *
         * \param pointer Pointer to the previously allocated block, or \c nullptr
         * \param size New requested size in bytes; passing 0 frees the block
         * \param alignment Alignment constraint of the memory to allocate
         *
         * \return Pointer to the reallocated memory block, or \c nullptr if
         *         \p size is 0 or allocation fails
         */
        void* Reallocate(void* pointer, std::size_t size, std::size_t alignment) override;

        /**
         * \brief Frees a previously allocated memory block
         *
         * Recovers the \c BlockHeader from \p pointer, then either returns the
         * block to its page's free list (binned path) or releases it directly
         * to the OS (large-allocation path). Passing \c nullptr is a no-op.
         *
         * \param pointer Pointer to the memory block to free
         */
        void Free(void* pointer) override;

    private:

        struct PageHeader;
        struct PageIndex;

        /**
         * \struct BlockHeader
         *
         * \brief Metadata stored immediately before every user-visible allocation
         *
         * The header is placed at the start of each fixed-size slot within a slab
         * (binned path) or at the start of a raw OS allocation (large path).
         * It is aligned to \c std::max_align_t so the user pointer returned by
         * \c BlockToUser can be advanced to any required alignment without
         * violating the header's own alignment.
         */
        struct alignas(std::max_align_t) BlockHeader
        {
            PageHeader* Page; /*!< Owning page for binned blocks; \c nullptr for large (direct OS) allocations */
            BlockHeader* NextFree; /*!< Intrusive link to the next free block in this page's free list; only valid while the block is free */
            /**
             * User-requested allocation size with a critical free-sentinel invariant:
             *
             * Holds the number of bytes the caller requested, on \b both the binned and
             * large-allocation paths. Reset to \c 0 when a binned block is freed.
             *
             * \par Free-sentinel invariant (binned path only)
             * \c UserSize == 0 is the canonical signal that a binned block is currently free.
             * This invariant is the \b sole mechanism used by \c Free and \c Reallocate to
             * detect double-free and use-after-free bugs at the \c assert level:
             * \code
             *   Free()       : assert(UserSize > 0)  // fires on double-free
             *   Reallocate() : assert(UserSize > 0)  // fires on use-after-free
             *   Allocate()   : assert(UserSize == 0) // fires on double-allocation
             * \endcode
             *
             * \par Invariant maintenance rules — do not break these
             * - \c AllocatePage must initialize every block's \c UserSize to \c 0.
             * - \c FreeBlock must set \c UserSize to \c 0 \b before re-linking the block onto
             *   the free list.
             * - \c Allocate must set \c UserSize to the user-requested size \b after popping
             *   the block from the free list.
             * - Nothing else may write to \c UserSize on the binned path without maintaining
             *   this invariant.
             */
            std::size_t UserSize;

            /**
             * Total size of the OS allocation in bytes (large-allocation path only).
             *
             * On the large path (\c Page == \c nullptr), holds the full byte count passed
             * to \c SystemMemory::Allocate (i.e. \c sizeof(BlockHeader) + block slot size)
             * so that \c SystemMemory::Free receives the correct size.
             *
             * Always \c 0 on the binned path (\c Page != \c nullptr); the capacity is
             * recoverable as \c Page->Index->BlockSize and does not need to be stored here.
             *
             * \note Together with \c UserSize this replaces the old dual-purpose \c Size
             *       field, giving each path a dedicated field with unambiguous semantics.
             *       Because \c BlockHeader is \c alignas(std::max_align_t) (16 B on x86-64),
             *       the two \c size_t fields occupy the 8 bytes of padding that existed in
             *       the previous single-field layout, so \c sizeof(BlockHeader) is unchanged.
             */
            std::size_t BlockSize;
        };

        /**
         * \struct PageHeader
         *
         * \brief Metadata stored at the beginning of every slab
         *
         * A slab is one or more contiguous OS pages allocated as a unit. The
         * \c PageHeader occupies the first bytes of the slab; the remainder is
         * carved into fixed-size \c BlockHeader slots.
         *
         * Block slots are initialized lazily: \c FirstUninitializedBlock advances
         * through the slab one slot at a time as blocks are first handed out,
         * keeping \c AllocatePage O(1). Once a block has been initialized and
         * subsequently freed, it lives on the \c FirstFreeBlock list and is
         * never touched via \c FirstUninitializedBlock again.
         */
        struct alignas(std::max_align_t) PageHeader
        {
            PageHeader* Previous; /*!< Previous page in this bin's doubly-linked page list; \c nullptr if this is the first page */
            PageHeader* Next; /*!< Next page in this bin's doubly-linked page list; \c nullptr if this is the last page */
            PageIndex* Index; /*!< Back-pointer to the \c PageIndex (bin descriptor) that owns this page */
            BlockHeader* FirstFreeBlock; /*!< Head of the singly-linked free list of recycled (previously freed) blocks within this page */
            std::size_t FreeBlockCount; /*!< Total available blocks: freed (on \c FirstFreeBlock list) + not-yet-initialized (before \c FirstUninitializedBlock end) */
            /**
             * Bump pointer into the contiguous uninitialized region of the slab.
             *
             * Points to the next block slot that has never been handed out. Advances
             * by \c Index->BlockSize each time a fresh slot is consumed. Set to
             * \c nullptr once the last uninitialized slot has been consumed, so that
             * any attempt to take another slot off the lazy-init path asserts rather
             * than silently writing past the end of the slab.
             *
             * Set to the first block slot address by \c AllocatePage; advanced (and
             * nulled when exhausted) by \c Allocate on the lazy-init path; never
             * written by \c FreeBlock.
             */
            BlockHeader* FirstUninitializedBlock;
            std::size_t UninitializedBlockCount;
        };

        /**
         * \struct PageIndex
         *
         * \brief Descriptor for a single size-class bin
         *
         * Each \c PageIndex tracks the geometry of its slabs and maintains
         * two lists: all pages belonging to the bin, and the subset of those
         * pages that still have at least one free block.
         *
         * To avoid allocation thrashing under alternating alloc/free patterns,
         * each bin keeps one fully-empty slab as a \e reserve instead of
         * returning it to the OS immediately. The reserve is reused by
         * \c AllocatePage before a new OS allocation is made, and is only
         * released when a \e second fully-empty slab would need to be retained
         * at the same time (i.e. the reserve slot is already occupied).
         */
        struct PageIndex
        {
            std::size_t SlabPageCount = 1; /*!< Number of OS pages that make up one slab for this bin */
            std::size_t BlockCount = 0; /*!< Number of blocks carved out of each slab */
            std::size_t BlockSize = 0; /*!< Size of each block slot in bytes (includes \c BlockHeader overhead and alignment padding reserve) */
            PageHeader* FirstPage = nullptr; /*!< Head of the doubly-linked list of all slabs belonging to this bin */
            PageHeader* FirstAvailablePage = nullptr; /*!< First page in the list that has at least one free block; \c nullptr when the bin is fully exhausted */
            /**
             * Single fully-empty slab held in reserve to avoid OS round-trips under
             * alternating alloc/free workloads.
             *
             * When a page becomes fully empty, \c FreeBlock stores it here instead of
             * releasing it to the OS immediately, provided this field is \c nullptr.
             * If this field is already occupied, the newly-emptied page is released to
             * the OS instead (so at most one reserve slab is held at any time).
             *
             * \c AllocatePage checks this field first; if non-\c nullptr it resets and
             * re-links the reserved page rather than asking the OS for a new slab.
             * The reserve is released unconditionally in \c ~BinnedAllocator.
             */
            PageHeader* ReservedPage = nullptr; /*!< One fully-empty slab kept off the active list as a reuse buffer; \c nullptr if no reserve is held */
        };

        /**
         * \brief Computes the total allocation size required for a user allocation
         *
         * Accounts for the \c BlockHeader, the alignment-offset word written by
         * \c BlockToUser, and enough padding bytes to guarantee the offset word
         * always fits immediately before the aligned user pointer regardless of
         * the requested alignment.
         *
         * \param allocationSize User-requested allocation size in bytes
         * \param alignment Required alignment of the user pointer
         *
         * \return Minimum allocation size in bytes that can satisfy the request
         */
        static std::size_t ComputeAllocationSize(std::size_t allocationSize, std::size_t alignment);

        /**
         * \brief Computes the aligned user pointer from a raw block pointer
         *
         * Advances past the \c BlockHeader, reserves space for the offset word,
         * aligns the result to \p alignment, then writes the byte distance from
         * the block start into the word immediately preceding the returned pointer.
         * This offset is later used by \c UserToBlock to recover the block.
         *
         * \param block     Pointer to the \c BlockHeader of the allocated block
         * \param alignment Required alignment of the user pointer
         *
         * \return Aligned pointer to the usable memory region
         */
        static void* BlockToUser(BlockHeader* block, std::size_t alignment);

        /**
         * \brief Recovers the \c BlockHeader pointer from a user pointer
         *
         * Reads the offset word stored immediately before \p userPointer (written
         * by \c BlockToUser) and subtracts it from \p userPointer to obtain the
         * original block address.
         *
         * \param userPointer Pointer previously returned by \c BlockToUser
         *
         * \return Pointer to the corresponding \c BlockHeader
         */
        static BlockHeader* UserToBlock(Byte* userPointer);

        /**
         * \brief Finds the smallest bin whose block size can accommodate a request
         *
         * Performs a linear scan over \c mPageIndexes in ascending block-size order
         * and returns the first bin whose \c BlockSize is greater than or equal to
         * \p blockSize.
         *
         * \param allocationSize Required allocation size in bytes, as returned by \c ComputeAllocationSize
         *
         * \return Pointer to the matching \c PageIndex, or \c nullptr if \p blockSize
         *         exceeds all bin sizes (large-allocation fallback)
         */
        PageIndex* FindPageIndex(std::size_t allocationSize);

        /**
         * \brief Carves a new slab from the OS and links it into a bin (O(1))
         *
         * Allocates \c index->SlabPageCount OS pages, initializes the \c PageHeader
         * at the start of the slab, and prepends the new page to \c index->FirstPage.
         * The new page becomes \c index->FirstAvailablePage.
         *
         * Block slots are \b not initialized here; \c PageHeader::FirstUninitializedBlock
         * is set to the first slot address and blocks are lazily initialized by
         * \c Allocate the first time each slot is handed out. This keeps
         * \c AllocatePage O(1) regardless of how many blocks fit in a slab.
         *
         * \param index Bin descriptor to allocate a new slab for
         */
        static void AllocatePage(PageIndex* index);

        /**
         * \brief Returns a single block to its page's free list
         *
         * Pushes \p block onto the head of its page's free list, clears
         * \c block->UserSize to 0 (the free sentinel), and increments
         * \c FreeBlockCount.
         *
         * When the page becomes fully empty, it is unlinked from the bin's active
         * page list and either parked as \c index->ReservedPage (if no reserve is
         * held yet) or released to the OS immediately (if the reserve slot is
         * already occupied). This keeps at most one empty slab buffered per bin,
         * avoiding OS round-trips under alternating alloc/free workloads.
         *
         * If the page still has free blocks, \c index->FirstAvailablePage is
         * updated to this page whenever it now holds more free blocks than the
         * current first-available candidate.
         *
         * \param block Block to return to the free list; must belong to a binned page
         */
        static void FreeBlock(BlockHeader* block);

        PageIndex mPageIndexes[BinCount]; /*!< Per-bin descriptors, one entry per size class, initialized at construction from \c BinConfigurations */
    };
}

#endif // COCKTAIL_CORE_MEMORY_ALLOCATOR_BINNEDALLOCATOR_HPP
