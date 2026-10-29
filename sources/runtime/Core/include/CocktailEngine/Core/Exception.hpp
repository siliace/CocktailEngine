#ifndef COCKTAILENGINE_CORE_EXCEPTION_HPP
#define COCKTAILENGINE_CORE_EXCEPTION_HPP

#include <exception>
#include <type_traits>
#include <typeinfo>

#include <CocktailEngine/Core/String.hpp>

#define COCKTAIL_DECLARE_EXCEPTION_FROM(__Name, __Base)                                                                                                                            \
    class __Name : public __Base                                                                                                                                                   \
    {                                                                                                                                                                              \
    public:                                                                                                                                                                        \
                                                                                                                                                                                   \
        static_assert(std::is_base_of_v<::Ck::Exception, __Base>);                                                                                                                 \
                                                                                                                                                                                   \
        template <typename... Args>                                                                                                                                                \
        static ::Ck::String FormatExceptionMessage(::Ck::StringView format, Args&&... args)                                                                                        \
        {                                                                                                                                                                          \
            return ::Ck::String::Format(CK_TEXT("%s : %s"), CK_TEXT(#__Name), ::Ck::String::Format(format, Forward<Args>(args)...));                                               \
        }                                                                                                                                                                          \
                                                                                                                                                                                   \
        __Name() = default;                                                                                                                                                        \
                                                                                                                                                                                   \
        explicit __Name(::Ck::String message) :                                                                                                                                    \
            __Base(Move(message))                                                                                                                                                  \
        {                                                                                                                                                                          \
        }                                                                                                                                                                          \
                                                                                                                                                                                   \
        __Name(const ::Ck::Exception& nested, ::Ck::String message) :                                                                                                              \
            __Base(nested, Move(message))                                                                                                                                          \
        {                                                                                                                                                                          \
        }                                                                                                                                                                          \
                                                                                                                                                                                   \
        template <typename... Args>                                                                                                                                                \
        explicit __Name(::Ck::StringView format, Args&&... args) :                                                                                                                 \
            __Base(FormatExceptionMessage(format, Forward<Args>(args)...))                                                                                                         \
        {                                                                                                                                                                          \
        }                                                                                                                                                                          \
                                                                                                                                                                                   \
        template <typename... Args>                                                                                                                                                \
        explicit __Name(const ::Ck::Exception& nested, ::Ck::StringView format, Args&&... args) :                                                                                  \
            __Base(nested, FormatExceptionMessage(format, Forward<Args>(args)...))                                                                                                 \
        {                                                                                                                                                                          \
        }                                                                                                                                                                          \
                                                                                                                                                                                   \
        char const* what() const noexcept override                                                                                                                                 \
        {                                                                                                                                                                          \
            return #__Name;                                                                                                                                                        \
        }                                                                                                                                                                          \
                                                                                                                                                                                   \
        ::Ck::StringView GetName() const override                                                                                                                                  \
        {                                                                                                                                                                          \
            return CK_TEXT(#__Name);                                                                                                                                               \
        }                                                                                                                                                                          \
                                                                                                                                                                                   \
    protected:                                                                                                                                                                     \
                                                                                                                                                                                   \
        __Name* Clone() const override                                                                                                                                             \
        {                                                                                                                                                                          \
            return new __Name(*this);                                                                                                                                              \
        }                                                                                                                                                                          \
    }

#define COCKTAIL_DECLARE_EXCEPTION(__Name) COCKTAIL_DECLARE_EXCEPTION_FROM(__Name, ::Ck::Exception)

namespace Ck
{
    /**
     * \brief Base class for exceptions reported by Cocktail Engine.
     *
     * An exception owns a human-readable message and can retain a cloned Cocktail
     * Engine exception that caused it. Derived exception types expose their
     * concrete name through \ref GetName and support polymorphic cloning for
     * exception nesting.
     */
    class COCKTAILENGINE_CORE_API Exception : public std::exception
    {
    public:

        /**
         * \brief Constructs an exception with an empty message.
         */
        Exception();

        /**
         * \brief Constructs an exception with a diagnostic message.
         *
         * \param message Message describing the failure.
         */
        explicit Exception(String message);

        /**
         * \brief Constructs an exception caused by another engine exception.
         *
         * \param nested Exception that caused this exception. Its state is cloned.
         * \param message Message describing the current failure.
         */
        Exception(const Exception& nested, String message);

        /**
         * \brief Copies an exception and its nested exception state.
         *
         * \param other Exception to copy.
         */
        Exception(const Exception& other);

        /**
         * \brief Moves an exception and transfers its nested exception state.
         *
         * \param other Exception to move from.
         */
        Exception(Exception&& other) noexcept;

        /**
         * \brief Destroys the exception and its owned nested exception.
         */
        ~Exception() override;

        /**
         * \brief Assigns another exception to this instance.
         *
         * \param other Exception to assign.
         *
         * \return This exception.
         */
        Exception& operator=(const Exception& other);

        /**
         * \brief Moves another exception into this instance.
         *
         * \param other Exception to move from.
         *
         * \return This exception.
         */
        Exception& operator=(Exception&& other) noexcept;

        /**
         * \brief Gets the static name of the exception type.
         *
         * \return A null-terminated narrow name suitable for \c std::exception
         *         compatibility.
         */
        char const* what() const noexcept override;

        /**
         * \brief Gets the engine-text name of the concrete exception type.
         *
         * \return The exception type name.
         */
        virtual StringView GetName() const;

        /**
         * \brief Gets the diagnostic message associated with this exception.
         *
         * \return The stored message.
         */
        const String& GetMessage() const;

        /**
         * \brief Gets the exception that caused this exception.
         *
         * The returned pointer is non-owning and remains valid while this exception
         * remains alive. It is null when no engine exception was provided as a cause.
         *
         * \return The nested exception, or null when this exception has no cause.
         */
        const Exception* GetNestedException() const noexcept;

    protected:

        /**
         * \brief Clones this exception while preserving its concrete type.
         *
         * \return A newly allocated copy of this exception.
         */
        virtual Exception* Clone() const;

    private:

        String mMessage;
        Exception* mNested;
    };

    COCKTAIL_DECLARE_EXCEPTION(LogicException);
    COCKTAIL_DECLARE_EXCEPTION_FROM(InvalidParameterException, LogicException);
    COCKTAIL_DECLARE_EXCEPTION_FROM(NotImplementedException, LogicException);
    COCKTAIL_DECLARE_EXCEPTION_FROM(ContainerEmpty, LogicException);
    COCKTAIL_DECLARE_EXCEPTION_FROM(ContainerOutOfRange, LogicException);

    COCKTAIL_DECLARE_EXCEPTION(RuntimeException);
    COCKTAIL_DECLARE_EXCEPTION_FROM(OutOfMemory, RuntimeException);
    COCKTAIL_DECLARE_EXCEPTION_FROM(EmptyOptionalException, RuntimeException);

    COCKTAIL_DECLARE_EXCEPTION_FROM(EncodingException, RuntimeException);
    COCKTAIL_DECLARE_EXCEPTION_FROM(CodepointEncodingException, EncodingException);
    COCKTAIL_DECLARE_EXCEPTION_FROM(CodepointDecodingException, EncodingException);
}

#endif // COCKTAILENGINE_CORE_EXCEPTION_HPP
