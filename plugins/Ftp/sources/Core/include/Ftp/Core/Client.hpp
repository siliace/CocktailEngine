#ifndef COCKTAILENGINE_FTP_CORE_CLIENT_HPP
#define COCKTAILENGINE_FTP_CORE_CLIENT_HPP

#include <CocktailEngine/Core/System/Network/TcpClient.hpp>
#include <CocktailEngine/Core/Utility/TranslatorCast.hpp>

#include <Ftp/Core/Request.hpp>
#include <Ftp/Core/Response.hpp>

namespace Ck::Ftp
{
    class Client
    {
    public:

        explicit Client(const IpAddress& remote, unsigned short port = 21);

        void Send(const Request& request) const;
        Response WaitResponse() const;

        /**
         * \brief Retrieve the address of the server the control connection is established with
         *
         * Useful to open data connections towards the same host.
         *
         * \return The remote address, or an empty optional if it is unknown
         */
        Optional<const IpAddress&> GetRemoteAddress() const
        {
            return mControlSocket.GetRemoteAddress();
        }

    private:

        /**
         * \brief Compute the length of the first complete reply of a received content
         *
         * \return The length, including the terminator, or an empty optional if more bytes must be received
         */
        static Optional<unsigned int> GetResponseLength(const Utf8String& content);

        TcpClient mControlSocket;
        mutable Utf8String mPending;
    };
}

#endif // COCKTAILENGINE_FTP_CORE_CLIENT_HPP
