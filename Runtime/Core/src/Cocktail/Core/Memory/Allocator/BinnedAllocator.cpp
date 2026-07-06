#include <Cocktail/Core/Memory/Allocator/BinnedAllocator.hpp>
#include <Cocktail/Core/Memory/Memory.hpp>
#include <Cocktail/Core/System/SystemMemory.hpp>

namespace Ck
{
    /**
     * \struct BinConfiguration
     *
     * \brief Compile-time descriptor for a single size-class bin
     *
     * Pairs a block payload size with the number of OS pages that form one slab
     * for that bin. These values are consumed once at \c BinnedAllocator
     * construction time to populate the runtime \c PageIndex array.
     */
    struct BinConfiguration
    {
        std::size_t BlockSize; /*!< Maximum user-visible block size served by this bin, in bytes */
        std::size_t SlabPageCount; /*!< Number of contiguous OS pages that form one slab for this bin */
    };

    /**
     * \brief Fixed bin layout table used to initialise \c BinnedAllocator::mPageIndexes
     *
     * Each entry defines a size class. Block sizes double with each bin, keeping the
     * number of distinct classes small while covering the common allocation range up to
     * 1 KiB. Larger bins use multipage slabs so that the fixed \c PageHeader overhead
     * (48 B) remains a small fraction of the slab, maintaining a healthy blocks-per-slab
     * ratio and reducing the relative cost of OS round-trips:
     *
     * | Bin | BlockSize | SlabPages | Blocks/slab |
     * |-----|-----------|-----------|-------------|
     * |  0  |    64 B   |     1     |      63     |
     * |  1  |   128 B   |     1     |      31     |
     * |  2  |   256 B   |     1     |      15     |
     * |  3  |   512 B   |     2     |      15     |
     * |  4  |  1024 B   |     3     |      11     |
     *
     * (Figures assume BlockHeader = 24 B, PageHeader = 48 B, OS page = 4 KiB.)
     */
    static constexpr StaticArray<BinConfiguration, BinnedAllocator::BinCount> BinConfigurations = MakeStaticArray<BinConfiguration>(
        BinConfiguration{ 64, 1 }, BinConfiguration{ 128, 1 }, BinConfiguration{ 256, 1 }, BinConfiguration{ 512, 2 }, BinConfiguration{ 1024, 3 });

    BinnedAllocator::BinnedAllocator()
    {
        for (unsigned int i = 0; i < BinCount; ++i)
        {
            std::size_t slabSize = BinConfigurations[i].SlabPageCount * SystemMemory::GetPageSize();
            std::size_t blockCount = (slabSize - sizeof(PageHeader)) / BinConfigurations[i].BlockSize;

            mPageIndexes[i].SlabPageCount = BinConfigurations[i].SlabPageCount;
            mPageIndexes[i].BlockCount = blockCount;
            mPageIndexes[i].BlockSize = BinConfigurations[i].BlockSize;
            mPageIndexes[i].FirstPage = nullptr;
            mPageIndexes[i].FirstAvailablePage = nullptr;
        }
    }

    BinnedAllocator::~BinnedAllocator()
    {
        for (PageIndex& pageIndex : mPageIndexes)
        {
            std::size_t slabSize = pageIndex.SlabPageCount * SystemMemory::GetPageSize();

            PageHeader* current = pageIndex.FirstPage;
            while (current)
            {
                PageHeader* next = current->Next;
                SystemMemory::Free(current, slabSize);
                current = next;
            }

            // Release the reserved empty page if one is held
            if (pageIndex.ReservedPage)
            {
                SystemMemory::Free(pageIndex.ReservedPage, slabSize);
                pageIndex.ReservedPage = nullptr;
            }
        }
    }

    void* BinnedAllocator::Allocate(std::size_t userSize, std::size_t alignment)
    {
        BlockHeader* allocatedBlock;

        // Reclaim blocks freed by other threads before allocating
        DrainRemoteFrees();

        // We don't want to allocate 0 bytes even if it's a valid call
        userSize = std::max(static_cast<std::size_t>(1), userSize);
        std::size_t allocationSize = ComputeAllocationSize(userSize, alignment);
        if (PageIndex* index = FindPageIndex(allocationSize))
        {
            if (!index->FirstAvailablePage)
                AllocatePage(index);

            PageHeader* page = index->FirstAvailablePage;
            assert(page && "No available page found");

            // Pop a recycled block from the free list first (O(1))
            allocatedBlock = page->FirstFreeBlock;
            if (allocatedBlock)
            {
                page->FirstFreeBlock = allocatedBlock->NextFree;
            }
            else
            {
                assert(page->UninitializedBlockCount > 0 && page->FirstUninitializedBlock && "No uninitialized blocks remaining but FreeBlockCount > 0");
                allocatedBlock = page->FirstUninitializedBlock;

                --page->UninitializedBlockCount;
                if (page->UninitializedBlockCount)
                {
                    page->FirstUninitializedBlock = reinterpret_cast<BlockHeader*>(reinterpret_cast<Byte*>(page->FirstUninitializedBlock) + index->BlockSize);
                }
                else
                {
                    page->FirstUninitializedBlock = nullptr;
                }

                // Initialize the block header for the first time
                allocatedBlock->Page = page;
                allocatedBlock->NextFree = nullptr;
                // Free-sentinel invariant: UserSize == 0 marks this block as free until
                // Allocate claims it below. BlockSize is 0 on the binned path.
                allocatedBlock->UserSize = 0;
                allocatedBlock->BlockSize = 0;
            }
            --page->FreeBlockCount;

            assert(page->FreeBlockCount == 0 || page->FirstFreeBlock != nullptr || page->FirstUninitializedBlock != nullptr);

            // Free-sentinel invariant: a freshly-popped block must have UserSize == 0,
            // which is the canonical "this block is free" marker written by FreeBlock
            // and by AllocatePage at slab initialization. A non-zero value here means
            // the block was handed out twice without an intervening free (double-alloc).
            assert(allocatedBlock->UserSize == 0 && "Double allocating same memory");

            // Claim the block: set UserSize to the user-requested size (> 0 by this point
            // because size was clamped to at least 1 above). This transitions the block
            // from the "free" state (UserSize == 0) to the "allocated" state (UserSize > 0).
            // BlockSize stays 0 — capacity on the binned path is always recovered from
            // block->Page->Index->BlockSize and does not need to be stored per block.
            allocatedBlock->UserSize = userSize;

            // If page has no more free blocks, advance FirstAvailablePage
            if (page->FreeBlockCount == 0)
            {
                PageHeader* availablePage = page->Next;
                while (availablePage && availablePage->FreeBlockCount == 0)
                    availablePage = availablePage->Next;

                index->FirstAvailablePage = availablePage;
            }
        }
        else
        {
            allocatedBlock = static_cast<BlockHeader*>(SystemMemory::Allocate(allocationSize));
            assert(allocatedBlock && "malloc failed for large allocation");

            allocatedBlock->Page = nullptr;
            allocatedBlock->NextFree = nullptr;
            allocatedBlock->UserSize = userSize;
            allocatedBlock->BlockSize = allocationSize;
        }

        return BlockToUser(allocatedBlock, alignment);
    }

    void* BinnedAllocator::Reallocate(void* pointer, std::size_t size, std::size_t alignment)
    {
        assert(pointer || size > 0);

        if (!pointer)
            return Allocate(size, alignment);

        if (size == 0)
        {
            Free(pointer);
            return nullptr;
        }

        // Reclaim blocks freed by other threads before reallocating
        DrainRemoteFrees();

        BlockHeader* block = UserToBlock(static_cast<Byte*>(pointer));
        // Free-sentinel invariant check: UserSize == 0 on the binned path means the block is
        // already free. Reallocating a freed block (use-after-free) is caught here before
        // any memory is read or written.
        assert(block->UserSize > 0 && "Reallocating a freed block (Use-After-Free detected)");

        // Save the pre-modification user size so the copy path below always
        // transfers the correct number of valid bytes even when this block was
        // previously shrunk in-place (which would have reduced block->UserSize).
        const std::size_t oldSize = block->UserSize;

        // Capacity of the current slot: for binned blocks it is the bin's fixed block
        // size; for large blocks it is the full OS allocation size stored in BlockSize.
        std::size_t blockMaxSize = block->Page ? block->Page->Index->BlockSize : block->BlockSize;
        if (ComputeAllocationSize(size, alignment) <= blockMaxSize)
        {
            block->UserSize = size;
            return pointer;
        }

        void* newBlock = Allocate(size, alignment);
        Memory::Copy(newBlock, pointer, std::min(oldSize, size));
        Free(pointer);

        return newBlock;
    }

    void BinnedAllocator::Free(void* pointer)
    {
        if (!pointer)
            return;

        BlockHeader* blockHeader = UserToBlock(static_cast<Byte*>(pointer));
        // Free-sentinel invariant check: a live block always has UserSize > 0 on the binned
        // path (Allocate sets it to the user-requested size which is at least 1).
        // UserSize == 0 means the block was already returned to the free list — catching the
        // double-free here before the block is re-linked onto the free list a second time.
        assert(blockHeader->UserSize > 0 && "Double freeing same memory");

        FreeBlock(blockHeader);
    }

    bool BinnedAllocator::IsThreadSafe() const
    {
        return true;
    }

    bool BinnedAllocator::IsThreadLocal() const
    {
        return true;
    }

    std::size_t BinnedAllocator::ComputeAllocationSize(std::size_t allocationSize, std::size_t alignment)
    {
        // Reserve at least sizeof(size_t)-1 bytes of padding so the offset word written
        // by BlockToUser always fits before the aligned user pointer, even for alignments
        // smaller than sizeof(size_t).
        const std::size_t paddingReserve = std::max(alignment, sizeof(std::size_t)) - 1;
        return sizeof(BlockHeader) + paddingReserve + sizeof(std::size_t) + allocationSize;
    }

    void* BinnedAllocator::BlockToUser(BlockHeader* block, std::size_t alignment)
    {
        Byte* base = reinterpret_cast<Byte*>(block) + sizeof(BlockHeader);
        Byte* aligned = AlignForward(base + sizeof(std::size_t), alignment);

        std::size_t paddingSize = aligned - base;
        assert(paddingSize >= sizeof(std::size_t));

        std::size_t& offset = *reinterpret_cast<std::size_t*>(aligned - sizeof(std::size_t));
        offset = paddingSize + sizeof(BlockHeader);

        return aligned;
    }

    BinnedAllocator::BlockHeader* BinnedAllocator::UserToBlock(Byte* userPointer)
    {
        std::size_t offset = *reinterpret_cast<std::size_t*>(userPointer - sizeof(std::size_t));
        return reinterpret_cast<BlockHeader*>(userPointer - offset);
    }

    BinnedAllocator::PageIndex* BinnedAllocator::FindPageIndex(std::size_t allocationSize)
    {
        for (PageIndex& pageIndex : mPageIndexes)
        {
            if (allocationSize <= pageIndex.BlockSize)
                return &pageIndex;
        }

        return nullptr;
    }

    void BinnedAllocator::AllocatePage(PageIndex* index)
    {
        PageHeader* pageHeader;

        // Reuse the reserved empty page if one is available, avoiding an OS round-trip
        if (index->ReservedPage)
        {
            pageHeader = index->ReservedPage;
            index->ReservedPage = nullptr;

            // The reserved page retains its full free list and uninitialized-block
            // state from when it was parked, so there is nothing to re-initialize
            // here — FreeBlockCount, FirstFreeBlock, FirstUninitializedBlock, and
            // UninitializedBlockCount are all still valid. We only need to clear the
            // link fields before splicing it back into the active page list.
            pageHeader->Previous = nullptr;
            pageHeader->Next = nullptr;
        }
        else
        {
            const std::size_t slabSize = index->SlabPageCount * SystemMemory::GetPageSize();
            Byte* page = static_cast<Byte*>(SystemMemory::Allocate(slabSize));
            assert(page && "malloc failed in AllocatePage");

            pageHeader = reinterpret_cast<PageHeader*>(page);
            pageHeader->Previous = nullptr;
            pageHeader->Next = nullptr;
            pageHeader->Index = index;
            pageHeader->FreeBlockCount = index->BlockCount;
            pageHeader->FirstFreeBlock = nullptr;

            // Bump pointer starts at the first block slot; Allocate advances it one
            // slot at a time as blocks are handed out for the first time.
            pageHeader->FirstUninitializedBlock = reinterpret_cast<BlockHeader*>(page + sizeof(PageHeader));
            pageHeader->UninitializedBlockCount = index->BlockCount;

            pageHeader->OwnerAllocator = this;
        }

        // Link page into page list for this index
        pageHeader->Next = index->FirstPage;
        if (index->FirstPage)
            index->FirstPage->Previous = pageHeader;

        index->FirstPage = pageHeader;

        // Since we just added a page with free blocks, it is available
        index->FirstAvailablePage = pageHeader;
    }

    void BinnedAllocator::FreeBlock(BlockHeader* block) const
    {
        if (PageHeader* page = block->Page)
        {
            if (page->OwnerAllocator == this)
            {
                PageIndex* index = page->Index;

                // Push the block onto the page's free list
                block->NextFree = page->FirstFreeBlock;
                page->FirstFreeBlock = block;
                assert(page->FirstFreeBlock != nullptr);

                // Free-sentinel invariant: reset UserSize to 0 to mark this block as free.
                // This must happen AFTER the block is pushed onto the free list and BEFORE
                // the page's FreeBlockCount is incremented, so that any concurrent or
                // re-entrant observer always sees a consistent state.
                // UserSize == 0 is the signal read by Free (double-free detection) and by
                // Allocate (double-alloc detection). Do not remove or defer this write.
                block->UserSize = 0;

                ++page->FreeBlockCount;

                // If the page became completely free, either keep it as a reserve or release it
                if (page->FreeBlockCount == index->BlockCount)
                {
                    PageHeader* previous = page->Previous;
                    PageHeader* next = page->Next;

                    if (previous)
                    {
                        previous->Next = next;
                        if (next)
                            next->Previous = previous;
                    }
                    else
                    {
                        index->FirstPage = next;
                        if (index->FirstPage)
                            index->FirstPage->Previous = nullptr;
                    }

                    if (page == page->Index->FirstAvailablePage)
                    {
                        PageHeader* availablePage = page->Index->FirstPage;
                        while (availablePage && availablePage->FreeBlockCount == 0)
                            availablePage = availablePage->Next;

                        index->FirstAvailablePage = availablePage;
                    }

                    if (!index->ReservedPage)
                    {
                        // No reserve held yet — unlink this page from the active list and
                        // park it as the reserve instead of returning it to the OS immediately.
                        // This prevents constant OS alloc/free thrashing under alternating
                        // alloc/free workloads. AllocatePage will reclaim it when needed.
                        index->ReservedPage = page;
                    }
                    else
                    {
                        std::size_t slabSize = index->SlabPageCount * SystemMemory::GetPageSize();
                        SystemMemory::Free(page, slabSize);
                    }
                }
                else
                {
                    if (!index->FirstAvailablePage || page->FreeBlockCount > index->FirstAvailablePage->FreeBlockCount)
                        index->FirstAvailablePage = page;

                    assert(page->FreeBlockCount > 0 && page->FirstFreeBlock != nullptr);
                }
            }
            else
            {
                // Cross-thread free: this block belongs to a different thread's allocator.
                // Push it onto the owner's lock-free remote free queue for deferred processing.
                page->OwnerAllocator->PushRemoteFree(block);
            }
        }
        else
        {
            SystemMemory::Free(block, block->BlockSize);
        }
    }

    void BinnedAllocator::PushRemoteFree(BlockHeader* block)
    {
        // Treiber stack push: CAS loop to prepend block to the remote free list.
        // Multiple foreign threads may push concurrently; the owning thread drains.
        BlockHeader* head = mRemoteFreeHead.load(std::memory_order_relaxed);
        do
        {
            block->NextFree = head;
        } while (!mRemoteFreeHead.compare_exchange_weak(head, block, std::memory_order_release, std::memory_order_relaxed));
    }

    void BinnedAllocator::DrainRemoteFrees()
    {
        // Atomic exchange claims the entire remote free list in one operation.
        // After the exchange, no other thread can see these blocks on our queue.
        BlockHeader* head = mRemoteFreeHead.exchange(nullptr, std::memory_order_acquire);

        // Walk the claimed list and free each block locally (we are the owner).
        while (head)
        {
            BlockHeader* next = head->NextFree;
            FreeBlock(head);
            head = next;
        }
    }
}
