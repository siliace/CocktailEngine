#ifndef COCKTAIL_CORE_MEMORY_UNIQUEPTR_HPP
#define COCKTAIL_CORE_MEMORY_UNIQUEPTR_HPP

#include <Cocktail/Core/Cocktail.hpp>

namespace Ck
{
    template <typename T>
    struct DefaultDeleter
    {
        DefaultDeleter() = default;
        DefaultDeleter(const DefaultDeleter& other) = default;
        DefaultDeleter(DefaultDeleter&& other) = default;
        DefaultDeleter& operator=(const DefaultDeleter& other) = default;
        DefaultDeleter& operator=(DefaultDeleter&& other) = default;

        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        DefaultDeleter(const DefaultDeleter<U>&)
        {
        }

        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        DefaultDeleter& operator=(const DefaultDeleter<U>&)
        {
            return *this;
        }

        void operator()(T* ptr) const noexcept
        {
            delete ptr;
        }
    };

    template <typename T>
    struct DefaultDeleter<T[]>
    {
        DefaultDeleter() = default;
        DefaultDeleter(const DefaultDeleter& other) = default;
        DefaultDeleter(DefaultDeleter&& other) = default;
        DefaultDeleter& operator=(const DefaultDeleter& other) = default;
        DefaultDeleter& operator=(DefaultDeleter&& other) = default;

        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U (*)[], T (*)[]>>>
        DefaultDeleter(const DefaultDeleter<U[]>&)
        {
        }

        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U (*)[], T (*)[]>>>
        DefaultDeleter& operator=(const DefaultDeleter<U[]>&)
        {
            return *this;
        }

        void operator()(T* ptr) const noexcept
        {
            delete[] ptr;
        }
    };

    template <typename T, typename TAllocator>
    class AllocatorAwareDeleter
    {
    public:

        AllocatorAwareDeleter() :
            mAllocator(nullptr)
        {
            /// Nothing
        }

        explicit AllocatorAwareDeleter(TAllocator& allocator) :
            mAllocator(&allocator)
        {
            /// Nothing
        }

        void operator()(T* ptr) noexcept
        {
            assert(mAllocator || !ptr);
            mAllocator->Deallocate(ptr);
        }

    private:

        TAllocator* mAllocator;
    };

    /**
     * \brief Smart pointer that ensures unique ownership of a dynamically allocated object
     *
     * \tparam T Type of the managed object
     *
     * \tparam TDeleter Deleter type used to destroy the managed object
     */
    template <typename T, typename TDeleter = DefaultDeleter<T>>
    class UniquePtr
    {
        template <typename TOther, typename TOtherDeleter>
        friend class UniquePtr;

    public:

        using ElementType = T;

        /**
         * \brief Default constructor creating an empty UniquePtr
         */
        UniquePtr() :
            mPtr(nullptr),
            mDeleter()
        {
            /// Nothing
        }

        /**
         * \brief Constructs a UniquePtr representing nullptr
         */
        UniquePtr(std::nullptr_t) :
            mPtr(nullptr),
            mDeleter()
        {
            /// Nothing
        }

        /**
         * \brief Constructs a UniquePtr from a raw pointer using the default deleter
         *
         * \param ptr Raw pointer to manage
         */
        explicit UniquePtr(T* ptr) :
            UniquePtr(ptr, TDeleter())
        {
            /// Nothing
        }

        /**
         * \brief Constructs a UniquePtr from a raw pointer and custom deleter
         *
         * \param ptr Raw pointer to manage
         * \param deleter Custom deleter instance
         */
        UniquePtr(T* ptr, TDeleter deleter) :
            mPtr(ptr),
            mDeleter(std::move(deleter))
        {
            /// Nothing
        }

        /**
         * \brief Constructs a null UniquePtr with a custom deleter
         *
         * \param deleter Custom deleter instance
         */
        UniquePtr(std::nullptr_t, TDeleter deleter) :
            mPtr(nullptr),
            mDeleter(std::move(deleter))
        {
            /// Nothing
        }

        /**
         * \brief Deleted copy constructor
         */
        UniquePtr(const UniquePtr&) = delete;

        /**
         * \brief Move constructor transferring ownership
         *
         * \param other UniquePtr to move from
         */
        UniquePtr(UniquePtr&& other) noexcept :
            mPtr(other.mPtr),
            mDeleter(std::move(other.GetDeleter()))
        {
            other.mPtr = nullptr;
        }

        /**
         * \brief Templated move constructor enabling upcasting between pointer types
         *
         * \tparam TOther Source pointed type
         * \tparam TOtherDeleter Deleter type of the source pointer
         *
         * \param other UniquePtr to move from
         */
        template <typename TOther, typename TOtherDeleter, typename = std::enable_if_t<!std::is_array_v<TOther> && std::is_base_of_v<T, TOther>>>
        UniquePtr(UniquePtr<TOther, TOtherDeleter>&& other) noexcept :
            mPtr(other.mPtr),
            mDeleter(std::move(other.GetDeleter()))
        {
            other.mPtr = nullptr;
        }

        /**
         * \brief Destructor
         *
         * Free the underlying pointer
         */
        ~UniquePtr()
        {
            Reset();
        }

        /**
         * \brief Deleted copy assignment operator
         */
        UniquePtr& operator=(const UniquePtr&) = delete;

        /**
         * \brief Move assignment operator transferring ownership
         *
         * \param other UniquePtr to move from
         *
         * \return Reference to this
         */
        UniquePtr& operator=(UniquePtr&& other) noexcept
        {
            if (this != &other)
            {
                Reset();

                mPtr = other.mPtr;
                mDeleter = std::move(other.mDeleter);

                other.mPtr = nullptr;
            }

            return *this;
        }

        /**
         * \brief Templated move assignment allowing upcasting
         *
         * \tparam TOther Source type
         * \tparam TOtherDeleter Source deleter type
         *
         * \param other UniquePtr to move from
         *
         * \return Reference to this
         */
        template <typename TOther, typename TOtherDeleter, typename = std::enable_if_t<!std::is_array_v<TOther> && std::is_base_of_v<T, TOther>>>
        UniquePtr& operator=(UniquePtr<TOther, TOtherDeleter>&& other) noexcept
        {
            Reset();

            mPtr = other.mPtr;
            mDeleter = std::move(other.mDeleter);

            other.mPtr = nullptr;

            return *this;
        }

        /**
         * \brief Checks whether the stored pointer is non-null
         *
         * \return True if the pointer is valid
         */
        bool IsValid() const
        {
            return mPtr != nullptr;
        }

        /**
         * \brief Replaces the managed object with another raw pointer
         *
         * \param ptr New pointer to manage
         */
        void Reset(T* ptr = nullptr)
        {
            if (mPtr && mPtr != ptr)
                mDeleter(mPtr);

            mPtr = ptr;
        }

        /**
         * \brief Releases ownership of the managed pointer without deleting it
         *
         * \return Raw pointer formerly owned
         */
        T* Release()
        {
            T* ptr = mPtr;
            mPtr = nullptr;

            return ptr;
        }

        /**
         * \brief Returns the stored raw pointer
         *
         * \return Managed pointer
         */
        T* Get() const
        {
            return mPtr;
        }

        /**
         * \brief Returns the stored deleter
         *
         * \return Constant reference to the deleter
         */
        const TDeleter& GetDeleter() const
        {
            return mDeleter;
        }

        /**
         * \brief Equality comparison between two UniquePtr instances
         *
         * \param rhs Other UniquePtr
         *
         * \return True if both store the same pointer
         */
        bool operator==(const UniquePtr& rhs) const
        {
            return mPtr == rhs.mPtr;
        }

        /**
         * \brief Checks if the pointer equals null
         *
         * \return True if pointer is null
         */
        bool operator==(std::nullptr_t) const
        {
            return !IsValid();
        }

        /**
         * \brief Inequality comparison
         *
         * \param rhs Other pointer
         *
         * \return True if pointers differ
         */
        bool operator!=(const UniquePtr& rhs) const
        {
            return !(*this == rhs);
        }

        /**
         * \brief Checks inequality with null
         *
         * \return True if pointer is valid
         */
        bool operator!=(std::nullptr_t rhs) const
        {
            return !(*this == rhs);
        }

        /**
         * \brief Arrow operator providing access to members of the managed object
         *
         * \return Raw pointer
         */
        T* operator->() const
        {
            return Get();
        }

        /**
         * \brief Dereference operator returning a reference to the managed object
         *
         * \return Reference to pointed object
         */
        T& operator*() const
        {
            return *Get();
        }

        /**
         * \brief Conversion to bool indicating whether the pointer is valid
         */
        explicit operator bool() const
        {
            return IsValid();
        }

    private:

        T* mPtr;
        TDeleter mDeleter;
    };

    /**
     * \brief Partial specialization of UniquePtr for array types
     *
     * \tparam T Array element type
     * \tparam TDeleter Deleter type
     */
    template <typename T, typename TDeleter>
    class UniquePtr<T[], TDeleter>
    {
        template <typename TOther, typename TOtherDeleter>
        friend class UniquePtr;

    public:

        using ElementType = T;

        /**
         * \brief Default constructor creating an empty UniquePtr
         */
        UniquePtr() :
            mPtr(nullptr),
            mDeleter()
        {
            /// Nothing
        }

        /**
         * \brief Constructs a null UniquePtr
         */
        UniquePtr(std::nullptr_t) :
            mPtr(nullptr),
            mDeleter()
        {
            /// Nothing
        }

        /**
         * \brief Constructs a UniquePtr from a raw array pointer using the default deleter
         *
         * \param ptr Pointer to an array
         */
        explicit UniquePtr(T* ptr) :
            UniquePtr(ptr, TDeleter())
        {
            /// Nothing
        }

        /**
         * \brief Constructs a UniquePtr from a raw array pointer with a custom deleter
         *
         * \param ptr Array pointer to manage
         * \param deleter Custom deleter
         */
        UniquePtr(T* ptr, TDeleter deleter) :
            mPtr(ptr),
            mDeleter(std::move(deleter))
        {
            /// Nothing
        }

        /**
         * \brief Constructs a null UniquePtr with a custom deleter
         *
         * \param deleter Custom deleter
         */
        UniquePtr(std::nullptr_t, TDeleter deleter) :
            mPtr(nullptr),
            mDeleter(std::move(deleter))
        {
            /// Nothing
        }

        /**
         * \brief Deleted copy constructor since UniquePtr enforces unique ownership
         */
        UniquePtr(const UniquePtr&) = delete;

        /**
         * \brief Move constructor transferring ownership of an array pointer
         *
         * \param other UniquePtr to move from
         */
        UniquePtr(UniquePtr&& other) noexcept :
            mPtr(other.mPtr),
            mDeleter(std::move(other.GetDeleter()))
        {
            other.mPtr = nullptr;
        }

        /**
         * \brief Templated move constructor for compatible array types
         *
         * \tparam TOther Other array type
         * \tparam TOtherDeleter Other deleter type
         *
         * \param other UniquePtr to move from
         */
        template <typename TOther, typename TOtherDeleter, typename = std::enable_if_t<std::is_array_v<TOther> && std::is_base_of_v<T, TOther>>>
        UniquePtr(UniquePtr<TOther, TOtherDeleter>&& other) noexcept :
            mPtr(other.mPtr),
            mDeleter(std::move(other.GetDeleter()))
        {
            other.mPtr = nullptr;
        }

        /**
         * \brief Destructor
         *
         * Free the underlying pointer
         */
        ~UniquePtr()
        {
            Reset();
        }

        /**
         * \brief Deleted copy assignment operator
         */
        UniquePtr& operator=(const UniquePtr&) = delete;

        /**
         * \brief Move assignment operator transferring ownership
         *
         * \param other UniquePtr to move from
         *
         * \return Reference to this
         */
        UniquePtr& operator=(UniquePtr&& other) noexcept
        {
            if (this != &other)
            {
                Reset();

                mPtr = other.mPtr;
                mDeleter = std::move(other.mDeleter);

                other.mPtr = nullptr;
            }

            return *this;
        }

        /**
         * \brief Templated move assignment for compatible array types
         *
         * \param other UniquePtr to move from
         *
         * \return Reference to this
         */
        template <typename TOther, typename TOtherDeleter, typename = std::enable_if_t<std::is_array_v<TOther> && std::is_base_of_v<T, TOther>>>
        UniquePtr& operator=(UniquePtr<TOther, TOtherDeleter>&& other) noexcept
        {
            Reset();

            mPtr = other.mPtr;
            mDeleter = std::move(other.mDeleter);

            other.mPtr = nullptr;

            return *this;
        }

        /**
         * \brief Checks whether the pointer is valid
         *
         * \return True if pointer is non-null
         */
        bool IsValid() const
        {
            return mPtr != nullptr;
        }

        /**
         * \brief Replaces the managed object with another raw pointer
         *
         * \param ptr New pointer to manage
         */
        void Reset(T* ptr = nullptr)
        {
            if (mPtr && mPtr != ptr)
                mDeleter(mPtr);

            mPtr = ptr;
        }

        /**
         * \brief Releases ownership of the managed array pointer without deleting it
         *
         * \return Raw array pointer
         */
        T* Release()
        {
            T* ptr = mPtr;
            mPtr = nullptr;

            return ptr;
        }

        /**
         * \brief Returns the stored array pointer
         *
         * \return Managed pointer
         */
        T* Get() const
        {
            return mPtr;
        }

        /**
         * \brief Returns the stored deleter
         *
         * \return Constant reference to deleter
         */
        const TDeleter& GetDeleter() const
        {
            return mDeleter;
        }

        /**
         * \brief Equality comparison between two UniquePtr instances
         *
         * \param rhs Other pointer
         *
         * \return True if both store the same pointer
         */
        bool operator==(const UniquePtr& rhs) const
        {
            return mPtr == rhs.mPtr;
        }

        /**
         * \brief Checks if pointer equals null
         */
        bool operator==(std::nullptr_t) const
        {
            return !IsValid();
        }

        /**
         * \brief Inequality comparison
         *
         * \param rhs Other pointer
         *
         * \return True if pointers differ
         */
        bool operator!=(const UniquePtr& rhs) const
        {
            return !(*this == rhs);
        }

        /**
         * \brief Checks inequality with null
         *
         * \return True if pointer is non-null
         */
        bool operator!=(std::nullptr_t rhs) const
        {
            return !(*this == rhs);
        }

        /**
         * \brief Provides indexed access to elements of the managed array
         *
         * \param index Element index
         *
         * \return Reference to array element
         */
        T& operator[](Uint64 index) const
        {
            return mPtr[index];
        }

        /**
         * \brief Conversion to bool indicating whether pointer is valid
         */
        explicit operator bool() const
        {
            return IsValid();
        }

    private:

        T* mPtr;
        TDeleter mDeleter;
    };

    /**
     * \brief Creates a UniquePtr for non array types
     *
     * \tparam T Object type
     * \tparam TArgs Constructor argument types
     *
     * \param args Arguments forwarded to constructor
     *
     * \return New UniquePtr managing the object
     */
    template <typename T, typename... TArgs, typename = std::enable_if_t<!std::is_array_v<T>>>
    UniquePtr<T> MakeUnique(TArgs&&... args)
    {
        return UniquePtr<T>(new T(std::forward<TArgs>(args)...));
    }

    /**
     * \brief Creates a UniquePtr for non-array types using a custom allocator
     *
     * Allocates memory using the provided allocator and constructs the object
     * in-place. The returned UniquePtr uses an allocator-aware deleter to ensure
     * proper destruction and deallocation.
     *
     * \tparam T Object type
     * \tparam TAllocator Allocator type
     * \tparam TArgs Constructor argument types
     *
     * \param allocator Allocator used to allocate and deallocate memory
     * \param args Arguments forwarded to the constructor of T
     *
     * \return A new UniquePtr managing the constructed object
     */
    template <typename T, typename TAllocator, typename... TArgs, typename = std::enable_if_t<!std::is_array_v<T>>>
    UniquePtr<T, AllocatorAwareDeleter<T, TAllocator>> MakeUniqueWithAllocator(TAllocator& allocator, TArgs&&... args)
    {
        T* pointer = allocator.Allocate(1);
        return UniquePtr<T, AllocatorAwareDeleter<T, TAllocator>>(
            new (pointer) T(std::forward<TArgs>(args)...),
            AllocatorAwareDeleter<T, TAllocator>(allocator)
        );
    }

    /**
     * \brief Creates a UniquePtr for array types
     *
     * \tparam T Array type
     *
     * \param size Number of elements
     *
     * \return New UniquePtr managing a dynamically allocated array
     */
    template <typename T, typename = std::enable_if_t<std::is_array_v<T>>>
    UniquePtr<T> MakeUnique(Uint64 size)
    {
        using ElementType = std::remove_extent_t<T>;
        return UniquePtr<T>(new ElementType[size]());
    }

    /**
     * \brief Creates a UniquePtr for array types using a custom allocator
     *
     * Allocates memory for an array using the provided allocator and constructs
     * each element using its default constructor. The returned UniquePtr uses
     * an allocator-aware deleter to properly destroy and deallocate the array.
     *
     * \tparam T Array type
     * \tparam TAllocator Allocator type
     *
     * \param allocator Allocator used to allocate and deallocate memory
     * \param size Number of elements to allocate
     *
     * \return A new UniquePtr managing the allocated array
     */
    template <typename T, typename TAllocator, typename = std::enable_if_t<std::is_array_v<T>>>
    UniquePtr<T, AllocatorAwareDeleter<std::remove_extent_t<T>, TAllocator>> MakeUniqueWithAllocator(TAllocator& allocator, Uint64 size)
    {
        using BaseType = std::remove_extent_t<T>;

        BaseType* pointer = allocator.Allocate(size);
        for (Uint64 i = 0; i < size; i++)
            new (&pointer[i]) BaseType();

        return UniquePtr<T, AllocatorAwareDeleter<BaseType, TAllocator>>(
            pointer,
            AllocatorAwareDeleter<BaseType, TAllocator>(allocator)
        );
    }
}

#endif // COCKTAIL_CORE_MEMORY_UNIQUEPTR_HPP
