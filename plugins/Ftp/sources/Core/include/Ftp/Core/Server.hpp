#ifndef COCKTAILENGINE_FTP_CORE_SERVER_HPP
#define COCKTAILENGINE_FTP_CORE_SERVER_HPP

#include <CocktailEngine/Core/System/Network/TcpClient.hpp>
#include <CocktailEngine/Core/System/Network/TcpServer.hpp>
#include <CocktailEngine/Core/Utility/Time/Duration.hpp>

#include <Ftp/Core/Request.hpp>
#include <Ftp/Core/Response.hpp>

namespace Ck::Ftp
{
    /**
     * \brief A control connection accepted by a \ref Server
     *
     * A connection is the server-side counterpart of a \ref Client: it reads
     * \ref Request commands sent by a remote client and replies with
     * \ref Response messages over the same control channel.
     */
    class Connection
    {
    public:

        /**
         * \brief Constructor
         *
         * \param socket The accepted control socket
         */
        explicit Connection(TcpClient socket);

        /**
         * \brief Read the next command sent by the remote client
         *
         * Blocks until a complete command line is received. Unknown commands
         * yield an empty optional while the line is otherwise consumed.
         *
         * \return The parsed request, or an empty optional on an unknown command
         *
         * \throw ConnectionClosedException The peer closed the connection
         */
        Optional<GenericRequest> ReadRequest() const;

        /**
         * \brief Send a reply to the remote client
         *
         * \param response The response to serialize onto the control channel
         */
        void SendResponse(const Response& response) const;

        /**
         * \brief Retrieve the underlying control socket
         */
        const TcpClient& GetSocket() const
        {
            return mControlSocket;
        }

    private:

        TcpClient mControlSocket;
    };

    /**
     * \brief Listens for incoming FTP control connections
     *
     * The server owns the listening socket and hands out \ref Connection
     * objects, one per accepted client, through which requests are read and
     * responses are written.
     */
    class Server
    {
    public:

        /**
         * \brief Constructor
         *
         * \param port The port the control socket listens on
         * \param host The local address to bind to
         */
        explicit Server(unsigned short port = 21, const IpAddress& host = IpAddressV4::Any);

        /**
         * \brief Block until a client connects
         *
         * \return The accepted connection
         */
        Connection Accept() const;

        /**
         * \brief Attempt to accept a client within a bounded amount of time
         *
         * \param timeout The maximum time to wait for a client
         * \param interval The polling interval
         *
         * \return The accepted connection, or an empty optional on timeout
         */
        Optional<Connection> TryAccept(const Duration& timeout, const Duration& interval) const;

    private:

        TcpServer mControlSocket;
    };
}

#endif // COCKTAILENGINE_FTP_CORE_SERVER_HPP
