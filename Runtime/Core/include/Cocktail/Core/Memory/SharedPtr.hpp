#ifndef COCKTAIL_CORE_MEMORY_SHAREDPTR_HPP
#define COCKTAIL_CORE_MEMORY_SHAREDPTR_HPP

#include <atomic>

#include <Cocktail/Core/Memory/ObjectMemoryUtils.hpp>
#include <Cocktail/Core/Memory/UniquePtr.hpp>

namespace Ck
{
    template <typename>
    class SharedPtr;

    namespace Detail
    {
        /**
         * \brief Helper tag used to call the right constructor from MakeShared
         */
        COCKTAIL_DECLARE_TAG(WithPointerAndControl);

        /**
         * \brief Base class implementing strong and weak reference counting
         *
         * This class manages the lifetime of an object through a strong reference
         * count and the lifetime of the control block itself through a weak
         * reference count. Derived classes define how the managed object and the
         * control block are destroyed.
         */
        class RefCounter
        {
        public:

            /**
             * \brief Default destructor
             */
            virtual ~RefCounter() = default;

            /**
             * \brief Increments the strong reference count
             */
            void Acquire()
            {
                mStrongCounter.fetch_add(1, std::memory_order_relaxed);
            }

            /**
             * \brief Attempts to increment the strong reference count if the object is still alive
             *
             * This operation is used to safely promote a weak reference to a strong reference
             * in a thread-safe manner. It only succeeds if the strong reference count is not
             * zero at the time of the operation.
             *
             * \return `true` if the strong reference was successfully acquired, `false` otherwise
             */
            bool TryAcquire()
            {
                Uint64 current = mStrongCounter.load(std::memory_order_relaxed);

                while (current != 0)
                {
                    if (mStrongCounter.compare_exchange_weak(current, current + 1, std::memory_order_acquire, std::memory_order_relaxed))
                        return true;
                }

                return false;
            }

            /**
             * \brief Increments the weak reference count
             */
            void WeakAcquire()
            {
                mWeakCounter.fetch_add(1, std::memory_order_relaxed);
            }

            /**
             * \brief Decrements the strong reference count
             *
             * When the last strong reference is released, the managed object is
             * destroyed and the implicit weak reference owned by the control block
             * is released.
             */
            void Release()
            {
                if (mStrongCounter.fetch_sub(1, std::memory_order_acq_rel) == 1)
                {
                    Destroy();
                    WeakRelease();
                }
            }

            /**
             * \brief Decrements the weak reference count
             *
             * Deletes the control block when no weak references remain
             */
            void WeakRelease()
            {
                if (mWeakCounter.fetch_sub(1, std::memory_order_acq_rel) == 1)
                    Delete();
            }

            /**
             * \brief Returns the current number of strong references
             *
             * \return The strong reference count
             */
            Uint64 GetCounter() const
            {
                return mStrongCounter.load(std::memory_order_relaxed);
            }

            /**
             * \brief Returns the current number of weak references
             *
             * \return The weak reference count
             */
            Uint64 GetWeakCounter() const
            {
                return mWeakCounter.load(std::memory_order_relaxed);
            }

        protected:

            /**
             * \brief Default constructor
             *
             * Creates a new instance of RefCounter with one strong and one weak reference.
             */
            RefCounter() :
                mStrongCounter(1),
                mWeakCounter(1)
            {
                /// Nothing
            }

            /**
             * \brief Destroys the managed object
             */
            virtual void Destroy() = 0;

            /**
             * \brief Deletes the control block itself
             */
            virtual void Delete() = 0;

        private:

            std::atomic<Uint64> mStrongCounter;
            std::atomic<Uint64> mWeakCounter;
        };

        /**
         * \brief Reference counter for externally allocated objects
         *
         * \tparam T Managed object type
         * \tparam TDeleter Deleter type
         */
        template <typename T, typename TDeleter = DefaultDeleter<T>>
        class RemoteRefCounter : public RefCounter
        {
        public:

            /**
             * \brief Constructor
             *
             * Creates a new instance of control block for an existing object
             *
             * \param remote Pointer to the managed object
             * \param deleter Deleter used to destroy the object
             */
            RemoteRefCounter(T* remote, const TDeleter& deleter = {}) :
                mRemote(remote),
                mDeleter(deleter)
            {
                assert(mRemote);
            }

        protected:

            /**
             * \brief Destroys the managed object
             */
            void Destroy() override
            {
                mDeleter(mRemote);
            }

            /**
             * \brief Deletes the control block itself
             */
            void Delete() override
            {
                delete this;
            }

        private:

            T* mRemote;
            TDeleter mDeleter;
        };

        /**
         * \brief Reference counter storing the managed object in-place
         *
         * This control block embeds the object storage to avoid a separate
         * allocation.
         *
         * \tparam T Managed object type
         */
        template <typename T>
        class MakeSharedRefCounter : public RefCounter
        {
        public:

            /**
             * \brief Constructor
             *
             * Creates a new instance of ref counter with a managed object in internal storage
             *
             * \tparam TArgs Constructor argument types
             *
             * \param args Arguments forwarded to the object's constructor
             */
            template <typename... TArgs>
            MakeSharedRefCounter(TArgs&&... args)
            {
                ObjectMemoryUtils::Construct(reinterpret_cast<T*>(&mStorage), std::forward<TArgs>(args)...);
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

            /**
             * \brief Destroys the managed object
             */
            void Destroy() override
            {
                ObjectMemoryUtils::Destroy(GetPointer());
            }

            /**
             * \brief Deletes the control block itself
             */
            void Delete() override
            {
                delete this;
            }

        private:

            using StorageType = std::aligned_storage_t<sizeof(T), alignof(T)>;

            StorageType mStorage;
        };

        /**
         * \brief Base class used to detect SharedFromThis support
         *
         * Types deriving from this class can be automatically initialized by
         * SharedPtr to provide shared and weak references to themselves.
         */
        class SharedFromThisBase
        {
        public:

            /**
             * \brief Default destructor
             */
            virtual ~SharedFromThisBase() = default;

        protected:

            template <typename>
            friend class Ck::SharedPtr;

            /**
             * \brief Initializes the internal weak reference used for SharedFromThis support
             *
             * This method is called by SharedPtr when it first takes ownership of an object
             * deriving from SharedFromThis. It stores a weak reference to the same control
             * block so that the object can later safely create SharedPtr or WeakPtr to itself.
             *
             * The operation is intended to be called exactly once for a given object lifetime.
             * Reinitializing the weak reference is not expected and may lead to undefined behavior.
             *
             * \param pointer Pointer to the managed object instance
             * \param refCounter Control block associated with the owning SharedPtr
             */
            virtual void InitWeakThis(void* pointer, RefCounter* refCounter) = 0;
        };
    }

    /**
     * \brief Smart pointer implementing shared ownership semantics
     *
     * Multiple SharedPtr instances may own the same object through a shared
     * reference counter. The managed object is destroyed automatically when the
     * last owning instance is released.
     *
     * \tparam T Managed object type
     */
    template <typename T>
    class SharedPtr
    {
    public:

        /**
         * \brief Default constructor
         *
         * Creates a new empty instance of SharedPtr.
         */
        constexpr SharedPtr() :
            mPointer(nullptr),
            mRefCounter(nullptr)
        {
            /// Nothing
        }

        /**
         * \brief Constructor
         *
         * Creates a new empty instance of SharedPtr.
         */
        constexpr SharedPtr(std::nullptr_t) :
            mPointer(nullptr),
            mRefCounter(nullptr)
        {
            /// Nothing
        }

        /**
         * \brief Constructor
         *
         * Creates a new instance of SharedPtr by taking ownership of a raw pointer.
         *
         * \tparam U Source pointer type
         *
         * \param pointer Pointer to manage
         */
        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        explicit SharedPtr(U* pointer) :
            mPointer(nullptr),
            mRefCounter(nullptr)
        {
            UniquePtr<U> unique(pointer);
            if (unique)
                mRefCounter = new Detail::RemoteRefCounter<U>(unique.Get());

            mPointer = unique.Release();

            EnableWeakThis<U>(mPointer, mRefCounter);
        }

        /**
         * \brief Constructor
         *
         * Creates a new instance of SharedPtr by taking ownership from a UniquePtr
         *
         * Transfers ownership of the managed object from the specified UniquePtr
         * to this SharedPtr. A new control block is created and initialized with
         * a single strong reference. After construction, the source UniquePtr no
         * longer owns the object and becomes empty.
         *
         * \tparam U Source object type
         * \tparam TDeleter Deleter type used by the source UniquePtr
         *
         * \param unique Unique pointer to transfer ownership from
         */
        template <typename U, typename TDeleter, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        explicit SharedPtr(UniquePtr<U, TDeleter>&& unique) :
            mPointer(nullptr),
            mRefCounter(nullptr)
        {
            if (unique)
                mRefCounter = new Detail::RemoteRefCounter<U, TDeleter>(unique.Get(), unique.GetDeleter());

            mPointer = unique.Release();

            EnableWeakThis<U>(mPointer, mRefCounter);
        }

        /**
         * \brief Constructor
         *
         * Creates a new instance of SharedPtr by taking ownership of a raw pointer using a custom deleter.
         *
         * \tparam U Source pointer type
         * \tparam TDeleter Deleter type
         *
         * \param pointer Pointer to manage
         * \param deleter Deleter used to destroy the object
         */
        template <typename U, typename TDeleter, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        SharedPtr(U* pointer, TDeleter&& deleter) :
            mPointer(pointer),
            mRefCounter(nullptr)
        {
            UniquePtr<U, std::decay_t<TDeleter>> unique(pointer, std::forward<TDeleter>(deleter));
            if (unique)
                mRefCounter = new Detail::RemoteRefCounter<U, TDeleter>(unique.Get(), unique.GetDeleter());

            mPointer = unique.Release();

            EnableWeakThis<U>(mPointer, mRefCounter);
        }

        /**
         * \brief Copy constructor
         *
         * Creates a new instance of SharedPtr by copying another one.
         * Increments the strong reference count.
         *
         * \param other Shared pointer to copy.
         */
        SharedPtr(const SharedPtr& other) :
            mPointer(nullptr),
            mRefCounter(nullptr)
        {
            Reset(other.mPointer, other.mRefCounter);
        }

        /**
         * \brief Copy constructor
         *
         * Creates a new instance of SharedPtr by copying another one.
         * Increments the strong reference count.
         *
         * \tparam U Source pointer type
         *
         * \param other Shared pointer to copy
         */
        template <typename U = T, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        SharedPtr(const SharedPtr<U>& other) :
            mPointer(nullptr),
            mRefCounter(nullptr)
        {
            Reset(other.mPointer, other.mRefCounter);
        }

        /**
         * \brief Move constructor
         *
         * Creates a new instance of SharedPtr by taking ownership on another one.
         * Transfers ownership without modifying the reference count.
         *
         * \param other Shared pointer to copy.
         */
        SharedPtr(SharedPtr&& other) noexcept :
            mPointer(std::exchange(other.mPointer, nullptr)),
            mRefCounter(std::exchange(other.mRefCounter, nullptr))
        {
            /// Nothing
        }

        /**
         * \brief Move constructor
         *
         * Creates a new instance of SharedPtr by taking ownership on another one.
         * Transfers ownership without modifying the reference count.
         *
         * \tparam U Source pointer type
         *
         * \param other Shared pointer to move
         */
        template <typename U = T, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        SharedPtr(SharedPtr<U>&& other) noexcept :
            mPointer(std::exchange(other.mPointer, nullptr)),
            mRefCounter(std::exchange(other.mRefCounter, nullptr))
        {
            /// Nothing
        }

        /**
         * \brief Destructor
         *
         * Releases ownership of the managed object.
         */
        ~SharedPtr()
        {
            Reset(nullptr);
        }

        /**
         * \brief Assigns a null pointer
         *
         * Releases the current reference.
         *
         * \return Reference to this SharedPtr
         */
        SharedPtr& operator=(std::nullptr_t)
        {
            Reset(nullptr);

            return *this;
        }

        /**
         * \brief Copy assignment operator
         *
         * Replaces the managed object by copying another SharedPtr.
         *
         * \param other Shared pointer to copy
         *
         * \return Reference to this SharedPtr
         */
        SharedPtr& operator=(const SharedPtr& other)
        {
            if (this == &other)
                return *this;

            Reset(other.mPointer, other.mRefCounter);

            return *this;
        }

        /**
         * \brief Copy assignment operator
         *
         * Replaces the managed object by copying another SharedPtr.
         *
         * \tparam U Source pointer type
         *
         * \param other Shared pointer to copy
         *
         * \return Reference to this SharedPtr
         */
        template <typename U = T, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        SharedPtr& operator=(const SharedPtr<U>& other)
        {
            if (mRefCounter == other.mRefCounter)
            {
                mPointer = other.mPointer;
                return *this;
            }

            Reset(other.mPointer, other.mRefCounter);

            return *this;
        }

        /**
         * \brief Move assignment operator
         *
         * Replaces the managed object by taking ownership from another SharedPtr.
         *
         * \param other Shared pointer to move
         *
         * \return Reference to this SharedPtr
         */
        SharedPtr& operator=(SharedPtr&& other) noexcept
        {
            if (this == &other)
                return *this;

            Reset(nullptr);

            mPointer = std::exchange(other.mPointer, nullptr);
            mRefCounter = std::exchange(other.mRefCounter, nullptr);

            return *this;
        }

        /**
         * \brief Move assignment operator
         *
         * Replaces the managed object by taking ownership from another SharedPtr.
         *
         * \tparam U Source pointer type
         *
         * \param other Shared pointer to move
         *
         * \return Reference to this SharedPtr
         */
        template <typename U = T, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        SharedPtr& operator=(SharedPtr<U>&& other) noexcept
        {
            Reset(nullptr);

            mPointer = std::exchange(other.mPointer, nullptr);
            mRefCounter = std::exchange(other.mRefCounter, nullptr);

            return *this;
        }

        /**
         * \brief Returns the stored raw pointer
         *
         * \return Pointer to the managed object, or nullptr
         */
        T* Get() const noexcept
        {
            return mPointer;
        }

        /**
         * \brief Releases ownership and resets the pointer to nullptr
         */
        void Reset(std::nullptr_t)
        {
            if (mRefCounter)
                mRefCounter->Release();

            mPointer = nullptr;
            mRefCounter = nullptr;
        }

        /**
         * \brief Creates a SharedPtr with a statically cast pointer type
         *
         * Shares ownership with the current instance.
         *
         * \tparam U Destination type
         *
         * \return A SharedPtr<U> referring to the same managed object
         */
        template <typename U>
        SharedPtr<U> StaticCast() const
        {
            if (mRefCounter)
                mRefCounter->Acquire();

            return SharedPtr<U>(Detail::WithPointerAndControl, static_cast<U*>(mPointer), mRefCounter, false);
        }

        /**
         * \brief Get the number of different instances of SharedPtr (including this) managing the current object
         *
         * \return The use count or 0 if there is no managed object
         */
        Uint64 UseCount() const noexcept
        {
            return mRefCounter ? mRefCounter->GetCounter() : 0;
        }

        /**
         * \brief Returns the stored pointer
         *
         * \return Pointer to the managed object
         */
        T* operator->() const noexcept
        {
            return mPointer;
        }

        /**
         * \brief Dereferences the managed object
         *
         * \return Reference to the managed object
         */
        T& operator*() const noexcept
        {
            return *mPointer;
        }

        /**
         * \brief Tests whether the SharedPtr contains a valid pointer
         *
         * \return `true` if the stored pointer is not null, `false` otherwise
         */
        explicit operator bool() const noexcept
        {
            return mPointer != nullptr;
        }

    private:

        template <typename>
        friend class SharedPtr;

        template <typename>
        friend class WeakPtr;

        template <typename U, typename... TArgs>
        friend SharedPtr<U> MakeShared(TArgs&&...);

        /**
         * \brief Constructor
         *
         * Creates a new SharedPtr from an existing pointer and control block.
         *
         * This constructor assumes that the control block already owns the managed
         * object and does not modify the reference count.
         *
         * \param pointer Managed object pointer
         * \param refCounter Associated control block
         * \param fresh Flag indicating if the pointer is a fresh one that needs to enable its weak this
         */
        SharedPtr(Detail::WithPointerAndControlTag, T* pointer, Detail::RefCounter* refCounter, bool fresh = true) :
            mPointer(pointer),
            mRefCounter(refCounter)
        {
            if (fresh)
                EnableWeakThis<T>(mPointer, mRefCounter);
        }

        /**
         * \brief Replaces the managed object and updates reference counts
         *
         * Releases the current weak reference, stores the specified pointer and
         * control block, and acquires a new reference if needed.
         *
         * \param pointer New managed pointer
         * \param refCounter Associated control block
         */
        void Reset(T* pointer, Detail::RefCounter* refCounter)
        {
            if (refCounter)
                refCounter->Acquire();

            if (mRefCounter)
                mRefCounter->Release();

            mPointer = pointer;
            mRefCounter = refCounter;
        }

        /**
         * \brief Initializes SharedFromThis support for the managed object
         *
         * If the managed object derives from SharedFromThis, its internal weak
         * reference is initialized to reference the current control block. The weak
         * reference is only initialized once.
         */
        template <typename U>
        static void EnableWeakThis(U* pointer, Detail::RefCounter* refCounter)
        {
            if constexpr (std::is_base_of_v<Detail::SharedFromThisBase, U>)
            {
                if (pointer)
                    static_cast<Detail::SharedFromThisBase*>(pointer)->InitWeakThis(pointer, refCounter);
            }
        }

        T* mPointer;
        Detail::RefCounter* mRefCounter;
    };

    template <typename T>
    bool operator==(std::nullptr_t, const SharedPtr<T>& rhs)
    {
        return nullptr == rhs.Get();
    }

    template <typename T>
    bool operator==(const SharedPtr<T>& lhs, std::nullptr_t)
    {
        return lhs.Get() == nullptr;
    }

    template <typename T, typename U>
    bool operator==(const T* lhs, const SharedPtr<U>& rhs)
    {
        return lhs == rhs.Get();
    }

    template <typename T, typename U>
    bool operator==(const SharedPtr<T>& lhs, const U* rhs)
    {
        return lhs.Get() == rhs;
    }

    template <typename T, typename U>
    bool operator==(const SharedPtr<T>& lhs, const SharedPtr<U>& rhs)
    {
        return lhs.Get() == rhs.Get();
    }

    template <typename T>
    bool operator!=(std::nullptr_t, const SharedPtr<T>& rhs)
    {
        return nullptr != rhs.Get();
    }

    template <typename T>
    bool operator!=(const SharedPtr<T>& lhs, std::nullptr_t)
    {
        return lhs.Get() != nullptr;
    }

    template <typename T, typename U>
    bool operator!=(const T* lhs, const SharedPtr<U>& rhs)
    {
        return lhs != rhs.Get();
    }

    template <typename T, typename U>
    bool operator!=(const SharedPtr<T>& lhs, const U* rhs)
    {
        return lhs.Get() != rhs;
    }

    template <typename T, typename U>
    bool operator!=(const SharedPtr<T>& lhs, const SharedPtr<U>& rhs)
    {
        return lhs.Get() != rhs.Get();
    }

    /**
     * \brief Creates a SharedPtr by constructing the object in-place
     *
     * Performs a single allocation for both the control block and the managed
     * object.
     *
     * \tparam T Object type
     * \tparam TArgs Constructor argument types
     *
     * \param args Arguments forwarded to T's constructor
     *
     * \return A SharedPtr owning the newly constructed object
     */
    template <typename T, typename... TArgs>
    SharedPtr<T> MakeShared(TArgs&&... args)
    {
        Detail::MakeSharedRefCounter<T>* refCounter = new Detail::MakeSharedRefCounter<T>(std::forward<TArgs>(args)...);
        return SharedPtr<T>(Detail::WithPointerAndControl, refCounter->GetPointer(), refCounter);
    }
}

namespace std
{
    template <typename T>
    struct hash<Ck::SharedPtr<T>>
    {
        size_t operator()(const Ck::SharedPtr<T>& value) const noexcept
        {
            return hash<T*>()(value.Get());
        }
    };
}

#endif // COCKTAIL_CORE_MEMORY_SHAREDPTR_HPP
