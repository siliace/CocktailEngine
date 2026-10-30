#ifndef COCKTAILENGINE_CORE_UTILITY_RESULT_HPP
#define COCKTAILENGINE_CORE_UTILITY_RESULT_HPP

#include <concepts>
#include <type_traits>

#include <CocktailEngine/Core/Exception.hpp>
#include <CocktailEngine/Core/Utility/Optional.hpp>
#include <CocktailEngine/Core/Utility/Variant.hpp>

namespace Ck
{
    /**
     * \brief Exception thrown when accessing the inactive side of a Result.
     */
    COCKTAIL_DECLARE_EXCEPTION_FROM(BadResultAccessException, RuntimeException);

    namespace Detail
    {
        template <typename T>
        struct ResultValue
        {
            template <typename... TArgs>
                requires std::constructible_from<T, TArgs&&...>
            explicit ResultValue(InPlaceTag, TArgs&&... args) noexcept(std::is_nothrow_constructible_v<T, TArgs&&...>) :
                Value(Forward<TArgs>(args)...)
            {
                /// Nothing
            }

            T Value;
        };

        template <typename TError>
        struct ResultError
        {
            template <typename... TArgs>
                requires std::constructible_from<TError, TArgs&&...>
            explicit ResultError(InPlaceTag, TArgs&&... args) noexcept(std::is_nothrow_constructible_v<TError, TArgs&&...>) :
                Error(Forward<TArgs>(args)...)
            {
                /// Nothing
            }

            TError Error;
        };
    }

    /**
     * \brief Represents either a successful value or an error.
     *
     * A Result is created with Success or Failure and has no default state. The
     * value and error types may be identical because they are stored in distinct
     * internal alternatives. A moved-from Result remains successful or failed,
     * although its contained object is in the state defined by its move constructor.
     *
     * References held by the Optionals returned by TryGetValue and TryGetError,
     * as well as those returned by GetValue and GetError, remain valid until the
     * Result is assigned to or destroyed.
     *
     * Both template parameters must be object types with non-throwing destructors.
     * Copy operations are available only when both types are copy-constructible.
     * Move operations are available only when both types are move-constructible.
     *
     * \tparam T Type of the successful value.
     * \tparam TError Type of the error value.
     */
    template <typename T, typename TError>
    class Result
    {
        static_assert(std::is_object_v<T>, "Result values must be object types");
        static_assert(std::is_object_v<TError>, "Result errors must be object types");
        static_assert(std::is_nothrow_destructible_v<T>, "Result values must have non-throwing destructors");
        static_assert(std::is_nothrow_destructible_v<TError>, "Result errors must have non-throwing destructors");

        using ValueStorage = Detail::ResultValue<T>;
        using ErrorStorage = Detail::ResultError<TError>;
        using Storage = Variant<ValueStorage, ErrorStorage>;

        static constexpr bool CanCopy = std::is_copy_constructible_v<T> && std::is_copy_constructible_v<TError>;
        static constexpr bool CanMove = std::is_move_constructible_v<T> && std::is_move_constructible_v<TError>;
        static constexpr bool HasNoThrowMove = std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<TError>;

    public:

        /**
         * \brief Result instances must be explicitly successful or failed.
         */
        Result() = delete;

        /**
         * \brief Copy-constructs a Result.
         *
         * \param other Result to copy.
         */
        Result(const Result& other) requires(CanCopy) = default;

        Result(const Result&) requires(!CanCopy) = delete;

        /**
         * \brief Move-constructs a Result without changing its state.
         *
         * The source keeps its successful or failed state. Its contained value or
         * error is moved from and therefore has the state defined by its type.
         *
         * \param other Result to move from.
         */
        Result(Result&& other) noexcept(HasNoThrowMove) requires(CanMove)
        {
            MoveFrom(other);
        }

        Result(Result&&) requires(!CanMove) = delete;

        /**
         * \brief Replaces this Result with a copy of another Result.
         *
         * \param other Result to copy.
         *
         * \return This Result.
         */
        Result& operator=(const Result& other) requires(CanCopy) = default;

        Result& operator=(const Result&) requires(!CanCopy) = delete;

        /**
         * \brief Replaces this Result by moving the contents of another Result.
         *
         * The source keeps its successful or failed state, while its contained
         * object becomes moved from.
         *
         * \param other Result to move from.
         *
         * \return This Result.
         */
        Result& operator=(Result&& other) noexcept(HasNoThrowMove) requires(CanMove)
        {
            if (this != &other)
            {
                MoveFrom(other);
            }

            return *this;
        }

        Result& operator=(Result&&) requires(!CanMove) = delete;

        /**
         * \brief Creates a successful Result by constructing its value in place.
         *
         * \tparam TArgs Constructor argument types for \p T.
         *
         * \param args Arguments forwarded to the constructor of \p T.
         *
         * \return A successful Result containing the constructed value.
         */
        template <typename... TArgs>
            requires std::constructible_from<T, TArgs&&...>
        static Result Success(TArgs&&... args)
        {
            return Result(InPlaceType<ValueStorage>, Forward<TArgs>(args)...);
        }

        /**
         * \brief Creates a failed Result by constructing its error in place.
         *
         * \tparam TArgs Constructor argument types for \p TError.
         *
         * \param args Arguments forwarded to the constructor of \p TError.
         *
         * \return A failed Result containing the constructed error.
         */
        template <typename... TArgs>
            requires std::constructible_from<TError, TArgs&&...>
        static Result Failure(TArgs&&... args)
        {
            return Result(InPlaceType<ErrorStorage>, Forward<TArgs>(args)...);
        }

        /**
         * \brief Checks whether this Result contains a successful value.
         *
         * \return \c true when this Result is successful, \c false otherwise.
         */
        bool IsSuccess() const
        {
            return mStorage.template Is<ValueStorage>();
        }

        /**
         * \brief Checks whether this Result contains an error.
         *
         * \return \c true when this Result is failed, \c false otherwise.
         */
        bool IsFailure() const
        {
            return mStorage.template Is<ErrorStorage>();
        }

        /**
         * \brief Tests whether this Result is successful.
         *
         * This enables direct use in boolean contexts such as \c if (result).
         *
         * \return \c true when this Result is successful, \c false otherwise.
         */
        explicit operator bool() const
        {
            return IsSuccess();
        }

        /**
         * \brief Retrieves the successful value as a mutable reference.
         *
         * \return Mutable reference to the successful value.
         *
         * \throws BadResultAccessException If this Result contains an error.
         */
        T& GetValue()
        {
            Optional<T&> value = TryGetValue();
            if (value.HasValue())
                return value.Get();

            ThrowBadValueAccess();
        }

        /**
         * \brief Retrieves the successful value as a const reference.
         *
         * \return Const reference to the successful value.
         *
         * \throws BadResultAccessException If this Result contains an error.
         */
        const T& GetValue() const
        {
            Optional<const T&> value = TryGetValue();
            if (value.HasValue())
                return value.Get();

            ThrowBadValueAccess();
        }

        /**
         * \brief Retrieves the error as a mutable reference.
         *
         * \return Mutable reference to the error.
         *
         * \throws BadResultAccessException If this Result is successful.
         */
        TError& GetError()
        {
            Optional<TError&> error = TryGetError();
            if (error.HasValue())
                return error.Get();

            ThrowBadErrorAccess();
        }

        /**
         * \brief Retrieves the error as a const reference.
         *
         * \return Const reference to the error.
         *
         * \throws BadResultAccessException If this Result is successful.
         */
        const TError& GetError() const
        {
            Optional<const TError&> error = TryGetError();
            if (error.HasValue())
                return error.Get();

            ThrowBadErrorAccess();
        }

        /**
         * \brief Attempts to retrieve the successful value as a mutable Optional reference.
         *
         * \return An Optional containing a mutable reference to the successful value,
         *         or an empty Optional when this Result contains an error.
         */
        Optional<T&> TryGetValue()
        {
            ValueStorage* value = mStorage.template TryGet<ValueStorage>();
            if (value != nullptr)
                return Optional<T&>::Of(value->Value);

            return Optional<T&>::Empty();
        }

        /**
         * \brief Attempts to retrieve the successful value as a const Optional reference.
         *
         * \return An Optional containing a const reference to the successful value,
         *         or an empty Optional when this Result contains an error.
         */
        Optional<const T&> TryGetValue() const
        {
            const ValueStorage* value = mStorage.template TryGet<ValueStorage>();
            if (value != nullptr)
                return Optional<const T&>::Of(value->Value);

            return Optional<const T&>::Empty();
        }

        /**
         * \brief Attempts to retrieve the error as a mutable Optional reference.
         *
         * \return An Optional containing a mutable reference to the error, or an
         *         empty Optional when this Result is successful.
         */
        Optional<TError&> TryGetError()
        {
            ErrorStorage* error = mStorage.template TryGet<ErrorStorage>();
            if (error != nullptr)
                return Optional<TError&>::Of(error->Error);

            return Optional<TError&>::Empty();
        }

        /**
         * \brief Attempts to retrieve the error as a const Optional reference.
         *
         * \return An Optional containing a const reference to the error, or an
         *         empty Optional when this Result is successful.
         */
        Optional<const TError&> TryGetError() const
        {
            const ErrorStorage* error = mStorage.template TryGet<ErrorStorage>();
            if (error != nullptr)
                return Optional<const TError&>::Of(error->Error);

            return Optional<const TError&>::Empty();
        }

    private:

        template <typename TStorage, typename... TArgs>
        explicit Result(InPlaceTypeTag<TStorage>, TArgs&&... args) :
            mStorage(InPlaceType<TStorage>, InPlace, Forward<TArgs>(args)...)
        {
            /// Nothing
        }

        void MoveFrom(Result& other) requires(CanMove)
        {
            if (other.IsSuccess())
            {
                mStorage.template Emplace<ValueStorage>(InPlace, Move(other.GetValue()));
            }
            else
            {
                mStorage.template Emplace<ErrorStorage>(InPlace, Move(other.GetError()));
            }
        }

        [[noreturn]] void ThrowBadValueAccess() const
        {
            throw BadResultAccessException(CK_TEXT("Cannot retrieve a value from a failed Result."));
        }

        [[noreturn]] void ThrowBadErrorAccess() const
        {
            throw BadResultAccessException(CK_TEXT("Cannot retrieve an error from a successful Result."));
        }

        Storage mStorage;
    };
}

#endif // COCKTAILENGINE_CORE_UTILITY_RESULT_HPP
