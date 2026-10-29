#ifndef COCKTAILENGINE_CORE_SYSTEM_SYSTEMERROR_HPP
#define COCKTAILENGINE_CORE_SYSTEM_SYSTEMERROR_HPP

#include <CocktailEngine/Core/Export.hpp>
#include <CocktailEngine/Core/GenericException.hpp>

namespace Ck
{
    /**
     * \brief Translates native operating-system error codes into diagnostic text.
     *
     * The implementation maps the unsigned native error code used by the current
     * platform, such as a Win32 last-error value or \c errno, into an engine
     * \ref String.
     */
    class COCKTAILENGINE_CORE_API SystemErrorCodeTranslator : public ErrorCodeTranslator<unsigned int>
    {
    public:

        /**
         * \brief Process-wide translator for native system error codes.
         */
        static const SystemErrorCodeTranslator Instance;

        /**
         * \brief Translates a native system error code.
         *
         * \param errorCode Error code returned by the operating system.
         *
         * \return A platform-provided diagnostic message, or a fallback message
         *         containing the code when no description is available.
         */
        String TranslateErrorCode(unsigned int errorCode) const override;
    };

    /**
     * \brief Runtime exception that retains a native operating-system error code.
     *
     * The exception message is translated when the exception is constructed. Use
     * \ref GetErrorCode to inspect the original platform-specific code.
     */
    using SystemException = GenericException<unsigned int, SystemErrorCodeTranslator>;

    /**
     * \brief Retrieves the most recent error reported by the operating system.
     *
     * Error codes are thread-local on supported platforms. Read the error before
     * making another system call that can overwrite it.
     */
    class COCKTAILENGINE_CORE_API SystemError
    {
    public:

        /**
         * \brief Gets the current native last-error code.
         *
         * \return The current platform-specific last-error value.
         */
        static unsigned int GetLastErrorCode();

        /**
         * \brief Constructs an exception from the current native last-error code.
         *
         * \return A \ref SystemException containing the captured code and its
         *         translated diagnostic message.
         */
        static SystemException GetLastError()
        {
            return SystemException(GetLastErrorCode(), SystemErrorCodeTranslator::Instance);
        }

        /**
         * \brief Gets the translator used for native system error codes.
         *
         * \return The immutable process-wide translator. The reference remains
         *         valid for the lifetime of the process.
         */
        static const SystemErrorCodeTranslator& GetSystemErrorCategory()
        {
            return SystemErrorCodeTranslator::Instance;
        }
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_SYSTEMERROR_HPP
