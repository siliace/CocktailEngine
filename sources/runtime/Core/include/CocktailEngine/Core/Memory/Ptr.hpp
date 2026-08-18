#ifndef COCKTAILENGINE_CORE_MEMORY_PTR_HPP
#define COCKTAILENGINE_CORE_MEMORY_PTR_HPP

#include <CocktailEngine/Core/Memory/SharedPtr.hpp>

namespace Ck
{
    /**
     * \brief Lightweight non-owning pointer wrapper
     *
     * Ptr is a thin observer over a raw pointer that can be implicitly built from
     * a raw pointer, a UniquePtr or a SharedPtr. It carries no ownership, does not
     * touch any reference counter, and is therefore free to copy.
     *
     * Its main purpose is to shorten pointer declarations, especially in range-based
     * for loops iterating over a container of smart pointers. Binding the element to
     * a `SharedPtr<T>` by value would increment and decrement the strong reference
     * count on every iteration, while `Ptr<T>` only copies the address.
     *
     * \code
     * Array<SharedPtr<Shape>> shapes;
     *
     * for (Ptr<Shape> shape : shapes)     // no reference counting
     *     shape->AddToQueue(queue);
     * \endcode
     *
     * Because Ptr does not extend the lifetime of the pointed object, it must never
     * outlive the smart pointer it was built from. It is meant for local, short-lived
     * observation such as loop variables, parameters and local aliases, not for storage
     * in a long-lived structure.
     *
     * \tparam T Pointed object type
     */
    template <typename T>
    class Ptr
    {
    public:

        /**
         * \brief Default constructor
         *
         * Creates a new empty instance of Ptr.
         */
        constexpr Ptr() noexcept :
            mPointer(nullptr)
        {
            /// Nothing
        }

        /**
         * \brief Constructor
         *
         * Creates a new empty instance of Ptr.
         */
        constexpr Ptr(std::nullptr_t) noexcept :
            mPointer(nullptr)
        {
            /// Nothing
        }

        /**
         * \brief Constructor
         *
         * Creates a new instance of Ptr observing a raw pointer.
         *
         * The pointed object is not owned, its lifetime remains the responsibility of the caller.
         *
         * \param pointer Pointer to observe
         */
        constexpr Ptr(T* pointer) noexcept :
            mPointer(pointer)
        {
            /// Nothing
        }

        /**
         * \brief Constructor
         *
         * Creates a new instance of Ptr observing a raw pointer of a convertible type,
         * which typically allows implicit upcasting from a derived type to a base type.
         *
         * \tparam U Source pointer type
         *
         * \param pointer Pointer to observe
         */
        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        constexpr Ptr(U* pointer) noexcept :
            mPointer(pointer)
        {
            /// Nothing
        }

        /**
         * \brief Constructor
         *
         * Creates a new instance of Ptr observing the object managed by a UniquePtr.
         *
         * Ownership stays with the UniquePtr and the resulting Ptr is only valid as
         * long as the source pointer keeps managing the object.
         *
         * \tparam U Managed object type of the source pointer
         * \tparam Deleter Deleter type used by the source pointer
         *
         * \param pointer Unique pointer to observe
         */
        template <typename U, typename Deleter, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        Ptr(const UniquePtr<U, Deleter>& pointer) noexcept :
            mPointer(pointer.Get())
        {
            /// Nothing
        }

        /**
         * \brief Constructor
         *
         * Creates a new instance of Ptr observing the object managed by a SharedPtr.
         *
         * The strong reference count is left untouched, so the resulting Ptr is only
         * valid as long as at least one SharedPtr keeps the object alive.
         *
         * \tparam U Managed object type of the source pointer
         *
         * \param pointer Shared pointer to observe
         */
        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        Ptr(const SharedPtr<U>& pointer) noexcept :
            mPointer(pointer.Get())
        {
            /// Nothing
        }

        /**
         * \brief Returns the observed raw pointer
         *
         * \return Pointer to the observed object, or nullptr
         */
        T* Get() const noexcept
        {
            return mPointer;
        }

        /**
         * \brief Dereferences the observed object
         *
         * \return Reference to the observed object
         */
        T& operator*() const noexcept
        {
            return *mPointer;
        }

        /**
         * \brief Returns the observed pointer
         *
         * \return Pointer to the observed object
         */
        T* operator->() const noexcept
        {
            return mPointer;
        }

        /**
         * \brief Tests whether the Ptr contains a valid pointer
         *
         * \return `true` if the observed pointer is not null, `false` otherwise
         */
        explicit operator bool() const noexcept
        {
            return mPointer != nullptr;
        }

        /**
         * \brief Equality comparison between two Ptr instances
         *
         * Both operands are taken by value, so any type implicitly convertible to Ptr,
         * such as a raw pointer, a UniquePtr, a SharedPtr or nullptr, can be compared
         * against a Ptr.
         *
         * \param lhs Left hand side pointer
         * \param rhs Right hand side pointer
         *
         * \return `true` if both observe the same object, `false` otherwise
         */
        friend bool operator==(Ptr lhs, Ptr rhs) noexcept
        {
            return lhs.mPointer == rhs.mPointer;
        }

        /**
         * \brief Inequality comparison between two Ptr instances
         *
         * \param lhs Left hand side pointer
         * \param rhs Right hand side pointer
         *
         * \return `true` if both observe a different object, `false` otherwise
         */
        friend bool operator!=(Ptr lhs, Ptr rhs) noexcept
        {
            return lhs.mPointer != rhs.mPointer;
        }

    private:

        T* mPointer;
    };
}

#endif // COCKTAILENGINE_CORE_MEMORY_PTR_HPP
