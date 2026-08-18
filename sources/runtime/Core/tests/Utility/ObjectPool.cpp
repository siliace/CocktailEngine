#include <catch2/catch_all.hpp>

#include <algorithm>
#include <atomic>

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/Exception.hpp>
#include <CocktailEngine/Core/Memory/Ptr.hpp>
#include <CocktailEngine/Core/Memory/SharedFromThis.hpp>
#include <CocktailEngine/Core/System/Concurrency/Runnable.hpp>
#include <CocktailEngine/Core/System/Concurrency/Thread.hpp>
#include <CocktailEngine/Core/Utility/ObjectPool.hpp>

using namespace Ck;

namespace
{
    /**
     * \brief Index and size type of every array in this file
     *
     * All of them are plain \c Array, so they index with the size type of
     * \c HeapAllocator rather than with \c std::size_t, and the counts a test states
     * are declared in it instead of being cast at each use.
     */
    using Index = HeapAllocator::SizeType;

    /**
     * \brief Number of objects the first page of a pool holds
     *
     * The pool grows its n-th page to 32 * n slots, so this is what a fresh pool hands
     * out before it has to allocate again. The growth cases are written in terms of it
     * rather than of a bare 32, so that they keep testing what they mean if the policy
     * is ever retuned.
     */
    constexpr std::size_t FirstPageCapacity = 32;

    /**
     * \brief Type counting its own construction and destruction
     *
     * Counted atomically because the concurrency cases share one instance of the
     * counters across workers. The value it carries is what tells two objects apart
     * when a test needs to prove that two slots do not overlap.
     */
    class Tracked
    {
    public:

        static inline std::atomic<int> Constructed{ 0 };
        static inline std::atomic<int> Destroyed{ 0 };

        static void Reset()
        {
            Constructed.store(0);
            Destroyed.store(0);
        }

        /**
         * \brief Number of objects constructed and not yet destroyed
         */
        static int Live()
        {
            return Constructed.load() - Destroyed.load();
        }

        explicit Tracked(int value) :
            mValue(value)
        {
            Constructed.fetch_add(1, std::memory_order_relaxed);
        }

        /**
         * \brief Two argument constructor, to cover forwarding of more than one argument
         */
        Tracked(int first, int second) :
            mValue(first + second)
        {
            Constructed.fetch_add(1, std::memory_order_relaxed);
        }

        ~Tracked()
        {
            Destroyed.fetch_add(1, std::memory_order_relaxed);
        }

        Tracked(const Tracked&) = delete;
        Tracked& operator=(const Tracked&) = delete;

        int GetValue() const
        {
            return mValue;
        }

    private:

        int mValue;
    };

    /**
     * \brief Type whose constructor throws on demand
     *
     * Wider than a pointer, so that the slot it was given back is a slot a later
     * allocation can be seen to reuse rather than something that happens to land at
     * the same address for another reason.
     */
    class Detonator
    {
    public:

        static inline int Constructed = 0;
        static inline int Destroyed = 0;

        static void Reset()
        {
            Constructed = 0;
            Destroyed = 0;
        }

        explicit Detonator(bool explode)
        {
            if (explode)
                throw RuntimeException();

            ++Constructed;
        }

        ~Detonator()
        {
            ++Destroyed;
        }

    private:

        int mPadding[4] = {};
    };

    /**
     * \brief Type asking for more alignment than the allocator gives by default
     *
     * The pool passes \c alignof(T) to \c Memory::Allocate and then strides the page
     * by \c sizeof(T), so an over-aligned type is what shows both the page base and
     * the stride to be right.
     */
    struct alignas(64) OverAligned
    {
        explicit OverAligned(int value) :
            Value(value)
        {
        }

        int Value;
    };

    /**
     * \brief Smallest type a pool can be instantiated on
     *
     * One byte of size and of alignment, the opposite end of \c OverAligned: nothing
     * rounds the stride up, so two neighbouring slots are as close as they can get.
     */
    struct Tiny
    {
        explicit Tiny(char value) :
            Value(value)
        {
        }

        char Value;
    };

    /**
     * \brief Pooled type able to hand out a SharedPtr to itself
     *
     * A pooled object is destroyed and reconstructed in place, so its weak self
     * reference has to be re-established on every reuse rather than once for the
     * lifetime of the storage.
     */
    class Shareable : public SharedFromThis<Shareable>
    {
    public:

        explicit Shareable(int value) :
            mValue(value)
        {
        }

        int GetValue() const
        {
            return mValue;
        }

    private:

        int mValue;
    };

    /**
     * \brief Type that allocates from, and recycles into, the pool constructing it
     *
     * The pool runs both the constructor and the destructor of a pooled type while
     * holding its lock, so this type only works on a pool whose lock is recursive.
     * That is \c ObjectPool, and it is the reason the alias uses a recursive mutex:
     * the Vulkan device memory allocator relies on exactly this, a chunk allocating
     * its blocks from a pool it holds while it is itself being pooled.
     *
     * \warning Do not point this type at a \c SpinObjectPool. \c SpinMutex is not
     *          recursive and the nested allocation deadlocks.
     */
    class Reentrant
    {
    public:

        static inline int Constructed = 0;
        static inline int Destroyed = 0;

        static void Reset()
        {
            Constructed = 0;
            Destroyed = 0;
        }

        Reentrant(ObjectPool<Reentrant>& pool, int depth) :
            mDepth(depth)
        {
            ++Constructed;

            if (depth > 0)
            {
                // Allocated and released before this constructor returns, so the nested
                // Allocate and the nested Recycle both run under a lock this very call
                // already holds.
                auto child = pool.Allocate(pool, depth - 1);
                mChildDepth = child->GetDepth();
            }
        }

        ~Reentrant()
        {
            ++Destroyed;
        }

        int GetDepth() const
        {
            return mDepth;
        }

        int GetChildDepth() const
        {
            return mChildDepth;
        }

    private:

        int mDepth;
        int mChildDepth = -1;
    };

    /**
     * \brief Number of workers the multithreaded cases run with
     *
     * At least four even on a single-core machine, since the point is to have the
     * threads interleave rather than to saturate the hardware, and capped so a
     * many-core build agent does not turn a test into a benchmark.
     */
    unsigned int WorkerCount()
    {
        const unsigned int available = Thread::GetCoreCount();
        return std::max(4u, std::min(available != 0 ? available : 4u, 16u));
    }

    template <typename TBody>
    void RunOnThreads(unsigned int threadCount, TBody&& body)
    {
        Array<UniquePtr<Runnable>> runnables;
        runnables.Reserve(threadCount);

        Array<UniquePtr<Thread>> threads;
        threads.Reserve(threadCount);

        for (unsigned int index = 0; index < threadCount; ++index)
        {
            String threadName = String::Format(CK_TEXT("test-worker-%d"), index);
            UniquePtr<Runnable> runnable = MakeRunnable([&body](unsigned int index) {
                body(index);
            }, index);

            UniquePtr<Thread> thread = Thread::Create(runnable.Get(), threadName);
            threads.Emplace(Move(thread));
            runnables.Add(Move(runnable));
        }

        for (Ptr<Thread> thread : threads)
            thread->Join();
    }

    /**
     * \brief Reports whether \p addresses holds no value twice
     *
     * Sorts in place, which the callers do not mind: they collect the addresses for
     * this check and for nothing else.
     */
    bool AllDistinct(Array<void*>& addresses)
    {
        void* const* begin = addresses.GetData();
        void* const* end = begin + addresses.GetSize();

        std::sort(addresses.GetData(), addresses.GetData() + addresses.GetSize());

        return std::adjacent_find(begin, end) == end;
    }
}

TEST_CASE("Allocating from a pool constructs an object", "[ObjectPool]")
{
    Tracked::Reset();

    ObjectPool<Tracked> pool;

    SECTION("Unique ownership")
    {
        ObjectPoolUniquePtr<Tracked> object = pool.Allocate(42);

        REQUIRE(object);
        REQUIRE(object->GetValue() == 42);
        REQUIRE(Tracked::Live() == 1);
    }
    
    SECTION("Several constructor arguments are forwarded")
    {
        ObjectPoolUniquePtr<Tracked> object = pool.Allocate(40, 2);

        REQUIRE(object->GetValue() == 42);
    }

    SECTION("Two live objects are two distinct objects")
    {
        ObjectPoolUniquePtr<Tracked> first = pool.Allocate(1);
        ObjectPoolUniquePtr<Tracked> second = pool.Allocate(2);

        REQUIRE(first.Get() != second.Get());
        REQUIRE(first->GetValue() == 1);
        REQUIRE(second->GetValue() == 2);
        REQUIRE(Tracked::Live() == 2);
    }
}

TEST_CASE("Releasing a pooled object destroys it", "[ObjectPool]")
{
    Tracked::Reset();

    ObjectPool<Tracked> pool;

    SECTION("Going out of scope")
    {
        {
            ObjectPoolUniquePtr<Tracked> object = pool.Allocate(1);
            REQUIRE(Tracked::Live() == 1);
        }

        REQUIRE(Tracked::Live() == 0);
    }

    SECTION("Being reset")
    {
        ObjectPoolUniquePtr<Tracked> object = pool.Allocate(1);
        REQUIRE(Tracked::Live() == 1);

        object.Reset();

        REQUIRE_FALSE(object);
        REQUIRE(Tracked::Live() == 0);
    }

    SECTION("Being moved, which destroys nothing")
    {
        ObjectPoolUniquePtr<Tracked> object = pool.Allocate(1);
        Tracked* address = object.Get();

        ObjectPoolUniquePtr<Tracked> moved = Move(object);

        REQUIRE_FALSE(object);
        REQUIRE(moved.Get() == address);
        REQUIRE(Tracked::Live() == 1);
    }
}

TEST_CASE("A recycled slot is handed out again", "[ObjectPool]")
{
    Tracked::Reset();

    ObjectPool<Tracked> pool;

    SECTION("A single slot comes straight back")
    {
        Tracked* slot = nullptr;
        {
            ObjectPoolUniquePtr<Tracked> object = pool.Allocate(1);
            slot = object.Get();
        }

        ObjectPoolUniquePtr<Tracked> reused = pool.Allocate(2);

        REQUIRE(reused.Get() == slot);
        REQUIRE(reused->GetValue() == 2);
        REQUIRE(Tracked::Live() == 1);
    }

    SECTION("Slots come back in reverse order of release")
    {
        ObjectPoolUniquePtr<Tracked> first = pool.Allocate(1);
        ObjectPoolUniquePtr<Tracked> second = pool.Allocate(2);
        ObjectPoolUniquePtr<Tracked> third = pool.Allocate(3);

        Tracked* firstSlot = first.Get();
        Tracked* secondSlot = second.Get();
        Tracked* thirdSlot = third.Get();

        first.Reset();
        second.Reset();
        third.Reset();

        // The free list is a stack, so the last slot released is the first one reused.
        // The three handles have to be held while the check runs: releasing each one
        // before asking for the next would push its slot straight back on top and every
        // allocation would return the same address.
        ObjectPoolUniquePtr<Tracked> reusedFirst = pool.Allocate(0);
        ObjectPoolUniquePtr<Tracked> reusedSecond = pool.Allocate(0);
        ObjectPoolUniquePtr<Tracked> reusedThird = pool.Allocate(0);

        REQUIRE(reusedFirst.Get() == thirdSlot);
        REQUIRE(reusedSecond.Get() == secondSlot);
        REQUIRE(reusedThird.Get() == firstSlot);
    }
}

TEST_CASE("A pool grows past its first page", "[ObjectPool]")
{
    Tracked::Reset();

    ObjectPool<Tracked> pool;

    // Enough to need several pages, and not a multiple of a page, so the last one is
    // only partly used.
    const Index count = static_cast<Index>(FirstPageCapacity * 6 + 7);

    Array<ObjectPoolUniquePtr<Tracked>> objects;
    objects.Reserve(count);

    for (Index i = 0; i < count; ++i)
        objects.Add(pool.Allocate(static_cast<int>(i)));

    REQUIRE(objects.GetSize() == count);
    REQUIRE(Tracked::Live() == static_cast<int>(count));

    // Each object was given its own index as a value, so finding them all intact is
    // what rules out two slots overlapping across a page boundary.
    Array<void*> addresses;
    addresses.Reserve(count);

    for (Index i = 0; i < count; ++i)
    {
        REQUIRE(objects[i]->GetValue() == static_cast<int>(i));
        addresses.Add(objects[i].Get());
    }

    REQUIRE(AllDistinct(addresses));

    objects.Clear();

    REQUIRE(Tracked::Live() == 0);
}

TEST_CASE("A pool refuses a slot count it cannot describe", "[ObjectPool]")
{
    // The free list indexes its slots with Array's size type, and Array rounds capacity up
    // to a power of two, so a pool tops out at the largest power of two that type holds.
    // Tiny is one byte, so no byte size limit comes into play here and it is that index
    // bound alone which rejects the request. Nothing is allocated either: the bound is
    // checked before the page is asked for, which is the whole point of checking it.
    REQUIRE_THROWS_AS(ObjectPool<Tiny>((std::size_t(1) << 63) + 1), InvalidParameterException);
}

TEST_CASE("The free list has room for every slot the pool describes", "[ObjectPool]")
{
    Tracked::Reset();

    ObjectPool<Tracked> pool;

    // Fills the first page of 32 and then the second of 64, holding everything, so that the
    // second page is added while the whole of the first one is checked out. That is the
    // case where the reservation has to count the slots missing from the free list rather
    // than only those on it: every one of them comes back below, and the last has to find
    // room without the free list reallocating, since ReleaseSlot cannot throw.
    const Index count = static_cast<Index>(FirstPageCapacity + FirstPageCapacity * 2);

    Array<ObjectPoolUniquePtr<Tracked>> objects;
    objects.Reserve(count);

    for (Index i = 0; i < count; ++i)
        objects.Add(pool.Allocate(static_cast<int>(i)));

    REQUIRE(Tracked::Live() == static_cast<int>(count));

    objects.Clear();

    REQUIRE(Tracked::Live() == 0);

    // Every slot went back, so the same run is served again and no two objects share one.
    Array<void*> addresses;
    addresses.Reserve(count);

    for (Index i = 0; i < count; ++i)
        objects.Add(pool.Allocate(static_cast<int>(i)));

    for (Index i = 0; i < count; ++i)
    {
        REQUIRE(objects[i]->GetValue() == static_cast<int>(i));
        addresses.Add(objects[i].Get());
    }

    REQUIRE(AllDistinct(addresses));
    REQUIRE(Tracked::Live() == static_cast<int>(count));
}

TEST_CASE("A pool reports what it holds", "[ObjectPool]")
{
    Tracked::Reset();

    SECTION("A lazily allocated pool holds nothing")
    {
        ObjectPool<Tracked> pool;
        const ObjectPoolStatistics statistics = pool.GetStatistics();

        REQUIRE(statistics.SlotCount == 0);
        REQUIRE(statistics.AllocatedCount == 0);
        REQUIRE(statistics.VacantCount == 0);
        REQUIRE(statistics.PageCount == 0);
        REQUIRE(statistics.ByteCount == 0);
    }

    SECTION("A pre-allocated pool holds one page of vacant slots")
    {
        ObjectPool<Tracked> pool(48);
        const ObjectPoolStatistics statistics = pool.GetStatistics();

        REQUIRE(statistics.SlotCount == 48);
        REQUIRE(statistics.AllocatedCount == 0);
        REQUIRE(statistics.VacantCount == 48);
        REQUIRE(statistics.PageCount == 1);
        REQUIRE(statistics.ByteCount == 48 * sizeof(Tracked));
    }

    SECTION("Handing objects out moves slots from vacant to allocated")
    {
        ObjectPool<Tracked> pool(48);

        Array<ObjectPoolUniquePtr<Tracked>> objects;
        objects.Reserve(10);

        for (Index i = 0; i < 10; ++i)
            objects.Add(pool.Allocate(static_cast<int>(i)));

        const ObjectPoolStatistics statistics = pool.GetStatistics();

        REQUIRE(statistics.AllocatedCount == 10);
        REQUIRE(statistics.VacantCount == 38);
        REQUIRE(statistics.SlotCount == 48);
        REQUIRE(statistics.PageCount == 1);
    }

    SECTION("The three counts always agree")
    {
        ObjectPool<Tracked> pool;

        Array<ObjectPoolUniquePtr<Tracked>> objects;
        objects.Reserve(static_cast<Index>(FirstPageCapacity * 3));

        for (Index i = 0; i < static_cast<Index>(FirstPageCapacity * 3); ++i)
        {
            objects.Add(pool.Allocate(static_cast<int>(i)));

            const ObjectPoolStatistics statistics = pool.GetStatistics();

            REQUIRE(statistics.VacantCount + statistics.AllocatedCount == statistics.SlotCount);
            REQUIRE(statistics.AllocatedCount == static_cast<std::size_t>(i) + 1);
            REQUIRE(statistics.ByteCount == statistics.SlotCount * sizeof(Tracked));
        }
    }

    SECTION("Releasing everything frees no memory")
    {
        ObjectPool<Tracked> pool;

        Array<ObjectPoolUniquePtr<Tracked>> objects;
        objects.Reserve(static_cast<Index>(FirstPageCapacity * 3));

        for (Index i = 0; i < static_cast<Index>(FirstPageCapacity * 3); ++i)
            objects.Add(pool.Allocate(static_cast<int>(i)));

        const ObjectPoolStatistics busy = pool.GetStatistics();

        objects.Clear();

        const ObjectPoolStatistics idle = pool.GetStatistics();

        // The pool never gives a page back, so what a peak in demand costs is held for the
        // rest of its life. Reporting that is the point of these numbers.
        REQUIRE(idle.AllocatedCount == 0);
        REQUIRE(idle.VacantCount == busy.SlotCount);
        REQUIRE(idle.SlotCount == busy.SlotCount);
        REQUIRE(idle.PageCount == busy.PageCount);
        REQUIRE(idle.ByteCount == busy.ByteCount);
    }

    SECTION("Growing adds a page of the expected size")
    {
        ObjectPool<Tracked> pool;

        ObjectPoolUniquePtr<Tracked> first = pool.Allocate(0);

        REQUIRE(pool.GetStatistics().PageCount == 1);
        REQUIRE(pool.GetStatistics().SlotCount == FirstPageCapacity);

        Array<ObjectPoolUniquePtr<Tracked>> objects;
        objects.Reserve(static_cast<Index>(FirstPageCapacity));

        for (Index i = 0; i < static_cast<Index>(FirstPageCapacity); ++i)
            objects.Add(pool.Allocate(static_cast<int>(i)));

        // The n-th page holds 32 * n slots, so the second one doubles what the pool
        // describes and then some.
        const ObjectPoolStatistics statistics = pool.GetStatistics();

        REQUIRE(statistics.PageCount == 2);
        REQUIRE(statistics.SlotCount == FirstPageCapacity * 3);
        REQUIRE(statistics.AllocatedCount == FirstPageCapacity + 1);
    }

    SECTION("A construction that throws leaves no slot checked out")
    {
        Detonator::Reset();

        ObjectPool<Detonator> pool;

        REQUIRE_THROWS_AS(pool.Allocate(true), RuntimeException);

        const ObjectPoolStatistics statistics = pool.GetStatistics();

        REQUIRE(statistics.AllocatedCount == 0);
        REQUIRE(statistics.VacantCount == statistics.SlotCount);
    }
}

TEST_CASE("A pre-allocated pool serves its first objects from one page", "[ObjectPool]")
{
    Tracked::Reset();

    const Index count = 64;
    ObjectPool<Tracked> pool(count);

    Array<ObjectPoolUniquePtr<Tracked>> objects;
    objects.Reserve(count);

    for (Index i = 0; i < count; ++i)
        objects.Add(pool.Allocate(static_cast<int>(i)));

    REQUIRE(Tracked::Live() == static_cast<int>(count));

    // A single page of exactly count slots was reserved up front, so the objects span
    // one contiguous run of storage and nothing had to be allocated to serve them.
    const std::uintptr_t first = reinterpret_cast<std::uintptr_t>(objects[0].Get());
    std::uintptr_t lowest = first;
    std::uintptr_t highest = first;

    for (Index i = 1; i < count; ++i)
    {
        const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(objects[i].Get());

        lowest = std::min(lowest, address);
        highest = std::max(highest, address);
    }

    REQUIRE(highest - lowest == (count - 1) * sizeof(Tracked));

    objects.Clear();

    REQUIRE(Tracked::Live() == 0);
}

TEST_CASE("A throwing constructor leaves the pool usable", "[ObjectPool]")
{
    Detonator::Reset();

    ObjectPool<Detonator> pool;

    Detonator* slot = nullptr;
    {
        ObjectPoolUniquePtr<Detonator> probe = pool.Allocate(false);
        slot = probe.Get();
    }

    REQUIRE(Detonator::Constructed == 1);
    REQUIRE(Detonator::Destroyed == 1);

    REQUIRE_THROWS_AS(pool.Allocate(true), RuntimeException);

    // Nothing was constructed, so nothing is owed a destructor and the count of live
    // objects is untouched.
    REQUIRE(Detonator::Constructed == 1);
    REQUIRE(Detonator::Destroyed == 1);

    // The slot the failed construction had taken was put back on the free list, so the
    // next allocation picks it up again rather than growing the pool.
    ObjectPoolUniquePtr<Detonator> after = pool.Allocate(false);

    REQUIRE(after.Get() == slot);
    REQUIRE(Detonator::Constructed == 2);
}

TEST_CASE("Pooled objects outlive the pool they came from", "[ObjectPool]")
{
    Tracked::Reset();

    ObjectPoolUniquePtr<Tracked> object;
    {
        ObjectPool<Tracked> pool;
        object = pool.Allocate(7);
    }

    // The pool is gone, but the deleter carried in the handle holds the storage alive.
    REQUIRE(object);
    REQUIRE(object->GetValue() == 7);
    REQUIRE(Tracked::Live() == 1);

    object.Reset();

    REQUIRE(Tracked::Live() == 0);
}

TEST_CASE("Pages honour the alignment of the pooled type", "[ObjectPool]")
{
    // Past the first page in both cases, so the check covers a page the pool had to
    // grow into rather than only the one it started with.
    const Index count = static_cast<Index>(FirstPageCapacity + 8);

    SECTION("Over-aligned type")
    {
        ObjectPool<OverAligned> pool;

        Array<ObjectPoolUniquePtr<OverAligned>> objects;
        objects.Reserve(count);

        for (Index i = 0; i < count; ++i)
            objects.Add(pool.Allocate(static_cast<int>(i)));

        for (Index i = 0; i < count; ++i)
        {
            REQUIRE(reinterpret_cast<std::uintptr_t>(objects[i].Get()) % alignof(OverAligned) == 0);
            REQUIRE(objects[i]->Value == static_cast<int>(i));
        }
    }

    SECTION("Single byte type")
    {
        ObjectPool<Tiny> pool;

        Array<ObjectPoolUniquePtr<Tiny>> objects;
        objects.Reserve(count);

        for (Index i = 0; i < count; ++i)
            objects.Add(pool.Allocate(static_cast<char>('a' + i % 26)));

        Array<void*> addresses;
        addresses.Reserve(count);

        for (Index i = 0; i < count; ++i)
        {
            REQUIRE(objects[i]->Value == static_cast<char>('a' + i % 26));
            addresses.Add(objects[i].Get());
        }

        REQUIRE(AllDistinct(addresses));
    }
}

TEST_CASE("A pooled constructor may re-enter its own pool", "[ObjectPool]")
{
    Reentrant::Reset();

    ObjectPool<Reentrant> pool;
    ObjectPoolUniquePtr<Reentrant> root = pool.Allocate(pool, 3);

    REQUIRE(root->GetDepth() == 3);
    REQUIRE(root->GetChildDepth() == 2);

    // Four objects were constructed, and the three nested ones were released before
    // the outermost constructor returned.
    REQUIRE(Reentrant::Constructed == 4);
    REQUIRE(Reentrant::Destroyed == 3);

    root.Reset();

    REQUIRE(Reentrant::Destroyed == 4);
}

// AllocatePage turns a null from Memory::Allocate into an OutOfMemory, and that path is
// deliberately left uncovered here: it cannot be reached through the allocator the engine
// actually runs. BinnedAllocator::Allocate does not honour the nullptr-on-failure contract
// its header states for a request too large to bin. It asserts on the result and then
// writes the block header through it, so a failed page allocation aborts in a build with
// assertions and faults in one without, before ObjectPool is ever handed the null it is
// prepared for. Covering the OutOfMemory path means fixing that first.

TEMPLATE_TEST_CASE("A pool recycles the same way whatever it locks with", "[ObjectPool]",
    ObjectPool<Tracked>, SpinObjectPool<Tracked>, TlsObjectPool<Tracked>)
{
    Tracked::Reset();

    TestType pool;

    Tracked* slot = nullptr;
    {
        ObjectPoolUniquePtr<Tracked> object = pool.Allocate(9);
        slot = object.Get();

        REQUIRE(object->GetValue() == 9);
        REQUIRE(Tracked::Live() == 1);
    }

    REQUIRE(Tracked::Live() == 0);

    ObjectPoolUniquePtr<Tracked> reused = pool.Allocate(10);

    REQUIRE(reused.Get() == slot);
    REQUIRE(reused->GetValue() == 10);

    // Reading the counters locks, so it has to compile and run against every lock type and
    // not only against the one the plain aliases use.
    const ObjectPoolStatistics statistics = pool.GetStatistics();

    REQUIRE(statistics.AllocatedCount == 1);
    REQUIRE(statistics.VacantCount + statistics.AllocatedCount == statistics.SlotCount);
}

TEST_CASE("Concurrent allocations never hand out the same slot twice", "[ObjectPool]")
{
    Tracked::Reset();

    ObjectPool<Tracked> pool;

    const unsigned int workerCount = WorkerCount();
    const std::size_t perWorker = 64;

    // One array per worker, all of them emplaced before the run against a capacity
    // reserved for exactly that many, so the outer array cannot reallocate while workers
    // hold references into it and a worker only ever touches its own. Everything stays
    // alive until after the join, which is what makes overlapping slots detectable at all.
    Array<Array<ObjectPoolUniquePtr<Tracked>>> harvest;
    harvest.Reserve(static_cast<Index>(workerCount));

    for (unsigned int index = 0; index < workerCount; ++index)
        harvest.Emplace();

    RunOnThreads(workerCount, [&](unsigned int index) {
        Array<ObjectPoolUniquePtr<Tracked>>& mine = harvest[index];
        mine.Reserve(static_cast<Index>(perWorker));

        for (std::size_t i = 0; i < perWorker; ++i)
            mine.Add(pool.Allocate(static_cast<int>(index)));
    });

    const Index expected = static_cast<Index>(workerCount * perWorker);

    Array<void*> addresses;
    addresses.Reserve(expected);

    for (const Array<ObjectPoolUniquePtr<Tracked>>& mine : harvest)
    {
        for (const ObjectPoolUniquePtr<Tracked>& object : mine)
            addresses.Add(object.Get());
    }

    REQUIRE(addresses.GetSize() == expected);
    REQUIRE(Tracked::Live() == static_cast<int>(expected));
    REQUIRE(AllDistinct(addresses));

    harvest.Clear();

    REQUIRE(Tracked::Live() == 0);
}

TEST_CASE("Concurrent churn balances construction and destruction", "[ObjectPool]")
{
    Tracked::Reset();

    ObjectPool<Tracked> pool;

    const unsigned int workerCount = WorkerCount();
    const std::size_t iterations = 2000;

    // Catch2 assertions are not thread safe, so a worker reports by counting and the
    // result is asserted on the main thread once every worker has joined.
    std::atomic<int> mismatches{ 0 };

    RunOnThreads(workerCount, [&](unsigned int index) {
        const int value = static_cast<int>(index);

        for (std::size_t i = 0; i < iterations; ++i)
        {
            ObjectPoolUniquePtr<Tracked> object = pool.Allocate(value);

            // A slot handed to two workers at once would show up here as a value one
            // of them never asked for.
            if (!object || object->GetValue() != value)
                mismatches.fetch_add(1, std::memory_order_relaxed);
        }
    });

    CHECK(mismatches.load() == 0);
    CHECK(Tracked::Constructed.load() == static_cast<int>(workerCount * iterations));
    REQUIRE(Tracked::Live() == 0);
}

// A SharedObjectPool does not pool T. It pools the control block that carries T, so one
// slot holds the reference counts and the object side by side and shared ownership costs
// no allocation of its own. That makes its teardown two-staged, and most of what follows
// is about the seam between the two stages.

TEST_CASE("Allocating from a shared pool constructs an object", "[SharedObjectPool]")
{
    Tracked::Reset();

    SharedObjectPool<Tracked> pool;

    SECTION("Shared ownership")
    {
        SharedPtr<Tracked> object = pool.Allocate(42);

        REQUIRE(object);
        REQUIRE(object->GetValue() == 42);
        REQUIRE(Tracked::Live() == 1);
    }

    SECTION("Several constructor arguments are forwarded")
    {
        SharedPtr<Tracked> object = pool.Allocate(40, 2);

        REQUIRE(object->GetValue() == 42);
    }

    SECTION("Two live objects are two distinct objects")
    {
        SharedPtr<Tracked> first = pool.Allocate(1);
        SharedPtr<Tracked> second = pool.Allocate(2);

        REQUIRE(first.Get() != second.Get());
        REQUIRE(first->GetValue() == 1);
        REQUIRE(second->GetValue() == 2);
        REQUIRE(Tracked::Live() == 2);
    }
}

TEST_CASE("Shared handles share one pooled object", "[SharedObjectPool]")
{
    Tracked::Reset();

    SharedObjectPool<Tracked> pool;

    Tracked* slot = nullptr;
    {
        SharedPtr<Tracked> first = pool.Allocate(5);
        slot = first.Get();

        {
            SharedPtr<Tracked> second = first;

            REQUIRE(second.Get() == slot);
            REQUIRE(Tracked::Live() == 1);
        }

        // Dropping one of two references destroys nothing.
        REQUIRE(Tracked::Live() == 1);
    }

    REQUIRE(Tracked::Live() == 0);

    // The object sits at a fixed offset inside its control block, so a reused slot puts it
    // back at the very same address.
    SharedPtr<Tracked> reused = pool.Allocate(6);

    REQUIRE(reused.Get() == slot);
    REQUIRE(reused->GetValue() == 6);
}

TEST_CASE("A weak reference defers the return of the slot", "[SharedObjectPool]")
{
    Tracked::Reset();

    SharedObjectPool<Tracked> pool;

    Tracked* slot = nullptr;
    WeakPtr<Tracked> weak;
    {
        SharedPtr<Tracked> object = pool.Allocate(1);
        slot = object.Get();
        weak = object;

        REQUIRE_FALSE(weak.IsExpired());
    }

    // The object goes as soon as the last strong reference does, but the reference counts
    // live in the slot, so the slot itself cannot come back while a weak reference can
    // still be asked whether the object is alive.
    REQUIRE(Tracked::Live() == 0);
    REQUIRE(weak.IsExpired());
    REQUIRE(weak.ToStrong() == nullptr);

    SharedPtr<Tracked> other = pool.Allocate(2);

    REQUIRE(other.Get() != slot);

    other = nullptr;
    weak = nullptr;

    // Both slots are back now, the deferred one released last and therefore reused first.
    SharedPtr<Tracked> reused = pool.Allocate(3);

    REQUIRE(reused.Get() == slot);
    REQUIRE(reused->GetValue() == 3);
}

TEST_CASE("Shared pooled objects outlive the pool they came from", "[SharedObjectPool]")
{
    Tracked::Reset();

    {
        SharedPtr<Tracked> object;
        {
            SharedObjectPool<Tracked> pool;
            object = pool.Allocate(7);
        }

        // The control block holds a reference on the pool state, so the storage stays
        // alive even though the pool itself is gone.
        REQUIRE(object);
        REQUIRE(object->GetValue() == 7);
        REQUIRE(Tracked::Live() == 1);
    }

    REQUIRE(Tracked::Live() == 0);
}

TEST_CASE("A shared pooled object can reach a SharedPtr to itself", "[SharedObjectPool]")
{
    SharedObjectPool<Shareable> pool;

    Shareable* slot = nullptr;
    {
        SharedPtr<Shareable> object = pool.Allocate(3);
        slot = object.Get();

        REQUIRE(object->AsShared().Get() == slot);
        REQUIRE_FALSE(object->AsWeak().IsExpired());
    }

    // The slot comes back with a fresh object in it, so its self reference is the new one
    // rather than the expired reference the previous occupant left behind.
    SharedPtr<Shareable> reused = pool.Allocate(4);

    REQUIRE(reused.Get() == slot);
    REQUIRE(reused->GetValue() == 4);
    REQUIRE(reused->AsShared().Get() == slot);
}

TEST_CASE("A throwing constructor leaves the shared pool usable", "[SharedObjectPool]")
{
    Detonator::Reset();

    SharedObjectPool<Detonator> pool;

    Detonator* slot = nullptr;
    {
        SharedPtr<Detonator> probe = pool.Allocate(false);
        slot = probe.Get();
    }

    REQUIRE(Detonator::Constructed == 1);
    REQUIRE(Detonator::Destroyed == 1);

    REQUIRE_THROWS_AS(pool.Allocate(true), RuntimeException);

    REQUIRE(Detonator::Constructed == 1);
    REQUIRE(Detonator::Destroyed == 1);

    // The control block was being built when the object inside it threw, so the whole slot
    // had to go back untouched.
    SharedPtr<Detonator> after = pool.Allocate(false);

    REQUIRE(after.Get() == slot);
    REQUIRE(Detonator::Constructed == 2);
}

TEST_CASE("A shared pool grows past its first page", "[SharedObjectPool]")
{
    Tracked::Reset();

    SharedObjectPool<Tracked> pool;

    const Index count = static_cast<Index>(FirstPageCapacity * 4 + 5);

    Array<SharedPtr<Tracked>> objects;
    objects.Reserve(count);

    for (Index i = 0; i < count; ++i)
        objects.Add(pool.Allocate(static_cast<int>(i)));

    REQUIRE(Tracked::Live() == static_cast<int>(count));

    Array<void*> addresses;
    addresses.Reserve(count);

    for (Index i = 0; i < count; ++i)
    {
        REQUIRE(objects[i]->GetValue() == static_cast<int>(i));
        addresses.Add(objects[i].Get());
    }

    REQUIRE(AllDistinct(addresses));

    objects.Clear();

    REQUIRE(Tracked::Live() == 0);
}

TEST_CASE("A shared slot honours the alignment of the object it carries", "[SharedObjectPool]")
{
    // The object does not sit at the start of its slot here, it sits after the reference
    // counts. What keeps it aligned is that the control block inherits the alignment of the
    // storage it embeds, so the page is allocated for the stricter of the two and the
    // offset of the object inside it is a multiple of its own alignment.
    SharedObjectPool<OverAligned> pool;

    const Index count = static_cast<Index>(FirstPageCapacity + 8);

    Array<SharedPtr<OverAligned>> objects;
    objects.Reserve(count);

    for (Index i = 0; i < count; ++i)
        objects.Add(pool.Allocate(static_cast<int>(i)));

    for (Index i = 0; i < count; ++i)
    {
        REQUIRE(reinterpret_cast<std::uintptr_t>(objects[i].Get()) % alignof(OverAligned) == 0);
        REQUIRE(objects[i]->Value == static_cast<int>(i));
    }
}

TEMPLATE_TEST_CASE("A shared pool recycles the same way whatever it locks with", "[SharedObjectPool]",
    SharedObjectPool<Tracked>, SpinSharedObjectPool<Tracked>, TlsSharedObjectPool<Tracked>)
{
    Tracked::Reset();

    TestType pool;

    Tracked* slot = nullptr;
    {
        SharedPtr<Tracked> object = pool.Allocate(9);
        slot = object.Get();

        REQUIRE(object->GetValue() == 9);
        REQUIRE(Tracked::Live() == 1);
    }

    REQUIRE(Tracked::Live() == 0);

    SharedPtr<Tracked> reused = pool.Allocate(10);

    REQUIRE(reused.Get() == slot);
    REQUIRE(reused->GetValue() == 10);

    // Reading the counters locks, so it has to compile and run against every lock type and
    // not only against the one the plain aliases use.
    const ObjectPoolStatistics statistics = pool.GetStatistics();

    REQUIRE(statistics.AllocatedCount == 1);
    REQUIRE(statistics.VacantCount + statistics.AllocatedCount == statistics.SlotCount);
}

TEST_CASE("Concurrent shared allocations never hand out the same slot twice", "[SharedObjectPool]")
{
    Tracked::Reset();

    SharedObjectPool<Tracked> pool;

    const unsigned int workerCount = WorkerCount();
    const std::size_t perWorker = 64;

    Array<Array<SharedPtr<Tracked>>> harvest;
    harvest.Reserve(static_cast<Index>(workerCount));

    for (unsigned int index = 0; index < workerCount; ++index)
        harvest.Emplace();

    RunOnThreads(workerCount, [&](unsigned int index) {
        Array<SharedPtr<Tracked>>& mine = harvest[index];
        mine.Reserve(static_cast<Index>(perWorker));

        for (std::size_t i = 0; i < perWorker; ++i)
            mine.Add(pool.Allocate(static_cast<int>(index)));
    });

    const Index expected = static_cast<Index>(workerCount * perWorker);

    Array<void*> addresses;
    addresses.Reserve(expected);

    for (const Array<SharedPtr<Tracked>>& mine : harvest)
    {
        for (Ptr<Tracked> object : mine)
            addresses.Add(object.Get());
    }

    REQUIRE(addresses.GetSize() == expected);
    REQUIRE(Tracked::Live() == static_cast<int>(expected));
    REQUIRE(AllDistinct(addresses));

    harvest.Clear();

    REQUIRE(Tracked::Live() == 0);
}

TEST_CASE("A shared pool reports what it holds", "[SharedObjectPool]")
{
    Tracked::Reset();

    SECTION("A slot costs more than the object it carries")
    {
        SharedObjectPool<Tracked> pool(16);
        const ObjectPoolStatistics statistics = pool.GetStatistics();

        REQUIRE(statistics.SlotCount == 16);
        REQUIRE(statistics.VacantCount == 16);
        REQUIRE(statistics.PageCount == 1);

        // A slot holds the reference counts as well as the object, so the storage held is
        // strictly more than the objects alone would need.
        REQUIRE(statistics.ByteCount > 16 * sizeof(Tracked));
    }

    SECTION("Handing objects out moves slots from vacant to allocated")
    {
        SharedObjectPool<Tracked> pool(16);

        Array<SharedPtr<Tracked>> objects;
        objects.Reserve(6);

        for (Index i = 0; i < 6; ++i)
            objects.Add(pool.Allocate(static_cast<int>(i)));

        const ObjectPoolStatistics statistics = pool.GetStatistics();

        REQUIRE(statistics.AllocatedCount == 6);
        REQUIRE(statistics.VacantCount == 10);
        REQUIRE(statistics.SlotCount == 16);
    }

    SECTION("A forgotten weak reference shows up as a slot that never comes back")
    {
        SharedObjectPool<Tracked> pool(16);

        WeakPtr<Tracked> forgotten;
        {
            SharedPtr<Tracked> object = pool.Allocate(1);
            forgotten = object;

            REQUIRE(pool.GetStatistics().AllocatedCount == 1);
        }

        // This is the case the counters exist for. The object is gone, so nothing an owner
        // can observe says anything is wrong, yet the slot is still held and no amount of
        // further work will reclaim it while the weak reference lives.
        REQUIRE(Tracked::Live() == 0);
        REQUIRE(forgotten.IsExpired());
        REQUIRE(pool.GetStatistics().AllocatedCount == 1);

        forgotten = nullptr;

        REQUIRE(pool.GetStatistics().AllocatedCount == 0);
    }
}
