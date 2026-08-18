#include <catch2/catch_all.hpp>

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <iterator>
#include <thread>

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/Memory/Allocator/BinnedAllocator.hpp>
#include <CocktailEngine/Core/Memory/Memory.hpp>
#include <CocktailEngine/Core/Memory/UniquePtr.hpp>
#include <CocktailEngine/Core/System/SystemMemory.hpp>

using namespace Ck;

namespace
{
    /**
     * \brief Index and size type of every array in this file
     *
     * All of them are plain \c Array, so they all index with the size type of
     * \c HeapAllocator rather than with \c std::size_t, and the counts a test states
     * are declared in it instead of being cast at each use.
     */
    using Index = HeapAllocator::SizeType;

    /**
     * \brief Alignments every allocation path is exercised with
     *
     * Powers of two only, which is what \c AlignForward assumes. The largest ones
     * push \c ComputeAllocationSize past the biggest bin on their own, so they also
     * cover the case where the alignment rather than the size is what sends a
     * request to the OS.
     */
    constexpr std::size_t Alignments[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256 };

    /**
     * \brief Sizes the churn loops draw from
     *
     * Spread across every bin, on and around the boundaries where a request moves
     * from one size class to the next, plus a few past the largest bin so the
     * direct-to-OS fallback takes part in the same run.
     */
    constexpr std::size_t ChurnSizes[] = { 1, 4, 12, 24, 33, 48, 64, 80, 100, 150, 220, 300, 450, 700, 900, 1500, 3000 };

    /**
     * \brief Deterministic 32-bit xorshift
     *
     * Seeded explicitly rather than from a clock so a run that fails can be replayed
     * exactly as it happened, and so two threads of the same test never walk the same
     * sequence.
     */
    class Xorshift32
    {
    public:

        explicit Xorshift32(std::uint32_t seed) noexcept :
            mState(seed != 0 ? seed : 0x9E3779B9u)
        {
            /// Nothing
        }

        std::uint32_t Next() noexcept
        {
            std::uint32_t state = mState;
            state ^= state << 13;
            state ^= state >> 17;
            state ^= state << 5;
            mState = state;

            return state;
        }

        template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
        T NextBelow(T bound) noexcept
        {
            return Next() % bound;
        }

    private:

        std::uint32_t mState;
    };

    /**
     * \brief Byte a block seeded with \p seed is expected to hold at \p offset
     *
     * Depends on the offset as well as on the seed, so a block that ends up
     * overlapping another one fails the check wherever the overlap starts rather
     * than only when the two seeds happen to differ.
     */
    Byte PatternByte(std::uint32_t seed, std::size_t offset) noexcept
    {
        const std::uint32_t mixed = seed * 2654435761u + static_cast<std::uint32_t>(offset) * 2246822519u;
        return static_cast<Byte>((mixed >> 13) & 0xFFu);
    }

    void WritePattern(void* pointer, std::size_t size, std::uint32_t seed) noexcept
    {
        Byte* bytes = static_cast<Byte*>(pointer);
        for (std::size_t offset = 0; offset < size; ++offset)
            bytes[offset] = PatternByte(seed, offset);
    }

    bool VerifyPattern(const void* pointer, std::size_t size, std::uint32_t seed) noexcept
    {
        const Byte* bytes = static_cast<const Byte*>(pointer);
        for (std::size_t offset = 0; offset < size; ++offset)
        {
            if (bytes[offset] != PatternByte(seed, offset))
                return false;
        }

        return true;
    }

    bool IsAligned(const void* pointer, std::size_t alignment) noexcept
    {
        return (reinterpret_cast<std::uintptr_t>(pointer) % alignment) == 0;
    }

    /**
     * \brief What a test knows about a block it is holding
     *
     * The size and the alignment are kept because the allocator does not hand them
     * back, and the seed because verifying a block means knowing which pattern was
     * written into it.
     */
    struct Block
    {
        void* Pointer = nullptr;
        std::size_t Size = 0;
        std::size_t Alignment = 0;
        std::uint32_t Seed = 0;
    };

    /**
     * \brief What a worker thread reports back once it is done
     *
     * Catch2 assertion macros are not safe to call from several threads at once, so
     * a thread counts what went wrong and the main thread turns the counts into
     * assertions after joining.
     */
    struct ThreadReport
    {
        std::size_t Operations = 0;
        std::size_t NullFailures = 0;
        std::size_t AlignmentFailures = 0;
        std::size_t IntegrityFailures = 0;
    };

    /**
     * \brief Runs a mixed allocate / reallocate / free workload against an allocator
     *
     * The live set is kept around a target rather than allowed to drift, so the run
     * spends its time on the interesting states: pages that are partly free, free
     * lists that are being popped and pushed, and slabs that empty and come back.
     * Every block carries a pattern that is verified before it is released, which is
     * what turns an overlap or a corrupted header into a counted failure.
     *
     * Returns the failure counts rather than asserting, so the same loop can be used
     * from the main thread and from a worker thread.
     */
    ThreadReport Churn(MemoryAllocator& allocator, std::uint32_t seed, std::size_t operationCount, Index liveTarget)
    {
        ThreadReport report;

        Xorshift32 random(seed);
        std::uint32_t nextSeed = seed | 1u;

        Array<Block> live;
        live.Reserve(liveTarget);

        for (std::size_t operation = 0; operation < operationCount; ++operation)
        {
            const bool shouldAllocate = live.IsEmpty() || (live.GetSize() < liveTarget && random.NextBelow(100) < 55);
            if (shouldAllocate)
            {
                Block block;
                block.Size = ChurnSizes[random.NextBelow(std::size(ChurnSizes))];
                block.Alignment = Alignments[random.NextBelow(std::size(Alignments))];
                block.Seed = ++nextSeed;
                block.Pointer = allocator.Allocate(block.Size, block.Alignment);

                ++report.Operations;

                if (!block.Pointer)
                {
                    ++report.NullFailures;
                    continue;
                }

                if (!IsAligned(block.Pointer, block.Alignment))
                    ++report.AlignmentFailures;

                WritePattern(block.Pointer, block.Size, block.Seed);
                live.Add(block);

                continue;
            }

            const Index index = random.NextBelow(live.GetSize());
            Block& block = live[index];

            // One release out of four is turned into a resize, so the in-place path and
            // the allocate-copy-free path are part of the same churn instead of needing
            // a run of their own.
            if (random.NextBelow(4) == 0)
            {
                const std::size_t newSize = ChurnSizes[random.NextBelow(std::size(ChurnSizes))];
                void* resized = allocator.Reallocate(block.Pointer, newSize, block.Alignment);

                ++report.Operations;

                if (!resized)
                {
                    ++report.NullFailures;
                    live[index] = live.Last();
                    live.PopLast();
                    continue;
                }

                if (!IsAligned(resized, block.Alignment))
                    ++report.AlignmentFailures;

                // Reallocate only promises the smaller of the two sizes, so that is all
                // that is checked. Asking for more would be testing something the
                // interface does not claim.
                if (!VerifyPattern(resized, std::min(block.Size, newSize), block.Seed))
                    ++report.IntegrityFailures;

                block.Pointer = resized;
                block.Size = newSize;
                block.Seed = ++nextSeed;
                WritePattern(block.Pointer, block.Size, block.Seed);

                continue;
            }

            if (!VerifyPattern(block.Pointer, block.Size, block.Seed))
                ++report.IntegrityFailures;

            allocator.Free(block.Pointer);
            ++report.Operations;

            live[index] = live.Last();
            live.PopLast();
        }

        // Whatever survived the run has to have kept its contents through everything
        // that happened around it.
        for (const Block& block : live)
        {
            if (!VerifyPattern(block.Pointer, block.Size, block.Seed))
                ++report.IntegrityFailures;
        }

        for (const Block& block : live)
            allocator.Free(block.Pointer);

        return report;
    }

    /**
     * \brief Reusable rendez-vous point for the multithreaded cases
     *
     * Spins rather than blocks, which is what a test of a few hundred milliseconds
     * wants, and carries a generation counter so the same barrier can be waited on
     * once per round. The release / acquire pair on that counter is also what makes
     * a block written before the barrier readable by another thread after it.
     */
    class SpinBarrier
    {
    public:

        explicit SpinBarrier(unsigned int participants) noexcept :
            mParticipants(participants),
            mWaiting(0),
            mGeneration(0)
        {
            /// Nothing
        }

        void Wait() noexcept
        {
            const unsigned int generation = mGeneration.load(std::memory_order_acquire);

            if (mWaiting.fetch_add(1, std::memory_order_acq_rel) + 1 == mParticipants)
            {
                mWaiting.store(0, std::memory_order_relaxed);
                mGeneration.fetch_add(1, std::memory_order_release);
                return;
            }

            while (mGeneration.load(std::memory_order_acquire) == generation)
                std::this_thread::yield();
        }

    private:

        unsigned int mParticipants;
        std::atomic<unsigned int> mWaiting;
        std::atomic<unsigned int> mGeneration;
    };

    /**
     * \brief Number of workers the multithreaded cases run with
     *
     * At least four even on a single-core machine, since the point is to have the
     * threads interleave rather than to saturate the hardware, and capped so a
     * many-core build agent does not turn a test into a memory-bound benchmark.
     */
    unsigned int WorkerCount()
    {
        const unsigned int available = std::thread::hardware_concurrency();
        return std::max(4u, std::min(available != 0 ? available : 4u, 16u));
    }

    template <typename TBody>
    void RunOnThreads(unsigned int threadCount, TBody&& body)
    {
        Array<std::thread> threads;
        threads.Reserve(threadCount);

        for (unsigned int index = 0; index < threadCount; ++index)
            threads.Emplace([&body, index] { body(index); });

        for (std::thread& thread : threads)
            thread.join();
    }

    void CheckReport(const ThreadReport& report)
    {
        CHECK(report.Operations > 0);
        CHECK(report.NullFailures == 0);
        CHECK(report.AlignmentFailures == 0);
        CHECK(report.IntegrityFailures == 0);
    }

    /**
     * \brief Adapter routing the churn loop at the global allocator
     *
     * \c Ck::Memory is a static facade rather than a \c MemoryAllocator instance, so
     * it cannot be handed to \c Churn directly. Going through it is worth a few lines
     * of glue: it is the configuration a game actually runs, a
     * \c ThreadLocalAllocatorProxy over one \c BinnedAllocator per thread, which is
     * not something a test driving an allocator it owns can reach.
     */
    class GlobalAllocator final : public MemoryAllocator
    {
    public:

        void* Allocate(std::size_t size, std::size_t alignment) override
        {
            return Memory::Allocate(size, alignment);
        }

        void* Reallocate(void* pointer, std::size_t size, std::size_t alignment) override
        {
            return Memory::Reallocate(pointer, size, alignment);
        }

        void Free(void* pointer) override
        {
            Memory::Free(pointer);
        }

        bool IsThreadSafe() const override
        {
            return true;
        }

        bool IsThreadLocal() const override
        {
            return false;
        }
    };
}

TEST_CASE("BinnedAllocator serves every size class", "[BinnedAllocator]")
{
    BinnedAllocator allocator;

    // One size inside each bin and one on either side of the boundaries between them,
    // then a few past the largest bin. Which of the two paths a given pair ends up on
    // is the allocator's business; what is checked is that both behave the same from
    // the outside.
    constexpr std::size_t Sizes[] = { 0,   1,   7,    8,     31,          63,   64,  100,
                                      103, 104, 200,  231,   232,         450,  487, 488,
                                      900, 999, 1024, 4096,  64 * 1024,   1024 * 1024 };

    for (std::size_t size : Sizes)
    {
        for (std::size_t alignment : Alignments)
        {
            void* pointer = allocator.Allocate(size, alignment);

            REQUIRE(pointer != nullptr);
            REQUIRE(IsAligned(pointer, alignment));

            // A zero-sized request is clamped to one byte rather than refused, so the
            // block it returns is still a block and has to be writable.
            const std::size_t writable = std::max<std::size_t>(size, 1);

            WritePattern(pointer, writable, 0xA5A5u);
            REQUIRE(VerifyPattern(pointer, writable, 0xA5A5u));

            allocator.Free(pointer);
        }
    }
}

TEST_CASE("BinnedAllocator hands out disjoint blocks", "[BinnedAllocator]")
{
    // Far more blocks than a single slab holds for any bin, so carving pages, walking
    // the page list and advancing the lazy initialisation bump pointer all take part.
    constexpr Index BlockCount = 512;
    constexpr std::size_t Sizes[] = { 8, 40, 100, 200, 450, 900 };

    for (std::size_t size : Sizes)
    {
        BinnedAllocator allocator;

        Array<Block> blocks;
        blocks.Reserve(BlockCount);

        for (Index index = 0; index < BlockCount; ++index)
        {
            Block block;
            block.Size = size;
            block.Alignment = 16;
            block.Seed = index + 1;
            block.Pointer = allocator.Allocate(block.Size, block.Alignment);

            REQUIRE(block.Pointer != nullptr);
            REQUIRE(IsAligned(block.Pointer, block.Alignment));

            WritePattern(block.Pointer, block.Size, block.Seed);
            blocks.Add(block);
        }

        // Every pattern still standing after the last allocation is what says no two
        // blocks were handed the same bytes: an overlap would have had the later write
        // destroy the earlier one.
        std::size_t corrupted = 0;
        for (const Block& block : blocks)
        {
            if (!VerifyPattern(block.Pointer, block.Size, block.Seed))
                ++corrupted;
        }
        CHECK(corrupted == 0);

        // And the same thing said directly, so a failure points at the geometry rather
        // than at a byte: sorted by address, no block may reach into the next one.
        Block* sorted = blocks.GetData();
        std::sort(sorted, sorted + blocks.GetSize(), [](const Block& left, const Block& right) {
            return reinterpret_cast<std::uintptr_t>(left.Pointer) < reinterpret_cast<std::uintptr_t>(right.Pointer);
        });

        std::size_t overlaps = 0;
        for (Index index = 1; index < blocks.GetSize(); ++index)
        {
            const std::uintptr_t previousEnd = reinterpret_cast<std::uintptr_t>(blocks[index - 1].Pointer) + blocks[index - 1].Size;
            if (previousEnd > reinterpret_cast<std::uintptr_t>(blocks[index].Pointer))
                ++overlaps;
        }
        CHECK(overlaps == 0);

        for (const Block& block : blocks)
            allocator.Free(block.Pointer);
    }
}

TEST_CASE("BinnedAllocator preserves contents across a resize", "[BinnedAllocator]")
{
    BinnedAllocator allocator;

    SECTION("A resize that still fits the slot keeps the block where it is")
    {
        void* pointer = allocator.Allocate(40, 16);
        REQUIRE(pointer != nullptr);
        WritePattern(pointer, 40, 0x101u);

        void* resized = allocator.Reallocate(pointer, 60, 16);

        REQUIRE(resized == pointer);
        REQUIRE(VerifyPattern(resized, 40, 0x101u));

        allocator.Free(resized);
    }

    SECTION("A resize into a larger bin carries the contents over")
    {
        void* pointer = allocator.Allocate(40, 16);
        REQUIRE(pointer != nullptr);
        WritePattern(pointer, 40, 0x202u);

        void* resized = allocator.Reallocate(pointer, 900, 16);

        REQUIRE(resized != nullptr);
        REQUIRE(IsAligned(resized, 16));
        REQUIRE(VerifyPattern(resized, 40, 0x202u));

        allocator.Free(resized);
    }

    SECTION("A resize past the largest bin carries the contents over")
    {
        void* pointer = allocator.Allocate(900, 16);
        REQUIRE(pointer != nullptr);
        WritePattern(pointer, 900, 0x303u);

        void* resized = allocator.Reallocate(pointer, 64 * 1024, 16);

        REQUIRE(resized != nullptr);
        REQUIRE(IsAligned(resized, 16));
        REQUIRE(VerifyPattern(resized, 900, 0x303u));

        // And once more entirely on the OS path, which stores its capacity in the block
        // header rather than reading it back from a bin.
        void* grown = allocator.Reallocate(resized, 256 * 1024, 16);

        REQUIRE(grown != nullptr);
        REQUIRE(VerifyPattern(grown, 900, 0x303u));

        allocator.Free(grown);
    }

    SECTION("A resize down keeps the bytes it still covers")
    {
        void* pointer = allocator.Allocate(900, 16);
        REQUIRE(pointer != nullptr);
        WritePattern(pointer, 900, 0x404u);

        void* shrunk = allocator.Reallocate(pointer, 16, 16);

        REQUIRE(shrunk != nullptr);
        REQUIRE(VerifyPattern(shrunk, 16, 0x404u));

        // Growing back only restores capacity, not the bytes the shrink dropped, so the
        // freshly reachable range is written before it is read.
        void* grown = allocator.Reallocate(shrunk, 800, 16);

        REQUIRE(grown != nullptr);
        REQUIRE(VerifyPattern(grown, 16, 0x404u));

        WritePattern(grown, 800, 0x505u);
        REQUIRE(VerifyPattern(grown, 800, 0x505u));

        allocator.Free(grown);
    }

    SECTION("A resize stands in for an allocation and for a release")
    {
        void* allocated = allocator.Reallocate(nullptr, 128, 16);

        REQUIRE(allocated != nullptr);
        REQUIRE(IsAligned(allocated, 16));

        WritePattern(allocated, 128, 0x606u);
        REQUIRE(VerifyPattern(allocated, 128, 0x606u));

        REQUIRE(allocator.Reallocate(allocated, 0, 16) == nullptr);
    }

    SECTION("Releasing nothing is not an error")
    {
        allocator.Free(nullptr);
        SUCCEED();
    }
}

TEST_CASE("BinnedAllocator recycles a slab instead of returning it to the OS", "[BinnedAllocator]")
{
    BinnedAllocator allocator;

    // Deliberately few enough blocks to sit inside a single slab of the smallest bin,
    // whatever the page size is: a cycle then empties that one slab completely, which
    // is the state the bin's reserve exists for.
    constexpr Index BlocksPerCycle = 32;
    constexpr Index CycleCount = 256;

    Array<void*> live;
    live.Reserve(BlocksPerCycle);

    Array<std::uintptr_t> seen;
    seen.Reserve(BlocksPerCycle * CycleCount);

    for (Index cycle = 0; cycle < CycleCount; ++cycle)
    {
        for (Index index = 0; index < BlocksPerCycle; ++index)
        {
            void* pointer = allocator.Allocate(24, 8);

            REQUIRE(pointer != nullptr);

            live.Add(pointer);
            seen.Add(reinterpret_cast<std::uintptr_t>(pointer));
        }

        for (void* pointer : live)
            allocator.Free(pointer);

        live.Clear();
    }

    std::uintptr_t* addresses = seen.GetData();
    std::sort(addresses, addresses + seen.GetSize());
    const auto distinct = static_cast<Index>(std::unique(addresses, addresses + seen.GetSize()) - addresses);

    // Each cycle gives everything back, so the bin holds the emptied slab as its
    // reserve and the next cycle is served from the very same slots. Handing out any
    // address beyond the first cycle's would mean a slab went back to the OS and
    // another was asked for right after.
    CHECK(distinct == BlocksPerCycle);
}

TEST_CASE("BinnedAllocator survives a randomised churn", "[BinnedAllocator][stress]")
{
    BinnedAllocator allocator;

    const ThreadReport report = Churn(allocator, 0x1234ABCDu, 50000, 2048);

    CheckReport(report);
}

TEST_CASE("BinnedAllocator instances stay independent across threads", "[BinnedAllocator][stress][multithread]")
{
    const unsigned int threadCount = WorkerCount();

    // One allocator per thread, which is how BinnedAllocator is meant to be reached:
    // it reports itself thread-local, so the engine puts a ThreadLocalAllocatorProxy
    // in front of it rather than a lock. Owned here so that every allocator outlives
    // every thread, since a thread that exits leaves nothing behind to clean up.
    Array<UniquePtr<BinnedAllocator>> allocators;
    allocators.Reserve(threadCount);
    for (unsigned int index = 0; index < threadCount; ++index)
        allocators.Add(MakeUnique<BinnedAllocator>());

    Array<ThreadReport> reports(threadCount);

    RunOnThreads(threadCount, [&](unsigned int index) {
        reports[index] = Churn(*allocators[index], 0x51ED0000u + index, 20000, 1024);
    });

    for (const ThreadReport& report : reports)
        CheckReport(report);
}

TEST_CASE("BinnedAllocator reclaims blocks released by another thread", "[BinnedAllocator][stress][multithread]")
{
    const unsigned int threadCount = WorkerCount();

    constexpr Index BlocksPerRound = 1024;
    constexpr Index RoundCount = 8;

    Array<UniquePtr<BinnedAllocator>> allocators;
    allocators.Reserve(threadCount);
    for (unsigned int index = 0; index < threadCount; ++index)
        allocators.Add(MakeUnique<BinnedAllocator>());

    // What each thread allocated and is handing over. Written by its owner before the
    // barrier and read by its neighbour after it, which is what the barrier's
    // release / acquire pair is there for.
    Array<Array<Block>> published(threadCount);
    Array<ThreadReport> reports(threadCount);

    SpinBarrier barrier(threadCount);

    RunOnThreads(threadCount, [&](unsigned int index) {
        BinnedAllocator& owner = *allocators[index];
        const unsigned int neighbour = (index + 1) % threadCount;

        Xorshift32 random(0xC0FFEE00u + index);
        ThreadReport report;
        std::uint32_t nextSeed = 0x8000u * (index + 1);

        for (Index round = 0; round < RoundCount; ++round)
        {
            Array<Block>& mine = published[index];
            mine.Clear();
            mine.Reserve(BlocksPerRound);

            for (Index position = 0; position < BlocksPerRound; ++position)
            {
                Block block;
                block.Size = ChurnSizes[random.NextBelow(std::size(ChurnSizes))];
                block.Alignment = Alignments[random.NextBelow(std::size(Alignments))];
                block.Seed = ++nextSeed;
                block.Pointer = owner.Allocate(block.Size, block.Alignment);

                ++report.Operations;

                if (!block.Pointer)
                {
                    ++report.NullFailures;
                    continue;
                }

                if (!IsAligned(block.Pointer, block.Alignment))
                    ++report.AlignmentFailures;

                WritePattern(block.Pointer, block.Size, block.Seed);
                mine.Add(block);
            }

            barrier.Wait();

            // Each thread releases its neighbour's blocks through its own allocator,
            // which is the shape a cross-thread free takes in production: the block
            // belongs to a page this allocator does not own, so it goes onto the
            // owner's lock-free queue instead of onto a free list this thread has no
            // right to touch.
            for (const Block& block : published[neighbour])
            {
                if (!VerifyPattern(block.Pointer, block.Size, block.Seed))
                    ++report.IntegrityFailures;

                owner.Free(block.Pointer);
                ++report.Operations;

                // Keep this allocator allocating while foreign blocks pile up on its
                // own queue, so that draining and pushing really do overlap rather than
                // taking turns.
                if ((report.Operations & 0x3Fu) == 0)
                {
                    void* scratch = owner.Allocate(64, 16);
                    if (!scratch)
                        ++report.NullFailures;
                    else
                        owner.Free(scratch);
                }
            }

            barrier.Wait();
        }

        reports[index] = report;
    });

    for (const ThreadReport& report : reports)
        CheckReport(report);

    // The threads are gone, so each allocator is the main thread's alone to use. A
    // queue is only drained from an allocation, and what a drain puts back has to be
    // usable again: if a remotely freed block came back onto its page's free list
    // wrongly, this is where the list hands out something it should not.
    std::size_t corrupted = 0;
    for (UniquePtr<BinnedAllocator>& allocator : allocators)
    {
        Array<Block> blocks;
        blocks.Reserve(BlocksPerRound);

        for (Index position = 0; position < BlocksPerRound; ++position)
        {
            Block block;
            block.Size = ChurnSizes[position % std::size(ChurnSizes)];
            block.Alignment = 16;
            block.Seed = position + 1;
            block.Pointer = allocator->Allocate(block.Size, block.Alignment);

            REQUIRE(block.Pointer != nullptr);

            WritePattern(block.Pointer, block.Size, block.Seed);
            blocks.Add(block);
        }

        for (const Block& block : blocks)
        {
            if (!VerifyPattern(block.Pointer, block.Size, block.Seed))
                ++corrupted;
        }

        for (const Block& block : blocks)
            allocator->Free(block.Pointer);
    }

    CHECK(corrupted == 0);
}

TEST_CASE("The global allocator holds up under concurrent load", "[BinnedAllocator][stress][multithread]")
{
    const unsigned int threadCount = WorkerCount();

    GlobalAllocator global;

    SECTION("Threads allocating and releasing their own blocks")
    {
        Array<ThreadReport> reports(threadCount);

        RunOnThreads(threadCount, [&](unsigned int index) {
            reports[index] = Churn(global, 0x9E3700u + index, 20000, 1024);
        });

        for (const ThreadReport& report : reports)
            CheckReport(report);
    }

    SECTION("Threads releasing blocks another thread allocated")
    {
        constexpr Index BlocksPerRound = 1024;
        constexpr Index RoundCount = 8;

        Array<Array<Block>> published(threadCount);
        Array<ThreadReport> reports(threadCount);

        SpinBarrier barrier(threadCount);

        RunOnThreads(threadCount, [&](unsigned int index) {
            const unsigned int neighbour = (index + 1) % threadCount;

            Xorshift32 random(0xBEEF0000u + index);
            ThreadReport report;
            std::uint32_t nextSeed = 0x8000u * (index + 1);

            for (Index round = 0; round < RoundCount; ++round)
            {
                Array<Block>& mine = published[index];
                mine.Clear();
                mine.Reserve(BlocksPerRound);

                for (Index position = 0; position < BlocksPerRound; ++position)
                {
                    Block block;
                    block.Size = ChurnSizes[random.NextBelow(std::size(ChurnSizes))];
                    block.Alignment = Alignments[random.NextBelow(std::size(Alignments))];
                    block.Seed = ++nextSeed;
                    block.Pointer = Memory::Allocate(block.Size, block.Alignment);

                    ++report.Operations;

                    if (!block.Pointer)
                    {
                        ++report.NullFailures;
                        continue;
                    }

                    if (!IsAligned(block.Pointer, block.Alignment))
                        ++report.AlignmentFailures;

                    WritePattern(block.Pointer, block.Size, block.Seed);
                    mine.Add(block);
                }

                barrier.Wait();

                for (const Block& block : published[neighbour])
                {
                    if (!VerifyPattern(block.Pointer, block.Size, block.Seed))
                        ++report.IntegrityFailures;

                    Memory::Free(block.Pointer);
                    ++report.Operations;
                }

                barrier.Wait();
            }

            reports[index] = report;
        });

        for (const ThreadReport& report : reports)
            CheckReport(report);
    }
}
