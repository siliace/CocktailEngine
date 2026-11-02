#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETEXCEPTION_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETEXCEPTION_HPP

#include <CocktailEngine/Core/GenericException.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketErrorCode.hpp>

namespace Ck
{
    /**
     * \brief Translates SocketErrorCode values into diagnostic messages.
     *
     * The translator produces stable, human-readable descriptions for
     * SocketException. The message is diagnostic only; applications should use
     * SocketException::GetErrorCode to branch on the reported failure.
     */
    class COCKTAILENGINE_CORE_API SocketErrorCodeTranslator : public ErrorCodeTranslator<SocketErrorCode>
    {
    public:

        /**
         * \brief Translates a socket error code into a human-readable message.
         *
         * \param errorCode Socket error to describe.
         *
         * \return Diagnostic message for \p errorCode.
         */
        String TranslateErrorCode(SocketErrorCode errorCode) const override;
    };

    /**
     * \brief Runtime exception that preserves a SocketErrorCode.
     *
     * High-level TCP and UDP wrappers throw this exception when a low-level
     * Socket operation returns an error. Catch it by reference and inspect
     * GetErrorCode rather than parsing its diagnostic message.
     */
    using SocketException = GenericException<SocketErrorCode, SocketErrorCodeTranslator>;
}

#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETEXCEPTION_HPP
