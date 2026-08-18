#ifndef COCKTAILENGINE_CORE_UTILITY_OBJECTPOOL_HPP
#define COCKTAILENGINE_CORE_UTILITY_OBJECTPOOL_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/Exception.hpp>
#include <CocktailEngine/Core/Memory/Asan.hpp>
#include <CocktailEngine/Core/Memory/SharedPtr.hpp>
#include <CocktailEngine/Core/Memory/UniquePtr.hpp>
#include <CocktailEngine/Core/System/Concurrency/LockGuard.hpp>
#include <CocktailEngine/Core/System/Concurrency/Mutex.hpp>
#include <CocktailEngine/Core/System/Concurrency/NullMutex.hpp>
#include <CocktailEngine/Core/System/Concurrency/SpinMutex.hpp>

namespace Ck
{
    /**
     * \brief Snapshot of what an object pool holds
     *
     * Every field is read under a single lock, so the numbers agree with one another. Read
     * one at a time they would not: a pool serving other threads moves between two reads,
     * and a caller could see more slots handed out than the pool describes.
     *
     * A pool never releases a page, so SlotCount and ByteCount only ever grow. They report
     * the high water mark of the pool rather than what it is using right now, which is what
     * makes them worth watching: AllocatedCount coming back to zero while SlotCount stays
     * high is a pool that was once asked for far more than it usually needs.
     */
    struct ObjectPoolStatistics
    {
        std::size_t SlotCount; /*!< Slots the pool describes, across every page it holds */
        std::size_t AllocatedCount; /*!< Slots handed out and not yet returned */
        std::size_t VacantCount; /*!< Slots on the free list, ready to be handed out again */
        std::size_t PageCount; /*!< Storage pages the pool has allocated */
        std::size_t ByteCount; /*!< Bytes of slot storage held, excluding allocator overhead */
    };

    namespace Detail
    {
        template <typename T>
        struct ObjectPoolDeleter;

        template <typename T>
        class SharedObjectPoolRefCounter;

        /**
         * \brief The one thing a pooled object needs from the pool that owns it
         *
         * A pool state is parameterized by its lock type, but nothing on the release path
         * cares which lock that is. Exposing recycling through this interface is what keeps
         * the lock out of the handle types: every pool of T, whichever lock it serializes
         * with, hands out the same ObjectPoolUniquePtr<T>, and a handle can be moved and
         * stored without its declaration naming a lock.
         *
         * The price is one indirect call per release, paid on a path that already touches a
         * lock and a free list.
         *
         * \tparam T Type of the pooled objects
         */
        template <typename T>
        class AbstractObjectPoolState
        {
        public:

            /**
             * \brief Default destructor
             */
            virtual ~AbstractObjectPoolState() = default;

        protected:

            template <typename>
            friend struct ObjectPoolDeleter;

            template <typename>
            friend class SharedObjectPoolRefCounter;

            /**
             * \brief Destroys an object and returns its slot to the pool
             *
             * Protected rather than public, and reachable only by the two friends declared
             * just above. That is what makes "only a deleter recycles" a rule the compiler
             * enforces instead of a convention, and what lets the implementation treat its
             * preconditions as invariants rather than as input to validate.
             *
             * \param object Object to recycle, never null
             */
            virtual void Recycle(T* object) = 0;
        };

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
         * \tparam T Type occupying a slot. For an ObjectPoolBase that is the pooled type
         *         itself, for a SharedObjectPoolBase it is the control block carrying it, so
         *         this class knows nothing of the distinction between the two pools
         * \tparam TLockable Lock type used to serialize access, must satisfy Lockable
         *
         * \remark The lock only ever covers the free list and the pages. Neither T's
         *         constructor nor its destructor runs under it, so a pooled type may
         *         allocate from, or recycle into, the very pool that is constructing or
         *         destroying it, and a \p Lockable that is not recursive is enough. It also
         *         means recycling an object never holds this pool's lock while taking
         *         another's, so pooled types that own pooled members impose no lock ordering.
         */
        template <typename T, Lockable TLockable>
        class ObjectPoolState : public AbstractObjectPoolState<T>
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
                LockGuard<TLockable> lg(mMutex);
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
            ~ObjectPoolState() override
            {
                assert(mAllocatedObjectCount == 0);

                LockGuard lg(mMutex);
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
             * \brief Reads what the pool currently holds
             *
             * \return A snapshot whose fields are consistent with one another
             *
             * \remark Takes the lock, so it contends with allocation. Meant for budgeting,
             *         reporting and leak hunting rather than for a decision taken per frame.
             *
             * \remark ByteCount is derived rather than tracked, and is exact: every page is
             *         sized as sizeof(T) * objectCount and SlotCount is the sum of those
             *         counts, so their product is the storage held. It leaves out whatever
             *         the memory allocator adds around each page.
             */
            ObjectPoolStatistics GetStatistics() const
            {
                LockGuard lg(mMutex);

                ObjectPoolStatistics statistics;
                statistics.VacantCount = mVacants.GetSize();
                statistics.AllocatedCount = mAllocatedObjectCount;
                statistics.SlotCount = statistics.VacantCount + statistics.AllocatedCount;
                statistics.PageCount = mPages.GetSize();
                statistics.ByteCount = statistics.SlotCount * sizeof(T);

                return statistics;
            }

        protected:

            /**
             * \brief Destroys an object and returns its slot to the free list
             *
             * The slot is poisoned and pushed back on the free list, ready to be handed out
             * again by a later Allocate. The underlying page memory is not released.
             *
             * \param object Object to recycle, never null
             *
             * \remark Does not throw: the free list is guaranteed to have spare capacity, so
             *         pushing the slot back cannot reallocate. Both callers need that, one
             *         being ObjectPoolDeleter::operator() and the other
             *         SharedObjectPoolRefCounter::Delete, neither of which may fail.
             *
             * \remark The two halves of the assert are not the same kind of check. That
             *         \p object is not null is an invariant, not an input to validate: this
             *         method is protected and its only callers are the two friends named
             *         above, one of them handed a pointer UniquePtr has already tested and the
             *         other passing this. A null is therefore unreachable, and there is
             *         deliberately no fallback for one: returning quietly would lose the slot
             *         AcquireSlot has already counted, and the loss would surface much later
             *         as an unrelated complaint from the destructor rather than here.
             *
             * \warning The state must stay alive for the whole call. That is not automatic:
             *          the object being destroyed here may itself hold the last reference to
             *          this state, which is why SharedObjectPoolRefCounter::Delete moves its
             *          own reference to a local before calling in.
             *
             * \warning \p object must come from this very pool and must not have been
             *          recycled already. Recycling the same object twice inserts the same slot
             *          twice in the free list and makes two later allocations return the same
             *          address. That is what the poison half of the assert is for, and it only
             *          works in an ASAN build: Asan::IsPoisoned reports false when the
             *          sanitizer is out, so nothing catches it otherwise.
             */
            void Recycle(T* object) override
            {
                assert(object && !Asan::IsPoisoned(object));
                ObjectMemoryUtils::Destroy(object);
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
                LockGuard lg(mMutex);

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
                LockGuard lg(mMutex);

                Asan::Poison(location, sizeof(T));

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
                    throw InvalidParameterException(CK_TEXT("Trying to create more slots than the pool can handle"));

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

            mutable TLockable mMutex; /*!< Serializes every access to the members below */
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
         */
        template <typename T>
        struct ObjectPoolDeleter
        {
            /**
             * \brief Destroys \p object and returns its slot to the pool
             *
             * \param object Object to recycle, must not be null
             *
             * \warning PoolState must not be empty. A default constructed deleter holds no
             *          state and invoking it is undefined behaviour. UniquePtr only calls a
             *          deleter for a non-null pointer, so a default constructed handle is
             *          harmless until something gives it one.
             */
            void operator()(T* object) const noexcept
            {
                PoolState->Recycle(object);
            }

            SharedPtr<AbstractObjectPoolState<T>> PoolState; /*!< Pool state the object is returned to */
        };

        /**
         * \class SharedObjectPoolRefCounter
         *
         * \brief Control block carrying a pooled object in its own storage
         *
         * What a SharedObjectPool actually pools. Shared ownership needs a control block
         * next to the object, so rather than pooling T and allocating a control block per
         * handle, the pool holds slots of this type: one slot carries the reference counts
         * and the object side by side, and shared ownership costs no allocation of its own.
         *
         * Teardown therefore happens in two stages, inherited from RefCounter. Destroy runs
         * when the last strong reference goes and destroys the object. Delete runs when the
         * last weak reference goes as well, and only then does the slot return to the pool.
         * A live WeakPtr keeps a slot checked out after its object is gone, which is the
         * same trade MakeShared makes.
         *
         * \tparam T Type of the pooled objects
         */
        template <typename T>
        class SharedObjectPoolRefCounter : public RefCounter
        {
        public:

            /**
             * \brief Constructor
             *
             * Creates a new instance of ref counter with a managed object in internal storage
             *
             * \tparam TArgs Constructor argument types
             *
             * \param poolState State this control block returns its own slot to, held as a
             *                  strong reference so that the object may outlive its pool
             * \param args Arguments forwarded to the object's constructor
             */
            template <typename... TArgs>
            SharedObjectPoolRefCounter(SharedPtr<AbstractObjectPoolState<SharedObjectPoolRefCounter>> poolState, TArgs&&... args) :
                mPoolState(Move(poolState))
            {
                ObjectMemoryUtils::Construct(reinterpret_cast<T*>(&mStorage), Forward<TArgs>(args)...);
            }

            /**
             * \brief Returns a pointer to the embedded object
             *
             * \return Pointer to the managed object
             */
            T* GetPointer()
            {
                return reinterpret_cast<T*>(&mStorage);
            }

        protected:

            void Destroy() override
            {
                ObjectMemoryUtils::Destroy(GetPointer());
            }

            /**
             * \brief Returns this control block, and the object it carries, to the pool
             *
             * The reference on the pool state is moved out to a local first, and that is not
             * a detail. Recycle destroys this control block before it is done with the state,
             * and mPoolState is the last reference to that state once the pool itself is
             * gone. Left in place, it would take the state down in the middle of the call
             * that is still using it, freeing the very page this block sits in. The local
             * keeps the state alive until Recycle has returned and the slot is back.
             */
            void Delete() override
            {
                SharedPtr<AbstractObjectPoolState<SharedObjectPoolRefCounter>> poolState = Move(mPoolState);

                poolState->Recycle(this);
            }

        private:

            alignas(T) Byte mStorage[sizeof(T)];
            SharedPtr<AbstractObjectPoolState<SharedObjectPoolRefCounter>> mPoolState; /*!< Pool state the object is returned to */
        };

        /**
         * \class ObjectPoolBase
         *
         * \brief Fixed type object pool handing out exclusive ownership
         *
         * Recycles the storage of same typed objects instead of allocating one block per
         * object. Objects are constructed in place inside page allocations and their
         * storage returns to a free list on destruction.
         *
         * Objects are handed out as ObjectPoolUniquePtr, whose deleter recycles them, so
         * lifetime is managed the same way as with MakeUnique. They may outlive the pool
         * instance itself. Use SharedObjectPoolBase when an object needs more than one owner;
         * wrapping one of these handles in a SharedPtr would allocate a control block per
         * object and give back the very allocation the pool exists to avoid.
         *
         * The pool is neither copyable nor movable: its identity is shared with every live
         * object through the deleters.
         *
         * \tparam T Type of the pooled objects
         * \tparam TLockable Lock type used to serialize access, must satisfy Lockable
         *
         * \remark Use the ObjectPool, SpinObjectPool and TlsObjectPool aliases rather than
         *         instantiating this template directly.
         */
        template <typename T, Lockable TLockable>
        class ObjectPoolBase
        {
        public:

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
                mState = MakeShared<ObjectPoolState<T, TLockable>>(preAllocated);
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
            UniquePtr<T, ObjectPoolDeleter<T>> Allocate(Args&&... args)
            {
                return UniquePtr(mState->Allocate(Forward<Args>(args)...), ObjectPoolDeleter<T>{ mState });
            }

            /**
             * \brief Reads what the pool currently holds
             *
             * \return A snapshot whose fields are consistent with one another
             *
             * \remark AllocatedCount is the number of live objects here, a slot being handed
             *         out for exactly as long as the handle over it lives.
             */
            ObjectPoolStatistics GetStatistics() const
            {
                return mState->GetStatistics();
            }

        private:

            SharedPtr<ObjectPoolState<T, TLockable>> mState; /*!< Storage shared with the deleters of live objects */
        };

        /**
         * \class SharedObjectPoolBase
         *
         * \brief Fixed type object pool handing out shared ownership
         *
         * The counterpart of ObjectPoolBase for objects that need more than one owner. It
         * pools SharedObjectPoolRefCounter<T> rather than T, so a slot carries the reference
         * counts along with the object and a shared handle costs no allocation beyond the
         * pooled slot itself.
         *
         * Objects may outlive the pool: each control block holds a reference on the pool
         * state, which therefore survives until the last object has been returned.
         *
         * The pool is neither copyable nor movable, its identity being shared with every
         * live object.
         *
         * \tparam T Type of the pooled objects
         * \tparam TLockable Lock type used to serialize access, must satisfy Lockable
         *
         * \remark Use the SharedObjectPool, SpinSharedObjectPool and TlsSharedObjectPool
         *         aliases rather than instantiating this template directly.
         *
         * \remark A slot is held for as long as a WeakPtr to its object exists, not only for
         *         as long as the object does. Prefer ObjectPoolBase when single ownership is
         *         enough: its slots come back as soon as the handle goes.
         */
        template <typename T, Lockable TLockable>
        class SharedObjectPoolBase
        {
        public:

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
            explicit SharedObjectPoolBase(std::size_t preAllocated = 0)
            {
                mState = MakeShared<ObjectPoolState<SharedObjectPoolRefCounter<T>, TLockable>>(preAllocated);
            }

            /**
             * \brief Deleted copy constructor
             */
            SharedObjectPoolBase(const SharedObjectPoolBase& other) = delete;

            /**
             * \brief Deleted move constructor
             */
            SharedObjectPoolBase(SharedObjectPoolBase&& other) noexcept = delete;

            /**
             * \brief Deleted copy assignment operator
             */
            SharedObjectPoolBase& operator=(const SharedObjectPoolBase& other) = delete;

            /**
             * \brief Deleted move assignment operator
             */
            SharedObjectPoolBase& operator=(SharedObjectPoolBase&& other) noexcept = delete;

            /**
             * \brief Allocates a pooled object under shared ownership
             *
             * \tparam TArgs Types of the arguments forwarded to T's constructor
             *
             * \param args Arguments forwarded to T's constructor
             *
             * \return Shared handle returning the slot once the last reference to the object,
             *         weak ones included, has gone
             *
             * \throw InvalidParameterException If the pool would end up describing more slots
             *                                  than it can
             * \throw OutOfMemory If the pool had to grow and could not allocate
             */
            template <typename... TArgs>
            SharedPtr<T> Allocate(TArgs&&... args)
            {
                SharedObjectPoolRefCounter<T>* refCounter = mState->Allocate(mState, Forward<TArgs>(args)...);
                return SharedPtr(WithPointerAndControl, refCounter->GetPointer(), refCounter, true);
            }

            /**
             * \brief Reads what the pool currently holds
             *
             * \return A snapshot whose fields are consistent with one another
             *
             * \warning AllocatedCount is not the number of live objects here. A slot is held
             *          until the last reference to its object goes, weak ones included, so a
             *          forgotten WeakPtr shows up as a slot that never comes back while the
             *          object it refers to is long gone. That gap is the reason to watch
             *          these numbers on a shared pool.
             *
             * \remark ByteCount covers the whole slot, so it counts the reference counts
             *         alongside the object. It is the memory actually held, which is larger
             *         than SlotCount * sizeof(T).
             */
            ObjectPoolStatistics GetStatistics() const
            {
                return mState->GetStatistics();
            }

        private:

            SharedPtr<ObjectPoolState<SharedObjectPoolRefCounter<T>, TLockable>> mState; /*!< Storage shared with the deleters of live objects */
        };
    }

    /**
     * \brief Exclusive ownership handle over a pooled object
     *
     * What every ObjectPool hands out. The lock the pool serializes with is deliberately
     * absent from the type: a handle from an ObjectPool, a SpinObjectPool and a
     * TlsObjectPool of the same T are one and the same type, and each returns its object to
     * the pool it actually came from. Code that stores or passes pooled objects around names
     * this and nothing about how the pool is synchronized.
     *
     * \tparam T Type of the pooled objects
     */
    template <typename T>
    using ObjectPoolUniquePtr = UniquePtr<T, Detail::ObjectPoolDeleter<T>>;

    /**
     * \brief Thread safe object pool
     *
     * The default choice for a pool reachable from several threads. Serializes access with
     * a blocking mutex, so a thread that finds the pool busy sleeps rather than spins.
     *
     * \tparam T Type of the pooled objects
     */
    template <typename T>
    using ObjectPool = Detail::ObjectPoolBase<T, Mutex>;

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

    /**
     * \brief Thread safe object pool handing out shared ownership
     *
     * The shared counterpart of ObjectPool, and the default choice for pooled objects with
     * more than one owner. Serializes access with a blocking mutex.
     *
     * \tparam T Type of the pooled objects
     */
    template <typename T>
    using SharedObjectPool = Detail::SharedObjectPoolBase<T, Mutex>;

    /**
     * \brief Shared ownership object pool serialized by a spin lock
     *
     * The shared counterpart of SpinObjectPool.
     *
     * \tparam T Type of the pooled objects
     */
    template <typename T>
    using SpinSharedObjectPool = Detail::SharedObjectPoolBase<T, SpinMutex>;

    /**
     * \brief Shared ownership object pool without synchronization
     *
     * The shared counterpart of TlsObjectPool, with the same confinement requirement: the
     * pool and every object it hands out belong to one thread.
     *
     * \tparam T Type of the pooled objects
     *
     * \warning Shared ownership makes that requirement easier to break than it is with
     *          TlsObjectPool. A SharedPtr copied onto another thread carries the right to
     *          release the object, and releasing it there recycles into an unsynchronized
     *          pool. The handle type says nothing about this, so it is on the caller to keep
     *          the last reference on the owning thread.
     */
    template <typename T>
    using TlsSharedObjectPool = Detail::SharedObjectPoolBase<T, NullMutex>;
}

#endif // COCKTAILENGINE_CORE_UTILITY_OBJECTPOOL_HPP
