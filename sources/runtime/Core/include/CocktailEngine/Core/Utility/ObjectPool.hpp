#ifndef COCKTAILENGINE_CORE_UTILITY_OBJECTPOOL_HPP
#define COCKTAILENGINE_CORE_UTILITY_OBJECTPOOL_HPP

#include <mutex>

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/Exception.hpp>
#include <CocktailEngine/Core/Memory/Asan.hpp>
#include <CocktailEngine/Core/Memory/SharedPtr.hpp>
#include <CocktailEngine/Core/Memory/UniquePtr.hpp>
#include <CocktailEngine/Core/System/Concurrency/NullMutex.hpp>
#include <CocktailEngine/Core/System/Concurrency/SpinMutex.hpp>

namespace Ck
{
    namespace Detail
    {
        /**
         * \class ObjectPoolState
         *
         * \brief Shared storage backing an ObjectPoolBase instance
         *
         * Holds the storage pages and the free list of a pool. Objects are carved out of
         * page allocations and constructed in place, so allocating a pooled object costs a
         * free list pop instead of a heap allocation.
         *
         * The state is always owned through a SharedPtr, referenced both by the pool and by
         * every deleter attached to a live object. A pooled object can therefore safely
         * outlive the ObjectPoolBase it came from: the state stays alive until the last
         * object has been recycled and the pool itself is gone.
         *
         * Invariants maintained by this class, relied upon elsewhere:
         *  - Pages are never moved nor released before the state is destroyed, so a pointer
         *    handed out by Allocate stays valid until it is recycled.
         *  - mVacants always has enough capacity to hold every slot of every page, so
         *    returning a slot to the free list never reallocates. This is what makes
         *    ReleaseSlot non-throwing, which ObjectPoolDeleter::operator() depends on.
         *  - mAllocatedObjectCount counts the slots that are not on the free list, which is
         *    not the same as the objects that are alive: a slot is counted from the moment
         *    it is handed out to the moment it is given back, and construction happens in
         *    between.
         *
         * \tparam T Type of the pooled objects
         * \tparam Lockable Lock type used to serialize access, must satisfy BasicLockable
         *
         * \remark The lock only ever covers the free list and the pages. Neither T's
         *         constructor nor its destructor runs under it, so a pooled type may
         *         allocate from, or recycle into, the very pool that is constructing or
         *         destroying it, and a \p Lockable that is not recursive is enough. It also
         *         means recycling an object never holds this pool's lock while taking
         *         another's, so pooled types that own pooled members impose no lock ordering.
         */
        template <typename T, typename Lockable>
        class ObjectPoolState
        {
        public:

            /**
             * \brief Constructor
             *
             * Creates an empty pool state, optionally allocating a first page upfront.
             *
             * \param preAllocated Number of objects the initial page must hold, 0 to allocate lazily
             *
             * \throw InvalidParameterException If \p preAllocated is beyond what a pool of T
             *                                  can describe, see MaxSlotCount
             * \throw OutOfMemory If the initial page could not be allocated
             */
            explicit ObjectPoolState(std::size_t preAllocated = 0) :
                mAllocatedObjectCount(0)
            {
                std::lock_guard<Lockable> lg(mMutex);
                if (preAllocated)
                    AllocatePage(preAllocated);
            }

            /**
             * \brief Destructor
             *
             * Releases every storage page. No destructor is run on the pooled objects
             * themselves: every object is expected to have been recycled beforehand, which
             * is guaranteed by the deleters holding a reference to this state.
             *
             * \remark Asserts that no object is still checked out. A failing assert means an
             *         object was destroyed without going through Recycle, or that a raw
             *         pointer obtained from Allocate was leaked.
             */
            ~ObjectPoolState()
            {
                assert(mAllocatedObjectCount == 0);

                std::lock_guard<Lockable> lg(mMutex);
                mVacants.Clear();
                mPages.Clear();
            }

            /**
             * \brief Takes a slot from the free list and constructs an object in it
             *
             * Grows the pool with a new page when no slot is available. If T's constructor
             * throws, the slot is returned to the free list and the exception propagates,
             * leaving the pool unchanged.
             *
             * \tparam Args Types of the arguments forwarded to T's constructor
             *
             * \param args Arguments forwarded to T's constructor
             *
             * \return Pointer to the newly constructed object, never null
             *
             * \throw OutOfMemory If a new page was needed and could not be allocated
             *
             * \remark The returned pointer is owned by the caller, who is responsible for
             *         passing it back to Recycle exactly once. Prefer the owning wrappers
             *         returned by ObjectPoolBase over calling this directly.
             */
            template <typename... Args>
            T* Allocate(Args&&... args)
            {
                T* location = AcquireSlot();

                if constexpr (std::is_nothrow_constructible_v<T, Args...>)
                {
                    return new (location) T(Forward<Args>(args)...);
                }
                else
                {
                    try
                    {
                        return new (location) T(Forward<Args>(args)...);
                    }
                    catch (...)
                    {
                        ReleaseSlot(location);
                        throw;
                    }
                }
            }

            /**
             * \brief Destroys an object and returns its slot to the free list
             *
             * The slot is poisoned and pushed back on the free list, ready to be handed out
             * again by a later Allocate. The underlying page memory is not released.
             *
             * \param object Object to recycle, may be null in which case the call is a no-op
             *
             * \remark Does not throw: the free list is guaranteed to have spare capacity, so
             *         pushing the slot back cannot reallocate. ObjectPoolDeleter relies on
             *         this to be noexcept.
             *
             * \warning \p object must come from this very pool and must not have been
             *          recycled already. Recycling the same object twice inserts the same
             *          slot twice in the free list and makes two later allocations return
             *          the same address. The assert below only catches this reliably in ASAN
             *          builds, since Asan::IsPoisoned always reports false otherwise.
             */
            void Recycle(T* object)
            {
                if (object == nullptr)
                    return;

                assert(!Asan::IsPoisoned(object));

                object->~T();

                ReleaseSlot(object);
            }

        private:

            /**
             * \brief Releases the raw storage of a page
             *
             * Only frees memory. Pages hold uninitialized storage, so no destructor is run.
             */
            struct PageDeleter
            {
                void operator()(T* page) const noexcept
                {
                    Memory::Free(page);
                }
            };

            /**
             * \brief Owning handle over the raw storage of a single page
             */
            using Page = UniquePtr<T, PageDeleter>;

            /**
             * \brief Takes a slot off the free list, growing the pool if it is empty
             *
             * The slot is accounted for as allocated as soon as it leaves the free list,
             * rather than once an object has been constructed in it. Construction happens
             * outside the lock, so this is what keeps the count meaning "slots that are not
             * on the free list" at every point another thread can observe it.
             *
             * \return Pointer to an unpoisoned slot no one else can reach, never null
             *
             * \throw OutOfMemory If a new page was needed and could not be allocated. The
             *                    free list and the count are left untouched in that case.
             */
            T* AcquireSlot()
            {
                std::lock_guard<Lockable> lg(mMutex);

                if (mVacants.IsEmpty())
                    AllocatePage((mPages.GetSize() + 1) * 32);

                T* location = mVacants.PopLast();
                ++mAllocatedObjectCount;

                Asan::Unpoison(location, sizeof(T));

                return location;
            }

            /**
             * \brief Puts a slot holding no object back on the free list
             *
             * The counterpart of AcquireSlot, used both to undo a construction that threw
             * and, by Recycle, to give back a slot whose object has just been destroyed.
             *
             * \param location Slot to give back, must not hold a live object
             *
             * \remark Non-throwing by construction: the free list is guaranteed to have
             *         spare capacity for every slot of every page, so pushing one back
             *         cannot reallocate. Both callers need that, one being a catch block and
             *         the other a noexcept deleter.
             */
            void ReleaseSlot(T* location) noexcept
            {
                Asan::Poison(location, sizeof(T));

                std::lock_guard<Lockable> lg(mMutex);
                assert(mAllocatedObjectCount > 0);

                mVacants.Add(location);
                --mAllocatedObjectCount;
            }

            /**
             * \brief Index and size type of the free list
             */
            using SlotIndex = typename LargeArray<T*>::SizeType;

            /**
             * \brief Largest number of slots a pool of T is able to describe
             *
             * Bounded twice over. A page states its size in bytes as sizeof(T) * objectCount,
             * so a slot count may not exceed what that product can hold. And every slot of
             * every page is pushed on the free list, so it may not exceed what the free list
             * can index either. Array rounds its capacity up to a power of two, which makes
             * the usable index bound the largest power of two its size type holds rather than
             * that type's maximum value.
             *
             * Both bounds sit far beyond any real pool. They exist so that a caller asking
             * for more than can be described is told so, instead of being handed a page
             * shorter than the slots published for it.
             */
            static constexpr std::size_t MaxSlotCount()
            {
                constexpr std::size_t byteBound = ~static_cast<std::size_t>(0) / sizeof(T);
                constexpr std::size_t indexBound = static_cast<std::size_t>(1) << (sizeof(SlotIndex) * 8 - 1);

                return byteBound < indexBound ? byteBound : indexBound;
            }

            /**
             * \brief Allocates a new page and pushes all of its slots on the free list
             *
             * Capacity is reserved on both arrays before any slot is registered, so the
             * registration loop cannot reallocate and cannot fail halfway through. Should a
             * reservation fail, the page is released by its owning handle.
             *
             * \param objectCount Number of objects the page must hold, must be greater than 0
             *
             * \throw InvalidParameterException If the pool would end up describing more slots
             *                                  than MaxSlotCount
             * \throw OutOfMemory If the page storage could not be allocated
             *
             * \remark The caller must hold mMutex.
             *
             * \remark Page sizes grow arithmetically, the n-th page holding 32 * n objects.
             *         Reaching a capacity of N objects therefore costs on the order of
             *         sqrt(N) separate allocations.
             */
            void AllocatePage(std::size_t objectCount)
            {
                assert(objectCount > 0);

                // Slots the pool already describes, which is not the length of the free list:
                // the slots currently handed out are missing from it and still have to be
                // counted, since they come back to it when they are recycled.
                const std::size_t totalSlots = mVacants.GetSize() + mAllocatedObjectCount;

                // Checked before anything is allocated, and written as a subtraction so the
                // check itself cannot wrap. Letting sizeof(T) * objectCount wrap below would
                // allocate a page shorter than asked for and then publish a slot pointer for
                // every object requested, most of them past its end.
                if (objectCount > MaxSlotCount() - totalSlots)
                    throw InvalidParameterException(CK_TEXT("ObjectPool: more slots requested than a pool of this type can describe"));

                const std::size_t pageSize = sizeof(T) * objectCount;

                void* pageMemory = Memory::Allocate(pageSize, alignof(T));
                if (!pageMemory)
                    throw OutOfMemory();

                Page page(static_cast<T*>(pageMemory));

                // Reserved for every slot the pool describes, not just for the ones on the
                // free list today. This is what makes ReleaseSlot non-throwing: the slots
                // currently handed out all come back here, and the last of them must find
                // room without reallocating. The bound checked above is what makes the cast
                // lossless.
                mVacants.Reserve(totalSlots + objectCount);
                mPages.Reserve(mPages.GetSize() + 1);

                for (std::size_t i = 0; i < objectCount; ++i)
                    mVacants.Add(page.Get() + i);

                Asan::Poison(page.Get(), pageSize);
                mPages.Add(Move(page));
            }

            Lockable mMutex; /*!< Serializes every access to the members below */
            LargeArray<T*> mVacants; /*!< Free list of unused slots, popped and pushed in LIFO order */
            LargeArray<Page> mPages; /*!< Owned storage pages, never moved nor released before destruction */
            std::size_t mAllocatedObjectCount; /*!< Number of objects currently checked out */
        };

        /**
         * \brief Deleter returning a pooled object to the pool it came from
         *
         * Keeps a strong reference on the pool state so that pooled objects remain valid
         * even if the ObjectPoolBase instance is destroyed first. The state is released once
         * the pool and every deleter referencing it are gone.
         *
         * \tparam T Type of the pooled objects
         * \tparam Lockable Lock type used by the pool state
         */
        template <typename T, typename Lockable>
        struct ObjectPoolDeleter
        {
            /**
             * \brief Destroys \p object and returns its slot to the pool
             *
             * \param object Object to recycle, may be null
             *
             * \warning PoolState must not be empty. A default constructed deleter holds no
             *          state and invoking it is undefined behaviour.
             */
            void operator()(T* object) const noexcept
            {
                PoolState->Recycle(object);
            }

            SharedPtr<ObjectPoolState<T, Lockable>> PoolState; /*!< Pool state the object is returned to */
        };

        /**
         * \class ObjectPoolBase
         *
         * \brief Fixed type object pool handing out owning pointers
         *
         * Recycles the storage of same typed objects instead of allocating one block per
         * object. Objects are constructed in place inside page allocations and their
         * storage returns to a free list on destruction.
         *
         * Objects are handed out through owning pointers whose deleter recycles them, so
         * lifetime is managed the same way as with MakeShared or MakeUnique. They may
         * outlive the pool instance itself.
         *
         * The pool is neither copyable nor movable: its identity is shared with every live
         * object through the deleters.
         *
         * \tparam T Type of the pooled objects
         * \tparam Lockable Lock type used to serialize access, must satisfy BasicLockable
         *
         * \remark Use the ObjectPool, SpinObjectPool and TlsObjectPool aliases rather than
         *         instantiating this template directly.
         */
        template <typename T, typename Lockable>
        class ObjectPoolBase
        {
        public:

            /**
             * \brief Shared ownership handle over a pooled object
             */
            using Shared = SharedPtr<T>;

            /**
             * \brief Exclusive ownership handle over a pooled object
             */
            using Unique = UniquePtr<T, ObjectPoolDeleter<T, Lockable>>;

            /**
             * \brief Constructor
             *
             * Creates a pool, optionally sized upfront to avoid growing on first use.
             *
             * \param preAllocated Number of objects to reserve storage for, 0 to allocate lazily
             *
             * \throw InvalidParameterException If \p preAllocated is beyond what a pool of T
             *                                  can describe
             * \throw OutOfMemory If the initial storage could not be allocated
             */
            explicit ObjectPoolBase(std::size_t preAllocated = 0)
            {
                mState = MakeShared<ObjectPoolState<T, Lockable>>(preAllocated);
            }

            /**
             * \brief Deleted copy constructor
             */
            ObjectPoolBase(const ObjectPoolBase& other) = delete;

            /**
             * \brief Deleted move constructor
             */
            ObjectPoolBase(ObjectPoolBase&& other) noexcept = delete;

            /**
             * \brief Deleted copy assignment operator
             */
            ObjectPoolBase& operator=(const ObjectPoolBase& other) = delete;

            /**
             * \brief Deleted move assignment operator
             */
            ObjectPoolBase& operator=(ObjectPoolBase&& other) noexcept = delete;

            /**
             * \brief Allocates a pooled object under shared ownership
             *
             * \tparam Args Types of the arguments forwarded to T's constructor
             *
             * \param args Arguments forwarded to T's constructor
             *
             * \return Shared handle recycling the object once the last reference is dropped
             *
             * \throw OutOfMemory If the pool had to grow and could not allocate
             *
             * \remark Shared ownership requires a reference counting control block, which is
             *         allocated on the heap on every call. Prefer AllocateUnique, which has
             *         no such overhead, whenever exclusive ownership is enough.
             */
            template <typename... Args>
            Shared Allocate(Args&&... args)
            {
                return Shared(mState->Allocate(Forward<Args>(args)...), ObjectPoolDeleter<T, Lockable>{ mState });
            }

            /**
             * \brief Allocates a pooled object under exclusive ownership
             *
             * \tparam Args Types of the arguments forwarded to T's constructor
             *
             * \param args Arguments forwarded to T's constructor
             *
             * \return Unique handle recycling the object when it goes out of scope
             *
             * \throw OutOfMemory If the pool had to grow and could not allocate
             */
            template <typename... Args>
            Unique AllocateUnique(Args&&... args)
            {
                return Unique(mState->Allocate(Forward<Args>(args)...), ObjectPoolDeleter<T, Lockable>{ mState });
            }

        private:

            SharedPtr<ObjectPoolState<T, Lockable>> mState; /*!< Storage shared with the deleters of live objects */
        };
    }

    /**
     * \brief Thread safe object pool
     *
     * The default choice for a pool reachable from several threads. Serializes access with
     * a blocking mutex, so a thread that finds the pool busy sleeps rather than spins.
     *
     * \tparam T Type of the pooled objects
     */
    template <typename T>
    using ObjectPool = Detail::ObjectPoolBase<T, std::recursive_mutex>;

    /**
     * \brief Thread safe object pool serialized by a spin lock
     *
     * Cheaper than ObjectPool when contention is low, which is what a pool is: the lock
     * covers a free list pop and nothing else, so it is never held for long.
     *
     * \tparam T Type of the pooled objects
     *
     * \remark SpinMutex is not recursive, which is not a restriction here: the pool runs no
     *         user code under its lock, so a re-entrant pooled type is served like any
     *         other. A thread that spins is waiting on another thread, never on itself.
     */
    template <typename T>
    using SpinObjectPool = Detail::ObjectPoolBase<T, SpinMutex>;

    /**
     * \brief Object pool without synchronization
     *
     * Drops locking entirely, for a pool confined to a single thread. Both the pool and
     * every object it hands out must be allocated and recycled from the owning thread.
     *
     * \tparam T Type of the pooled objects
     */
    template <typename T>
    using TlsObjectPool = Detail::ObjectPoolBase<T, NullMutex>;
}

#endif // COCKTAILENGINE_CORE_UTILITY_OBJECTPOOL_HPP
