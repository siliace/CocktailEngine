#ifndef COCKTAIL_CORE_MEMORY_ALLOCATOR_BINNEDALLOCATOR_HPP
#define COCKTAIL_CORE_MEMORY_ALLOCATOR_BINNEDALLOCATOR_HPP

#include <algorithm>

#include <Cocktail/Core/Memory/Allocator/MemoryAllocator.hpp>

namespace Ck
{
    template <unsigned int BlockSizeCount, unsigned int BlockPerPage, unsigned int BlockBaseSize = 16u>
    class BinnedAllocator : public MemoryAllocator
    {
    public:

        BinnedAllocator()
        {
            for (Uint32 i = 0; i < BlockSizeCount; i++)
            {
                mPageIndexes[i].BlockSize = i == 0 ? BlockBaseSize : 2u * mPageIndexes[i - 1].BlockSize;
                mPageIndexes[i].FirstPage = nullptr;
                mPageIndexes[i].FirstAvailablePage = nullptr;
            }
        }

        ~BinnedAllocator() override
        {
            // Free all pages in every index
            for (unsigned int i = 0; i < BlockSizeCount; ++i)
            {
                PageHeader* current = mPageIndexes[i].FirstPage;
                while (current)
                {
                    PageHeader* next = current->Next;
                    std::free(current);

                    current = next;
                }
            }
        }

        void* Allocate(std::size_t size) override
        {
            BlockHeader* allocatedBlock = nullptr;

            // We don't want to support too small allocations
            // We don't want either to create dedicated allocations of such small amount of memory
            size = std::max(size, static_cast<std::size_t>(BlockBaseSize));

            if (PageIndex* index = FindPageIndex(static_cast<unsigned int>(size)))
            {
                if (!index->FirstAvailablePage)
                    AllocatePage(index);

                PageHeader* page = index->FirstAvailablePage;
                assert(page && page->FirstFreeBlock && "No available page or free block found");

                // Pop a block from the page's free list (O(1))
                allocatedBlock = page->FirstFreeBlock;
                page->FirstFreeBlock = allocatedBlock->NextFree;

                --page->FreeBlockCount;

                // Record requested size in the block header
                assert(allocatedBlock->Size == 0 && "Double allocating same memory");
                allocatedBlock->Size = size;

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
                std::size_t blockSize = sizeof(BlockHeader) + size;

                allocatedBlock = static_cast<BlockHeader*>(std::malloc(blockSize));
                assert(allocatedBlock && "malloc failed for large allocation");

                allocatedBlock->Page = nullptr;
                allocatedBlock->NextFree = nullptr;
                allocatedBlock->Size = size;
            }

            return BlockToUser(allocatedBlock);
        }

        void* Reallocate(void* pointer, std::size_t size) override
        {
            assert(pointer || size > 0);

            if (!pointer)
                return Allocate(size);

            if (size == 0)
            {
                Free(pointer);
                return nullptr;
            }

            BlockHeader* block = UserToBlock(static_cast<Byte*>(pointer));
            std::size_t blockMaxSize = block->Page ? block->Page->Index->BlockSize : block->Size;
            if (size <= blockMaxSize)
            {
                block->Size = size;
                return pointer;
            }

            void* newBlock = Allocate(size);
            Memory::Copy(newBlock, pointer, block->Size);
            Free(pointer);

            return newBlock;
        }

        void Free(void* pointer) override
        {
            if (!pointer)
                return;

            Byte* base = static_cast<Byte*>(pointer) - sizeof(BlockHeader);
            BlockHeader* blockHeader = reinterpret_cast<BlockHeader*>(base);
            assert(blockHeader->Size > 0 && "Double freeing same memory");

            if (blockHeader->Page != nullptr)
            {
                FreeBlock(blockHeader);
                return;
            }

            std::free(base);
        }

    private:

        struct PageHeader;
        struct PageIndex;

        struct BlockHeader
        {
            PageHeader* Page; // owning page (nullptr for special cases)
            BlockHeader* NextFree; // next free block in this page (used only while block is free)
            std::size_t Size; // user-requested size (valid while allocated)
        };

        struct PageHeader
        {
            PageHeader* Previous;
            PageHeader* Next;
            PageIndex* Index; // back-pointer to which index this page belongs to
            BlockHeader* FirstFreeBlock; // head of the free-list for this page
            unsigned int FreeBlockCount;
            unsigned int TotalBlockCount;
        };

        struct PageIndex
        {
            unsigned int BlockSize; /*!< The block payload size */
            PageHeader* FirstPage = nullptr; // list of all pages for this bin
            PageHeader* FirstAvailablePage = nullptr; // first page which has at least one free block
        };

        static Byte* BlockToUser(BlockHeader* block)
        {
            return reinterpret_cast<Byte*>(block) + sizeof(BlockHeader);
        }

        static BlockHeader* UserToBlock(Byte* user)
        {
            return reinterpret_cast<BlockHeader*>(user - sizeof(BlockHeader));
        }

        PageIndex* FindPageIndex(unsigned int allocationSize)
        {
            for (std::size_t i = 0; i < BlockSizeCount; ++i)
            {
                if (allocationSize <= mPageIndexes[i].BlockSize)
                    return &mPageIndexes[i];
            }

            return nullptr;
        }

        void AllocatePage(PageIndex* index)
        {
            // Each page contains a PageHeader + n blocks with each a BlockHeader and a payload of blockPayloadSize
            std::size_t blockSize = sizeof(BlockHeader) + index->BlockSize;
            std::size_t pageSize = sizeof(PageHeader) + BlockPerPage * blockSize;

            Byte* page = static_cast<Byte*>(std::malloc(pageSize));
            assert(page && "malloc failed in AllocatePage");

            // Fill page header
            PageHeader* pageHeader = reinterpret_cast<PageHeader*>(page);
            pageHeader->Previous = nullptr;
            pageHeader->Next = nullptr;
            pageHeader->Index = index;
            pageHeader->FreeBlockCount = BlockPerPage;
            pageHeader->TotalBlockCount = BlockPerPage;
            pageHeader->FirstFreeBlock = nullptr;

            // Move to the first block
            Byte* cursor = page + sizeof(PageHeader);

            // Build singly-linked free list between blocks (NextFree)
            BlockHeader* previous = nullptr;
            for (unsigned int i = 0; i < BlockPerPage; ++i)
            {
                BlockHeader* blockHeader = reinterpret_cast<BlockHeader*>(cursor);
                blockHeader->Page = pageHeader;
                blockHeader->NextFree = previous;

                // push front so final head points to last created => order doesn't matter
                blockHeader->Size = 0;

                previous = blockHeader;
                cursor += blockSize;
            }

            // prevFree now points to the last-created block; make it the head
            pageHeader->FirstFreeBlock = previous;

            // Link page into page list for this index
            pageHeader->Next = index->FirstPage;
            if (index->FirstPage)
                index->FirstPage->Previous = pageHeader;

            index->FirstPage = pageHeader;

            // Since we just added a page with free blocks, it is available
            assert(!index->FirstAvailablePage);
            index->FirstAvailablePage = pageHeader;
        }

        void FreeBlock(BlockHeader* block)
        {
            PageHeader* page = block->Page;
            assert(page && "FreeBlock called on block without page (unexpected)");

            // Push the block onto the page's free list
            block->NextFree = page->FirstFreeBlock;
            page->FirstFreeBlock = block;

            // Optionally reset Size to 0 to mark free (not relied upon for correctness)
            block->Size = 0;

            unsigned int previousFreeCount = page->FreeBlockCount;
            ++page->FreeBlockCount;

            PageIndex* index = page->Index;

            // If the page became completely free, free the page itself
            if (page->FreeBlockCount == page->TotalBlockCount)
            {
                FreePage(index, page);
                return;
            }

            if (previousFreeCount == 0 || !index->FirstAvailablePage)
                index->FirstAvailablePage = page;
        }

        void FreePage(PageIndex* index, PageHeader* page)
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

            if (page == index->FirstAvailablePage)
            {
                PageHeader* availablePage = index->FirstPage;
                while (availablePage && availablePage->FreeBlockCount == 0)
                    availablePage = availablePage->Next;

                index->FirstAvailablePage = availablePage;
            }

            std::free(page);
        }

        PageIndex mPageIndexes[BlockSizeCount];
    };
}

#endif // COCKTAIL_CORE_MEMORY_ALLOCATOR_BINNEDALLOCATOR_HPP
