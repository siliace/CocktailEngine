#ifndef COCKTAIL_CORE_MEMORY_WEAKPTR_HPP
#define COCKTAIL_CORE_MEMORY_WEAKPTR_HPP

#include <Cocktail/Core/Memory/SharedPtr.hpp>

namespace Ck
{
    /**
     * \brief Smart pointer implementing non-owning shared references
     *
     * WeakPtr references an object managed by SharedPtr without contributing to
     * its strong reference count. It can be converted back to a SharedPtr as long
     * as the managed object has not been destroyed.
     *
     * \tparam T Managed object type
     */
    template <typename T>
    class WeakPtr
    {
    public:

        /**
         * \brief Default constructor
         *
         * Creates a new empty instance of WeakPtr.
         */
        constexpr WeakPtr() :
            WeakPtr(nullptr, nullptr)
        {
            /// Nothing
        }

        /**
         * \brief Constructor
         *
         * Creates a new empty instance of WeakPtr.
         */
        constexpr WeakPtr(std::nullptr_t) :
            WeakPtr(nullptr, nullptr)
        {
            /// Nothing
        }

        /**
         * \brief Constructor
         *
         * Creates a new WeakPtr from a SharedPtr.
         *
         * \param other Shared pointer to observe
         */
        WeakPtr(const SharedPtr<T>& other) :
            WeakPtr(nullptr, nullptr)
        {
            Reset(other.mPointer, other.mRefCounter);
        }

        /**
         * \brief Constructor
         *
         * Creates a new WeakPtr from a SharedPtr.
         *
         * \tparam U Source pointer type
         *
         * \param other Shared pointer to observe
         */
        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        WeakPtr(const SharedPtr<U>& other) :
            WeakPtr(nullptr, nullptr)
        {
            Reset(other.mPointer, other.mRefCounter);
        }

        /**
         * \brief Copy constructor
         *
         * Creates a new WeakPtr by copying another one.
         *
         * \param rhs Weak pointer to copy
         */
        WeakPtr(const WeakPtr& rhs) :
            WeakPtr(nullptr, nullptr)
        {
            Reset(rhs.mPointer, rhs.mRefCounter);
        }

        /**
         * \brief Copy constructor
         *
         * Creates a new WeakPtr by copying another one.
         *
         * \tparam U Source pointer type
         *
         * \param rhs Weak pointer to copy
         */
        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        WeakPtr(const WeakPtr<U>& rhs) :
            WeakPtr(nullptr, nullptr)
        {
            Reset(rhs.mPointer, rhs.mRefCounter);
        }

        /**
         * \brief Move constructor
         *
         * Creates a new WeakPtr by taking ownership from another one.
         *
         * \param other Weak pointer to move
         */
        WeakPtr(WeakPtr&& other) noexcept :
            WeakPtr(nullptr, nullptr)
        {
            mPointer = std::exchange(other.mPointer, nullptr);
            mRefCounter = std::exchange(other.mRefCounter, nullptr);
        }

        /**
         * \brief Move constructor
         *
         * Creates a new WeakPtr by taking ownership from another one.
         *
         * \tparam U Source pointer type
         *
         * \param other Weak pointer to move
         */
        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        WeakPtr(WeakPtr<U>&& other) noexcept :
            WeakPtr(nullptr, nullptr)
        {
            mPointer = std::exchange(other.mPointer, nullptr);
            mRefCounter = std::exchange(other.mRefCounter, nullptr);
        }

        /**
         * \brief Destructor
         *
         * Releases the weak reference held by this instance.
         */
        ~WeakPtr()
        {
            Reset(nullptr);
        }

        /**
         * \brief Assigns a null pointer
         *
         * Releases the current weak reference.
         *
         * \return Reference to this WeakPtr
         */
        WeakPtr& operator=(std::nullptr_t)
        {
            Reset(nullptr);

            return *this;
        }

        /**
         * \brief Assigns from a SharedPtr
         *
         * \param rhs Shared pointer to observe.
         *
         * \return Reference to this WeakPtr
         */
        WeakPtr& operator=(const SharedPtr<T>& rhs)
        {
            Reset(rhs.mPointer, rhs.mRefCounter);

            return *this;
        }

        /**
         * \brief Assigns from a SharedPtr
         *
         * \tparam U Source pointer type
         *
         * \param other Shared pointer to observe
         *
         * \return Reference to this WeakPtr
         */
        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        WeakPtr& operator=(const SharedPtr<U>& other)
        {
            Reset(other.mPointer, other.mRefCounter);
            return *this;
        }

        /**
         * \brief Copy assignment operator
         *
         * \param other Weak pointer to copy
         *
         * \return Reference to this WeakPtr
         */
        WeakPtr& operator=(const WeakPtr& other)
        {
            if (this == &other)
                return *this;

            Reset(other.mPointer, other.mRefCounter);

            return *this;
        }

        /**
         * \brief Copy assignment operator
         *
         * \tparam U Source pointer type
         *
         * \param other Weak pointer to copy
         *
         * \return Reference to this WeakPtr
         */
        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        WeakPtr& operator=(const WeakPtr<U>& other)
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
         * \param other Weak pointer to move
         *
         * \return Reference to this WeakPtr
         */
        WeakPtr& operator=(WeakPtr&& other) noexcept
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
         * \tparam U Source pointer type
         *
         * \param other Weak pointer to move
         *
         * \return Reference to this WeakPtr
         */
        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        WeakPtr& operator=(WeakPtr<U>&& other) noexcept
        {
            Reset(nullptr);

            mPointer = std::exchange(other.mPointer, nullptr);
            mRefCounter = std::exchange(other.mRefCounter, nullptr);

            return *this;
        }

        /**
         * \brief Releases the current weak reference and resets the pointer to nullptr
         */
        void Reset(std::nullptr_t)
        {
            if (mRefCounter)
                mRefCounter->WeakRelease();

            mPointer = nullptr;
            mRefCounter = nullptr;
        }

        /**
         * \brief Checks whether the managed object has been destroyed
         *
         * \return `true` if the object has expired, `false` otherwise
         */
        bool IsExpired() const noexcept
        {
            return mRefCounter == nullptr || mRefCounter->GetCounter() == 0;
        }

        /**
         * \brief Creates a SharedPtr from this WeakPtr
         *
         * Returns an empty SharedPtr if the managed object has already expired.
         *
         * \return A SharedPtr owning the managed object, or an empty SharedPtr if expired
         */
        SharedPtr<T> ToStrong() const
        {
            if (!mRefCounter)
                return nullptr;

            if (!mRefCounter->TryAcquire())
                return nullptr;

            return SharedPtr<T>(Detail::WithPointerAndControl, mPointer, mRefCounter, false);
        }

    private:

        template <typename>
        friend class SharedFromThis;

        template <typename>
        friend class SharedPtr;

        template <typename>
        friend class WeakPtr;

        /**
         * \brief Constructor
         *
         * Creates a WeakPtr from an existing pointer and control block
         *
         * \param pointer Managed object pointer
         * \param refCounter Associated control block
         */
        WeakPtr(T* pointer, Detail::RefCounter* refCounter) :
            mPointer(pointer),
            mRefCounter(refCounter)
        {
            /// Nothing
        }

        /**
         * \brief Replaces the observed object and updates weak reference counts
         *
         * Releases the current weak reference, stores the specified pointer and
         * control block, and acquires a new weak reference if needed.
         *
         * \param pointer New managed pointer
         * \param refCounter Associated control block
         */
        void Reset(T* pointer, Detail::RefCounter* refCounter)
        {
            if (refCounter)
                refCounter->WeakAcquire();

            if (mRefCounter)
                mRefCounter->WeakRelease();

            mPointer = pointer;
            mRefCounter = refCounter;
        }

        T* mPointer;
        Detail::RefCounter* mRefCounter;
    };
}

#endif // COCKTAIL_CORE_MEMORY_WEAKPTR_HPP
