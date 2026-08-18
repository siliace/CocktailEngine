#ifndef COCKTAILENGINE_CORE_UTILITY_OPTIONAL_HPP
#define COCKTAILENGINE_CORE_UTILITY_OPTIONAL_HPP

#include <CocktailEngine/Core/Cocktail.hpp>
#include <CocktailEngine/Core/Utility/ExceptionUtils.hpp>

namespace Ck
{
    /**
     * \brief Stores a value that may be absent.
     *
     * An Optional is either empty or owns one value of type \p T. An engaged
     * Optional whose value is a pointer may still contain \c nullptr; this is
     * distinct from an empty Optional.
     *
     * \tparam T Type of the owned value.
     */
    template <typename T>
    class Optional
    {
    public:

        /**
         * \brief Creates an empty Optional.
         *
         * \return An Optional with no contained value.
         */
        static Optional Empty()
        {
            return {};
        }

        /**
         * \brief Creates an Optional by moving a value into it.
         *
         * \param value Value to move into the Optional.
         *
         * \return An Optional containing the moved value.
         */
        static Optional Of(T&& value)
        {
            return Optional<T>(Forward<T>(value));
        }

        /**
         * \brief Creates an Optional by copying a value into it.
         *
         * \param value Value to copy into the Optional.
         *
         * \return An Optional containing a copy of \p value.
         */
        static Optional Of(const T& value)
        {
            return Optional(value);
        }

        /**
         * \brief Creates an Optional by constructing its value in place.
         *
         * \tparam Args Constructor argument types.
         * \param args Arguments forwarded to the constructor of \p T.
         *
         * \return An Optional containing the constructed value.
         */
        template <typename... Args>
        static Optional Of(InPlaceTag, Args&&... args)
        {
            return Optional(InPlace, Forward<Args>(args)...);
        }

        /**
         * \brief Constructs an empty Optional.
         */
        Optional() :
            mEmpty(true),
            mStorage{}
        {
            /// Nothing
        }

        /**
         * \brief Copy-constructs an Optional.
         *
         * The new Optional is empty when \p other is empty; otherwise it contains
         * a copy of the value in \p other.
         *
         * \param other Optional to copy.
         */
        Optional(const Optional& other) :
            mEmpty(true)
        {
            *this = other;
        }

        /**
         * \brief Move-constructs an Optional.
         *
         * If \p other contains a value, that value is moved to this Optional and
         * \p other becomes empty.
         *
         * \param other Optional to move from.
         */
        Optional(Optional&& other) noexcept(std::is_nothrow_move_constructible_v<T>) :
            mEmpty(true)
        {
            *this = Move(other);
        }

        /**
         * \brief Destroys the contained value when the Optional is engaged.
         */
        ~Optional()
        {
            Destroy();
        }

        /**
         * \brief Replaces this Optional with a copy of another Optional.
         *
         * The Optional is empty after the assignment when \p other is empty.
         * Self-assignment leaves the Optional unchanged.
         *
         * \param other Optional to copy.
         *
         * \return This Optional.
         */
        Optional& operator=(const Optional& other)
        {
            if (this == &other)
                return *this;

            Destroy();
            if (!other.mEmpty)
            {
                new (&mStorage) T(other.GetValue());
                mEmpty = false;
            }

            return *this;
        }

        /**
         * \brief Replaces this Optional by moving another Optional into it.
         *
         * The source Optional becomes empty after a successful move. Self-move
         * assignment leaves the Optional unchanged.
         *
         * \param other Optional to move from.
         *
         * \return This Optional.
         */
        Optional& operator=(Optional&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
        {
            if (this == &other)
                return *this;

            Destroy();
            if (!other.mEmpty)
            {
                new (&mStorage) T(Move(other.GetValue()));
                mEmpty = false;
                other.Destroy();
            }

            return *this;
        }

        /**
         * \brief Replaces the contained value with a value constructed in place.
         *
         * Any current value is destroyed before the replacement is constructed.
         *
         * \tparam Args Constructor argument types.
         * \param args Arguments forwarded to the constructor of \p T.
         *
         * \return Reference to the constructed value.
         */
        template <typename... Args>
        T& Emplace(Args&&... args)
        {
            Destroy();
            new (&mStorage) T(Forward<Args>(args)...);
            mEmpty = false;

            return GetValue();
        }

        /**
         * \brief Removes the contained value, if any.
         */
        void Reset()
        {
            Destroy();
        }

        /**
         * \brief Checks whether this Optional contains no value.
         *
         * \return \c true when this Optional is empty, \c false otherwise.
         */
        bool IsEmpty() const
        {
            return mEmpty;
        }

        /**
         * \brief Checks whether this Optional contains a value.
         *
         * \return \c true when this Optional is engaged, \c false otherwise.
         */
        bool HasValue() const
        {
            return !mEmpty;
        }

        /**
         * \brief Tests whether this Optional contains a value.
         *
         * This enables direct use in boolean contexts such as \c if (optional).
         *
         * \return \c true when this Optional is engaged, \c false otherwise.
         */
        explicit operator bool() const
        {
            return HasValue();
        }

        /**
         * \brief Transforms the contained value when present.
         *
         * The mapper is invoked only when this Optional is engaged. It receives a
         * const reference to the contained value. Its result may be a value or an
         * lvalue reference; the latter produces a non-owning Optional reference.
         *
         * \tparam Callable Callable type accepting \c const T&.
         * \param mapper Callable used to transform the value.
         *
         * \return An empty Optional when this Optional is empty; otherwise an
         *         Optional containing the result of \p mapper.
         */
        template <typename Callable>
        auto Map(Callable&& mapper) const -> Optional<std::invoke_result_t<Callable&&, const T&>>
        {
            using U = std::invoke_result_t<Callable&&, const T&>;

            if (mEmpty)
                return Optional<U>::Empty();

            return Optional<U>::Of(std::invoke(Forward<Callable>(mapper), GetValue()));
        }

        /**
         * \brief Invokes a callable with the contained value when present.
         *
         * The callable is not invoked when this Optional is empty. A copy of this
         * Optional is returned to allow fluent calls; consequently \p T must be
         * copy-constructible to use this method.
         *
         * \tparam Callable Callable type accepting \c const T&.
         * \param callable Callable to invoke conditionally.
         *
         * \return A copy of this Optional.
         */
        template <typename Callable>
        Optional<T> Then(Callable&& callable) const
        {
            if (!mEmpty)
                std::invoke(Forward<Callable>(callable), GetValue());

            return *this;
        }

        /**
         * \brief Returns the contained value.
         *
         * \return Mutable reference to the contained value.
         *
         * \throws EmptyOptionalException when this Optional is empty.
         */
        T& Get() &
        {
            if (mEmpty)
                ExceptionUtils::ThrowEmptyOptional();

            return GetValue();
        }

        /**
         * \brief Returns the contained value.
         *
         * \return Const reference to the contained value.
         *
         * \throws EmptyOptionalException when this Optional is empty.
         */
        const T& Get() const&
        {
            if (mEmpty)
                ExceptionUtils::ThrowEmptyOptional();

            return GetValue();
        }

        /**
         * \brief Moves the contained value out of this Optional.
         *
         * The Optional remains engaged; the state of its moved-from value is
         * determined by \p T.
         *
         * \return Rvalue reference to the contained value.
         *
         * \throws EmptyOptionalException when this Optional is empty.
         */
        T&& Get() &&
        {
            if (mEmpty)
                ExceptionUtils::ThrowEmptyOptional();

            return Move(GetValue());
        }

        /**
         * \brief Returns a const rvalue reference to the contained value.
         *
         * \return Const rvalue reference to the contained value.
         *
         * \throws EmptyOptionalException when this Optional is empty.
         */
        const T&& Get() const&&
        {
            if (mEmpty)
                ExceptionUtils::ThrowEmptyOptional();

            return Move(GetValue());
        }

        /**
         * \brief Provides pointer-like access to the contained value.
         *
         * \return Pointer to the contained value.
         *
         * \throws EmptyOptionalException when this Optional is empty.
         */
        T* operator->()
        {
            return &Get();
        }

        /**
         * \brief Provides pointer-like access to the contained value.
         *
         * \return Const pointer to the contained value.
         *
         * \throws EmptyOptionalException when this Optional is empty.
         */
        const T* operator->() const
        {
            return &Get();
        }

        /**
         * \brief Provides dereference access to the contained value.
         *
         * \return Mutable reference to the contained value.
         *
         * \throws EmptyOptionalException when this Optional is empty.
         */
        T& operator*() &
        {
            return Get();
        }

        /**
         * \brief Provides dereference access to the contained value.
         *
         * \return Const reference to the contained value.
         *
         * \throws EmptyOptionalException when this Optional is empty.
         */
        const T& operator*() const&
        {
            return Get();
        }

        /**
         * \brief Provides rvalue dereference access to the contained value.
         *
         * \return Rvalue reference to the contained value.
         *
         * \throws EmptyOptionalException when this Optional is empty.
         */
        T&& operator*() &&
        {
            return Move(*this).Get();
        }

        /**
         * \brief Provides const rvalue dereference access to the contained value.
         *
         * \return Const rvalue reference to the contained value.
         *
         * \throws EmptyOptionalException when this Optional is empty.
         */
        const T&& operator*() const&&
        {
            return Move(*this).Get();
        }

        /**
         * \brief Returns the contained value or a fallback reference.
         *
         * \param fallback Reference returned when this Optional is empty.
         *
         * \return The contained value when engaged; otherwise \p fallback.
         *
         * \warning The returned reference must not outlive \p fallback.
         */
        T& GetOr(T& fallback)
        {
            if (!mEmpty)
                return Get();

            return fallback;
        }

        /**
         * \brief Returns the contained value or a const fallback reference.
         *
         * \param fallback Reference returned when this Optional is empty.
         *
         * \return The contained value when engaged; otherwise \p fallback.
         *
         * \warning The returned reference must not outlive \p fallback.
         */
        const T& GetOr(const T& fallback) const
        {
            if (!mEmpty)
                return Get();

            return fallback;
        }

        /**
         * \brief Returns the contained value or invokes a fallback callable.
         *
         * The fallback is invoked only when this Optional is empty.
         *
         * \tparam Callable Nullary callable type.
         * \param fallback Callable used to produce the fallback result.
         *
         * \return The contained value when engaged; otherwise the result of
         *         \p fallback.
         */
        template <typename Callable>
        auto GetOrElse(Callable&& fallback) & -> std::invoke_result_t<Callable&&>
        {
            if (!mEmpty)
                return Get();

            return std::invoke(Forward<Callable>(fallback));
        }

        /**
         * \brief Returns the contained value or invokes a fallback callable.
         *
         * The fallback is invoked only when this Optional is empty.
         *
         * \tparam Callable Nullary callable type.
         * \param fallback Callable used to produce the fallback result.
         *
         * \return The contained value when engaged; otherwise the result of
         *         \p fallback.
         */
        template <typename Callable>
        auto GetOrElse(Callable&& fallback) const& -> std::invoke_result_t<Callable&&>
        {
            if (!mEmpty)
                return Get();

            return std::invoke(Forward<Callable>(fallback));
        }

        /**
         * \brief Moves the contained value or invokes a fallback callable.
         *
         * The fallback is invoked only when this Optional is empty. When engaged,
         * the Optional remains engaged and its value is moved from.
         *
         * \tparam Callable Nullary callable type.
         * \param fallback Callable used to produce the fallback result.
         *
         * \return The moved contained value when engaged; otherwise the result of
         *         \p fallback.
         */
        template <typename Callable>
        auto GetOrElse(Callable&& fallback) && -> std::invoke_result_t<Callable&&>
        {
            if (!mEmpty)
                return Move(Get());

            return std::invoke(Forward<Callable>(fallback));
        }

        /**
         * \brief Moves the contained value or invokes a fallback callable.
         *
         * The fallback is invoked only when this Optional is empty. The contained
         * value is not modified by this const overload.
         *
         * \tparam Callable Nullary callable type.
         * \param fallback Callable used to produce the fallback result.
         *
         * \return The contained value as a const rvalue when engaged; otherwise
         *         the result of \p fallback.
         */
        template <typename Callable>
        auto GetOrElse(Callable&& fallback) const&& -> std::invoke_result_t<Callable&&>
        {
            if (!mEmpty)
                return Move(Get());

            return std::invoke(Forward<Callable>(fallback));
        }

        /**
         * \brief Returns the contained value or throws a caller-selected exception.
         *
         * \tparam E Exception type constructed when this Optional is empty.
         * \tparam Args Constructor argument types for \p E.
         * \param args Arguments forwarded to the exception constructor.
         *
         * \return Mutable reference to the contained value.
         *
         * \throws E when this Optional is empty.
         */
        template <typename E, typename... Args>
        T& GetOrThrow(Args&&... args)
        {
            if (mEmpty)
                throw E(Forward<Args>(args)...);

            return GetValue();
        }

        /**
         * \brief Returns the contained value or throws a caller-selected exception.
         *
         * \tparam E Exception type constructed when this Optional is empty.
         * \tparam Args Constructor argument types for \p E.
         * \param args Arguments forwarded to the exception constructor.
         *
         * \return Const reference to the contained value.
         *
         * \throws E when this Optional is empty.
         */
        template <typename E, typename... Args>
        const T& GetOrThrow(Args&&... args) const
        {
            if (mEmpty)
                throw E(Forward<Args>(args)...);

            return GetValue();
        }

    private:

        /**
         * \brief Constructs an Optional from a moved value.
         *
         * \param value Value to move into the Optional.
         */
        explicit Optional(T&& value) :
            mEmpty(false)
        {
            new (&mStorage) T(Forward<T>(value));
        }

        /**
         * \brief Constructs an Optional from a copied value.
         *
         * \param value Value to copy into the Optional.
         */
        explicit Optional(const T& value) :
            mEmpty(false)
        {
            new (&mStorage) T(value);
        }

        /**
         * \brief Constructs the contained value in place.
         *
         * \tparam Args Constructor argument types.
         * \param args Arguments forwarded to the constructor of \p T.
         */
        template <typename... Args>
        explicit Optional(InPlaceTag, Args&&... args) :
            mEmpty(false)
        {
            new (&mStorage) T(Forward<Args>(args)...);
        }

        /**
         * \brief Destroys the contained value and marks this Optional empty.
         */
        void Destroy()
        {
            if (mEmpty)
                return;

            std::destroy_at(reinterpret_cast<T*>(&mStorage));
            mEmpty = true;
        }

        /**
         * \brief Returns the contained value without checking engagement.
         *
         * \return Mutable reference to the storage interpreted as \p T.
         */
        T& GetValue()
        {
            return *reinterpret_cast<T*>(&mStorage);
        }

        /**
         * \brief Returns the contained value without checking engagement.
         *
         * \return Const reference to the storage interpreted as \p T.
         */
        const T& GetValue() const
        {
            return *reinterpret_cast<const T*>(&mStorage);
        }

        bool mEmpty;
        alignas(T) Byte mStorage[sizeof(T)];
    };

    /**
     * \brief Stores an optional non-owning reference.
     *
     * This specialization never creates, destroys or extends the lifetime of the
     * referenced object. The caller must ensure that the referent remains valid
     * while this Optional is used. In particular, a reference obtained from a
     * container may be invalidated by operations on that container.
     *
     * \tparam T Type of the referenced object.
     */
    template <typename T>
    class Optional<T&>
    {
        template <typename>
        friend class Optional;

    public:

        /**
         * \brief Creates an empty Optional reference.
         *
         * \return An Optional reference with no referent.
         */
        static Optional Empty()
        {
            return {};
        }

        /**
         * \brief Creates an Optional reference to an existing object.
         *
         * \param value Object to reference. It is not copied or owned.
         *
         * \return An Optional reference to \p value.
         */
        static Optional Of(T& value)
        {
            return Optional<T&>(&value);
        }

        /**
         * \brief Constructs an empty Optional reference.
         */
        Optional() :
            mValue(nullptr)
        {
            /// Nothing
        }

        /**
         * \brief Copy-constructs an Optional reference.
         *
         * Both Optional instances refer to the same object when \p other is
         * engaged.
         *
         * \param other Optional reference to copy.
         */
        Optional(const Optional& other) :
            mValue(other.mValue)
        {
            /// Nothing
        }

        /**
         * \brief Converts an Optional mutable reference to an Optional const reference.
         *
         * This constructor is enabled only when \p T is the const-qualified form
         * of \p U.
         *
         * \tparam U Type of the source referent.
         * \param other Optional reference to convert.
         */
        template <typename U, typename = std::enable_if_t<std::is_same_v<std::remove_const_t<T>, U>>>
        Optional(const Optional<U&>& other) :
            mValue(other.mValue)
        {
            /// Nothing
        }

        /**
         * \brief Move-constructs an Optional reference.
         *
         * The source becomes empty. The referenced object is not modified.
         *
         * \param other Optional reference to move from.
         */
        Optional(Optional&& other) noexcept
        {
            mValue = std::exchange(other.mValue, nullptr);
        }

        /**
         * \brief Replaces this Optional reference with another reference.
         *
         * \param other Optional reference to copy.
         *
         * \return This Optional reference.
         */
        Optional& operator=(const Optional& other)
        {
            if (this == &other)
                return *this;

            mValue = other.mValue;
            return *this;
        }

        /**
         * \brief Converts and assigns an Optional mutable reference.
         *
         * This assignment is enabled only when \p T is the const-qualified form
         * of \p U.
         *
         * \tparam U Type of the source referent.
         * \param other Optional reference to convert and copy.
         *
         * \return This Optional reference.
         */
        template <typename U, typename = std::enable_if_t<std::is_same_v<std::remove_const_t<T>, U>>>
        Optional& operator=(const Optional<U&>& other)
        {
            mValue = other.mValue;
            return *this;
        }

        /**
         * \brief Replaces this Optional reference by moving another reference.
         *
         * The source becomes empty. The referenced object is not modified.
         *
         * \param other Optional reference to move from.
         *
         * \return This Optional reference.
         */
        Optional& operator=(Optional&& other) noexcept
        {
            mValue = std::exchange(other.mValue, nullptr);
            return *this;
        }

        /**
         * \brief Clears this Optional reference without modifying its referent.
         */
        void Reset()
        {
            mValue = nullptr;
        }

        /**
         * \brief Checks whether this Optional reference has no referent.
         *
         * \return \c true when this Optional reference is empty, \c false otherwise.
         */
        bool IsEmpty() const
        {
            return mValue == nullptr;
        }

        /**
         * \brief Checks whether this Optional reference has a referent.
         *
         * \return \c true when this Optional reference is engaged, \c false otherwise.
         */
        bool HasValue() const
        {
            return mValue != nullptr;
        }

        /**
         * \brief Tests whether this Optional reference has a referent.
         *
         * \return \c true when this Optional reference is engaged, \c false otherwise.
         */
        explicit operator bool() const
        {
            return HasValue();
        }

        /**
         * \brief Transforms the referenced object when present.
         *
         * The mapper is invoked only when this Optional reference is engaged. It
         * receives \c T&, even when the Optional wrapper is const, because the
         * wrapper's constness does not change the referenced object.
         *
         * \tparam Callable Callable type accepting \c T&.
         * \param mapper Callable used to transform the referenced object.
         *
         * \return An empty Optional when this Optional reference is empty;
         *         otherwise an Optional containing the result of \p mapper.
         */
        template <typename Callable>
        auto Map(Callable&& mapper) const -> Optional<std::invoke_result_t<Callable&&, T&>>
        {
            using U = std::invoke_result_t<Callable&&, T&>;

            if (!mValue)
                return Optional<U>::Empty();

            return Optional<U>::Of(std::invoke(Forward<Callable>(mapper), *mValue));
        }

        /**
         * \brief Invokes a callable with the referenced object when present.
         *
         * The callable is not invoked when this Optional reference is empty. A
         * copy of this lightweight, non-owning wrapper is returned for chaining.
         *
         * \tparam Callable Callable type accepting \c T&.
         * \param callable Callable to invoke conditionally.
         *
         * \return A copy of this Optional reference.
         */
        template <typename Callable>
        Optional<T&> Then(Callable&& callable) const
        {
            if (mValue)
                std::invoke(Forward<Callable>(callable), *mValue);

            return *this;
        }

        /**
         * \brief Returns the referenced object.
         *
         * \return Mutable reference to the referenced object.
         *
         * \throws EmptyOptionalException when this Optional reference is empty.
         */
        T& Get()
        {
            if (!mValue)
                ExceptionUtils::ThrowEmptyOptional();

            return *mValue;
        }

        /**
         * \brief Returns the referenced object.
         *
         * \return Const reference to the referenced object.
         *
         * \throws EmptyOptionalException when this Optional reference is empty.
         */
        const T& Get() const
        {
            if (!mValue)
                ExceptionUtils::ThrowEmptyOptional();

            return *mValue;
        }

        /**
         * \brief Provides pointer-like access to the referenced object.
         *
         * \return Pointer to the referenced object.
         *
         * \throws EmptyOptionalException when this Optional reference is empty.
         */
        T* operator->()
        {
            return &Get();
        }

        /**
         * \brief Provides pointer-like access to the referenced object.
         *
         * \return Const pointer to the referenced object.
         *
         * \throws EmptyOptionalException when this Optional reference is empty.
         */
        const T* operator->() const
        {
            return &Get();
        }

        /**
         * \brief Provides dereference access to the referenced object.
         *
         * \return Mutable reference to the referenced object.
         *
         * \throws EmptyOptionalException when this Optional reference is empty.
         */
        T& operator*()
        {
            return Get();
        }

        /**
         * \brief Provides dereference access to the referenced object.
         *
         * \return Const reference to the referenced object.
         *
         * \throws EmptyOptionalException when this Optional reference is empty.
         */
        const T& operator*() const
        {
            return Get();
        }

        /**
         * \brief Returns the referenced object or a fallback reference.
         *
         * \param fallback Reference returned when this Optional reference is empty.
         *
         * \return The referenced object when engaged; otherwise \p fallback.
         *
         * \warning The returned reference must not outlive \p fallback or the referent.
         */
        T& GetOr(T& fallback)
        {
            if (mValue)
                return Get();

            return fallback;
        }

        /**
         * \brief Returns the referenced object or a const fallback reference.
         *
         * \param fallback Reference returned when this Optional reference is empty.
         *
         * \return The referenced object when engaged; otherwise \p fallback.
         *
         * \warning The returned reference must not outlive \p fallback or the referent.
         */
        const T& GetOr(const T& fallback) const
        {
            if (mValue)
                return Get();

            return fallback;
        }

        /**
         * \brief Returns the referenced object or invokes a fallback callable.
         *
         * The fallback is invoked only when this Optional reference is empty.
         *
         * \tparam Callable Nullary callable type.
         * \param fallback Callable used to produce the fallback result.
         *
         * \return The referenced object when engaged; otherwise the result of
         *         \p fallback.
         */
        template <typename Callable>
        auto GetOrElse(Callable&& fallback) -> std::invoke_result_t<Callable&&>
        {
            if (mValue)
                return Get();

            return std::invoke(Forward<Callable>(fallback));
        }

        /**
         * \brief Returns the referenced object or invokes a fallback callable.
         *
         * The fallback is invoked only when this Optional reference is empty.
         *
         * \tparam Callable Nullary callable type.
         * \param fallback Callable used to produce the fallback result.
         *
         * \return The referenced object when engaged; otherwise the result of
         *         \p fallback.
         */
        template <typename Callable>
        auto GetOrElse(Callable&& fallback) const -> std::invoke_result_t<Callable&&>
        {
            if (mValue)
                return Get();

            return std::invoke(Forward<Callable>(fallback));
        }

        /**
         * \brief Returns the referenced object or throws a caller-selected exception.
         *
         * \tparam E Exception type constructed when this Optional reference is empty.
         * \tparam Args Constructor argument types for \p E.
         * \param args Arguments forwarded to the exception constructor.
         *
         * \return Mutable reference to the referenced object.
         *
         * \throws E when this Optional reference is empty.
         */
        template <typename E, typename... Args>
        T& GetOrThrow(Args&&... args)
        {
            if (!mValue)
                throw E(Forward<Args>(args)...);

            return *mValue;
        }

        /**
         * \brief Returns the referenced object or throws a caller-selected exception.
         *
         * \tparam E Exception type constructed when this Optional reference is empty.
         * \tparam Args Constructor argument types for \p E.
         * \param args Arguments forwarded to the exception constructor.
         *
         * \return Const reference to the referenced object.
         *
         * \throws E when this Optional reference is empty.
         */
        template <typename E, typename... Args>
        const T& GetOrThrow(Args&&... args) const
        {
            if (!mValue)
                throw E(Forward<Args>(args)...);

            return *mValue;
        }

    private:

        /**
         * \brief Constructs an Optional reference from a pointer.
         *
         * \param value Pointer to the referenced object, or \c nullptr for empty.
         */
        explicit Optional(T* value) :
            mValue(value)
        {
            /// Nothing
        }

        T* mValue;
    };
}

#endif // COCKTAILENGINE_CORE_UTILITY_OPTIONAL_HPP
