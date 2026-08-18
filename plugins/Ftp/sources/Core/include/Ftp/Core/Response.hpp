#ifndef COCKTAILENGINE_FTP_CORE_RESPONSE_HPP
#define COCKTAILENGINE_FTP_CORE_RESPONSE_HPP

#include <CocktailEngine/Core/Exception.hpp>
#include <CocktailEngine/Core/IO/Output/Writer/Writer.hpp>
#include <CocktailEngine/Core/String.hpp>

namespace Ck::Ftp
{
    /**
     * \brief Thrown when a control connection is closed by the peer
     */
    COCKTAIL_DECLARE_EXCEPTION_FROM(ConnectionClosedException, RuntimeException);

    /**
     * \brief Represents a reply sent by an FTP server to a client
     *
     * A response is composed of a three-digit status \ref Code and a textual
     * \ref Message. The class offers helpers to classify the reply according
     * to the ranges defined by RFC 959, and is able to both parse an incoming
     * reply (client side) and serialize an outgoing one (server side).
     */
    class Response
    {
    public:

        /**
         * \brief Tells whether the reply indicates a positive completion (2xx)
         */
        bool IsPositiveCompletion() const
        {
            return Code >= 200 && Code < 300;
        }

        /**
         * \brief Tells whether the reply is a positive preliminary reply (1xx)
         */
        bool IsPreliminary() const
        {
            return Code >= 100 && Code < 200;
        }

        /**
         * \brief Tells whether the reply is a positive intermediate reply (3xx)
         */
        bool IsIntermediate() const
        {
            return Code >= 300 && Code < 400;
        }

        /**
         * \brief Tells whether the reply indicates a failure (4xx or 5xx)
         */
        bool IsNegativeCompletion() const
        {
            return Code >= 400 && Code < 600;
        }

        /**
         * \brief Serialize the response onto the given writer
         *
         * The reply is written following the single-line format defined by
         * RFC 959, i.e. the three-digit code, a space and the message,
         * terminated by the FTP message terminator (`\r\n`). Multi-line
         * messages (those containing embedded line terminators) are emitted
         * using the long-reply form.
         *
         * \param writer The writer the response is serialized onto
         */
        void Serialize(Writer<Encoders::Utf8>& writer) const;

        /**
         * \brief Parse a raw reply received from the control connection
         *
         * \param content The raw reply, including the trailing terminator
         *
         * \return The parsed response
         */
        static Response Parse(const Utf8String& content);

        Uint16 Code;
        Utf8String Message;
    };
}

#endif // COCKTAILENGINE_FTP_CORE_RESPONSE_HPP
