#ifndef COCKTAIL_CORE_MEMORY_ALLOCATOR_BINNEDALLOCATOR_HPP
#define COCKTAIL_CORE_MEMORY_ALLOCATOR_BINNEDALLOCATOR_HPP

#include <algorithm>

#include <Cocktail/Core/Memory/Allocator/MemoryAllocator.hpp>
#include <Cocktail/Core/System/SystemMemory.hpp>

namespace Ck
{
    template <unsigned int BlockSizeCount>
    class BinnedAllocator : public MemoryAllocator
    {
    public:

        BinnedAllocator()
        {
            std::size_t baseBlockSize = NextPowerOfTwo(ComputeBlockSize(alignof(std::max_align_t), alignof(std::max_align_t)));

            for (Uint32 i = 0; i < BlockSizeCount; i++)
            {
                std::size_t blockSize = i == 0 ? baseBlockSize : 2u * mPageIndexes[i - 1].BlockSize;
                std::size_t availableSize = SystemMemory::GetPageSize() - sizeof(PageHeader);
                std::size_t blockCount = availableSize / blockSize;

                mPageIndexes[i].BlockCount = blockCount;
                mPageIndexes[i].BlockSize = blockSize;
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
                if (current)
                {
                    std::size_t pageSize = sizeof(PageHeader) + mPageIndexes[i].BlockCount * current->Index->BlockSize;
                    while (current)
                    {
                        PageHeader* next = current->Next;
                        SystemMemory::Free(current, pageSize);

                        current = next;
                    }
                }
            }
        }

        void* Allocate(std::size_t size, std::size_t alignment) override
        {
            BlockHeader* allocatedBlock;

            // We don't want to support too small allocations
            // We don't want either to create dedicated allocations of such small amount of memory
            std::size_t blockSize = std::max(ComputeBlockSize(size, alignment), alignof(std::max_align_t));
            if (PageIndex* index = FindPageIndex(static_cast<unsigned int>(blockSize)))
            {
                if (!index->FirstAvailablePage)
                    AllocatePage(index);

                PageHeader* page = index->FirstAvailablePage;
                assert(page && page->FirstFreeBlock && "No available page or free block found");

                // Pop a block from the page's free list (O(1))
                allocatedBlock = page->FirstFreeBlock;
                page->FirstFreeBlock = allocatedBlock->NextFree;
                --page->FreeBlockCount;

                assert(page->FreeBlockCount == 0 || page->FirstFreeBlock != nullptr);

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
                allocatedBlock = static_cast<BlockHeader*>(SystemMemory::Allocate(blockSize));
                assert(allocatedBlock && "malloc failed for large allocation");

                allocatedBlock->Page = nullptr;
                allocatedBlock->NextFree = nullptr;
                allocatedBlock->Size = blockSize;
            }

            return BlockToUser(allocatedBlock, alignment);
        }

        void* Reallocate(void* pointer, std::size_t size, std::size_t alignment) override
        {
            assert(pointer || size > 0);

            if (!pointer)
                return Allocate(size, alignment);

            if (size == 0)
            {
                Free(pointer);
                return nullptr;
            }

            BlockHeader* block = UserToBlock(static_cast<Byte*>(pointer));
            assert(block->Size > 0 && "Reallocating a freed block (Use-After-Free detected)");

            std::size_t blockMaxSize = block->Page ? block->Page->Index->BlockSize : block->Size;
            if (size <= blockMaxSize)
            {
                block->Size = size;
                return pointer;
            }

            void* newBlock = Allocate(size, alignment);
            Memory::Copy(newBlock, pointer, block->Size);
            Free(pointer);

            return newBlock;
        }

        void Free(void* pointer) override
        {
            if (!pointer)
                return;

            BlockHeader* blockHeader = UserToBlock(pointer);
            assert(blockHeader->Size > 0 && "Double freeing same memory");

            if (blockHeader->Page != nullptr)
            {
                FreeBlock(blockHeader);
                return;
            }

            SystemMemory::Free(blockHeader, blockHeader->Size);
        }

    private:

        struct PageHeader;
        struct PageIndex;

        struct alignas(std::max_align_t) BlockHeader
        {
            PageHeader* Page; /*!< owning page (nullptr for special cases) */
            BlockHeader* NextFree; /*!< next free block in this page (used only while block is free) */
            std::size_t Size; /*!< user-requested size (valid while allocated) */
        };

        struct alignas(std::max_align_t) PageHeader
        {
            PageHeader* Previous;
            PageHeader* Next;
            PageIndex* Index; /*!< back-pointer to which index this page belongs to */
            BlockHeader* FirstFreeBlock; /*!< head of the free-list for this page */
            std::size_t FreeBlockCount;
            std::size_t TotalBlockCount;
        };

        struct PageIndex
        {
            std::size_t BlockCount = 0; /*!< The number of block available per page */
            std::size_t BlockSize = 0; /*!< The block payload size */
            PageHeader* FirstPage = nullptr; /*!< list of all pages for this bin */
            PageHeader* FirstAvailablePage = nullptr; /*!< first page which has at least one free block */
        };

        static std::size_t ComputeBlockSize(std::size_t allocationSize, std::size_t alignment)
        {
            return sizeof(BlockHeader) + alignment - 1 + sizeof(std::size_t) + allocationSize ;
        }

        static void* BlockToUser(BlockHeader* block, std::size_t alignment)
        {
            Byte* base = reinterpret_cast<Byte*>(block) + sizeof(BlockHeader);
            Byte* aligned = AlignForward(base + sizeof(std::size_t), alignment);

            std::size_t paddingSize = aligned - base;
            assert(paddingSize >= sizeof(std::size_t));

            std::size_t& offset = *reinterpret_cast<std::size_t*>(aligned - sizeof(std::size_t));
            offset = paddingSize + sizeof(BlockHeader);

            return aligned;
        }

        static BlockHeader* UserToBlock(void* userPointer)
        {
            Byte* user = static_cast<Byte*>(userPointer);

            std::size_t offset = *reinterpret_cast<std::size_t*>(user - sizeof(std::size_t));

            return reinterpret_cast<BlockHeader*>(user - offset);
        }

        PageIndex* FindPageIndex(unsigned int blockSize)
        {
            for (std::size_t i = 0; i < BlockSizeCount; ++i)
            {
                if (blockSize <= mPageIndexes[i].BlockSize)
                    return &mPageIndexes[i];
            }

            return nullptr;
        }

        void AllocatePage(PageIndex* index)
        {
            // Each page contains a PageHeader + n blocks with each a BlockHeader and a payload of blockPayloadSize
            std::size_t pageSize = sizeof(PageHeader) + index->BlockCount * index->BlockSize;

            Byte* page = static_cast<Byte*>(SystemMemory::Allocate(pageSize));
            assert(page && "malloc failed in AllocatePage");

            // Fill page header
            PageHeader* pageHeader = reinterpret_cast<PageHeader*>(page);
            pageHeader->Previous = nullptr;
            pageHeader->Next = nullptr;
            pageHeader->Index = index;
            pageHeader->FreeBlockCount = index->BlockCount;
            pageHeader->TotalBlockCount = index->BlockCount;
            pageHeader->FirstFreeBlock = nullptr;

            // Move to the first block
            Byte* cursor = page + sizeof(PageHeader);

            // Build singly-linked free list between blocks (NextFree)
            BlockHeader* first = nullptr;
            BlockHeader* previous = nullptr;
            for (unsigned int i = 0; i < index->BlockCount; ++i)
            {
                BlockHeader* blockHeader = reinterpret_cast<BlockHeader*>(cursor);
                blockHeader->Page = pageHeader;
                blockHeader->NextFree = nullptr;
                blockHeader->Size = 0;

                if (i == 0)
                    first = blockHeader;

                if (previous)
                    previous->NextFree = blockHeader;

                previous = blockHeader;
                cursor += index->BlockSize;
            }

            assert(first);
            pageHeader->FirstFreeBlock = first;

            // Link page into page list for this index
            pageHeader->Next = index->FirstPage;
            if (index->FirstPage)
                index->FirstPage->Previous = pageHeader;

            index->FirstPage = pageHeader;

            // Since we just added a page with free blocks, it is available
            index->FirstAvailablePage = pageHeader;
        }

        void FreeBlock(BlockHeader* block)
        {
            PageHeader* page = block->Page;
            assert(page && "FreeBlock called on block without page (unexpected)");

            PageIndex* index = page->Index;

            // Push the block onto the page's free list
            block->NextFree = page->FirstFreeBlock;
            page->FirstFreeBlock = block;
            assert(page->FirstFreeBlock != nullptr);

            block->Size = 0;

            ++page->FreeBlockCount;

            // If the page became completely free, free the page itself
            if (page->FreeBlockCount == page->TotalBlockCount)
            {
                FreePage(index, page);
                return;
            }

            if (!index->FirstAvailablePage || page->FreeBlockCount > index->FirstAvailablePage->FreeBlockCount)
                index->FirstAvailablePage = page;

            assert(page->FreeBlockCount > 0 && page->FirstFreeBlock != nullptr);
        }

        void FreePage(PageIndex* index, PageHeader* page)
        {
            assert(page->FreeBlockCount == page->TotalBlockCount);
            std::size_t pageSize = sizeof(PageHeader) + index->BlockCount * index->BlockSize;

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

            SystemMemory::Free(page, pageSize);
        }

        PageIndex mPageIndexes[BlockSizeCount];
    };
}

#endif // COCKTAIL_CORE_MEMORY_ALLOCATOR_BINNEDALLOCATOR_HPP
