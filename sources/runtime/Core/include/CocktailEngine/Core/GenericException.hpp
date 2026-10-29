#ifndef COCKTAILENGINE_CORE_GENERICEXCEPTION_HPP
#define COCKTAILENGINE_CORE_GENERICEXCEPTION_HPP

#include <type_traits>

#include <CocktailEngine/Core/Exception.hpp>

namespace Ck
{
    /**
     * \brief Converts an error code into the message carried by a generic exception.
     *
     * Implementations define the error-code domain used by a \ref GenericException.
     * A translator is consulted only while an exception is constructed; the exception
     * stores the resulting message and does not retain the translator.
     *
     * \tparam TErrorCode Type that identifies an error in the translated domain.
     */
    template <typename TErrorCode>
    class ErrorCodeTranslator
    {
    public:

        /**
         * \brief Type of error code accepted by this translator.
         */
        using ErrorCodeType = TErrorCode;

        virtual ~ErrorCodeTranslator() = default;

        /**
         * \brief Converts an error code into a diagnostic message.
         *
         * \param errorCode Error code to translate.
         *
         * \return A message describing \p errorCode.
         */
        virtual String TranslateErrorCode(ErrorCodeType errorCode) const = 0;
    };

    /**
     * \brief Runtime exception that preserves a typed error code.
     *
     * The exception obtains its message from an \ref ErrorCodeTranslator at
     * construction time and retains a copy of the original error code. Catch this
     * exception by reference, then use \ref GetErrorCode to branch on the typed
     * error code rather than parsing \ref Exception::GetMessage.
     *
     * \tparam TErrorCode Type of error code stored by the exception.
     * \tparam TErrorCodeTranslator Translator whose \c ErrorCodeType must match
     *         \p TErrorCode.
     */
    template <typename TErrorCode, typename TErrorCodeTranslator>
    class GenericException : public RuntimeException
    {
    public:

        static_assert(std::is_same_v<TErrorCode, typename TErrorCodeTranslator::ErrorCodeType>);

        /**
         * \brief Constructs an exception from an error code.
         *
         * \param errorCode Error code to preserve and translate.
         * \param translator Translator used to generate the exception message. When
         *         omitted, \p TErrorCodeTranslator must be default-constructible.
         */
        explicit GenericException(TErrorCode errorCode, const TErrorCodeTranslator& translator = {}) :
            RuntimeException(translator.TranslateErrorCode(errorCode)),
            mErrorCode(errorCode)
        {
            /// Nothing
        }

        /**
         * \brief Constructs an exception with a nested Cocktail Engine exception.
         *
         * \param nested Exception that caused this error.
         * \param errorCode Error code to preserve and translate.
         * \param translator Translator used to generate the exception message. When
         *         omitted, \p TErrorCodeTranslator must be default-constructible.
         */
        GenericException(const Exception& nested, TErrorCode errorCode, const TErrorCodeTranslator& translator = {}) :
            RuntimeException(nested, translator.TranslateErrorCode(errorCode)),
            mErrorCode(errorCode)
        {
            /// Nothing
        }

        /**
         * \brief Gets the static name of this exception type.
         *
         * \return \c "GenericException".
         */
        char const* what() const noexcept override
        {
            return "GenericException";
        }

        /**
         * \brief Gets the typed name of this exception.
         *
         * \return \c "GenericException".
         */
        StringView GetName() const override
        {
            return CK_TEXT("GenericException");
        }

        /**
         * \brief Gets the error code that caused this exception.
         *
         * \return The unmodified error code supplied at construction.
         */
        TErrorCode GetErrorCode() const
        {
            return mErrorCode;
        }

    protected:

        /**
         * \brief Clones this exception and retains its typed error code.
         *
         * \return A newly allocated copy of this exception.
         */
        GenericException* Clone() const override
        {
            return new GenericException(*this);
        }

    private:

        TErrorCode mErrorCode;
    };
}

#endif // COCKTAILENGINE_CORE_GENERICEXCEPTION_HPP
