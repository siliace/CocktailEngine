#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_TCPCLIENT_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_TCPCLIENT_HPP

#include <CocktailEngine/Core/IO/Input/Stream/InputStream.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketOwner.hpp>
#include <CocktailEngine/Core/Utility/Optional.hpp>

namespace Ck
{
    /**
     * \brief Provides a throwing, stream-oriented TCP client API.
     *
     * A client starts disconnected. Bind and Connect create the underlying TCP
     * socket when necessary, while the convenience constructors perform those
     * operations immediately. Socket failures are reported as SocketException.
     * A TcpClient is not safe for concurrent calls without external
     * synchronization.
     */
    class COCKTAILENGINE_CORE_API TcpClient : public SocketOwner
    {
    public:

        /**
         * \brief Constructs a disconnected client.
         */
        TcpClient() = default;

        /**
         * \brief Connects to a remote numeric IP address.
         *
         * \param address Numeric IPv4 or IPv6 address of the remote peer.
         * \param port Remote port in host byte order.
         *
         * \throws SocketException If the socket cannot be created or connected.
         */
        TcpClient(const IpAddress& address, unsigned short port);

        /**
         * \brief Binds a local endpoint, then connects to a remote endpoint.
         *
         * \param address Numeric IPv4 or IPv6 address of the remote peer.
         * \param port Remote port in host byte order.
         * \param localAddress Numeric local IPv4 or IPv6 address to bind.
         * \param localPort Local port in host byte order.
         *
         * \throws SocketException If the socket cannot be created, bound, or
         *         connected.
         */
        TcpClient(const IpAddress& address, unsigned short port, const IpAddress& localAddress, unsigned short localPort);

        /**
         * \brief Creates the underlying TCP socket without binding or connecting it.
         *
         * Configure socket options that must take effect before Bind or Connect,
         * such as address reuse or IPv6-only mode, after this call and before
         * establishing an endpoint.
         *
         * \pre The client owns no socket.
         *
         * \param protocol IP protocol used to select IPv4 or IPv6.
         *
         * \throws SocketException If the socket cannot be created.
         */
        void Create(IpAddress::Protocol protocol);

        /**
         * \brief Binds the client socket to a local numeric IP address and port.
         *
         * Creates the underlying TCP socket if necessary. Binding alone does not
         * establish a remote connection.
         *
         * \pre \p localPort is not zero.
         *
         * \param localAddress Numeric IPv4 or IPv6 local address.
         * \param localPort Local port in host byte order.
         *
         * \throws SocketException If the socket cannot be created or bound.
         */
        void Bind(const IpAddress& localAddress, unsigned short localPort);

        /**
         * \brief Connects the client socket to a remote numeric endpoint.
         *
         * Creates the underlying TCP socket if necessary.
         *
         * \pre \p port is not zero.
         *
         * \param address Numeric IPv4 or IPv6 address of the remote peer.
         * \param port Remote port in host byte order.
         *
         * \throws SocketException If the socket cannot be created or connected.
         */
        void Connect(const IpAddress& address, unsigned short port);

        /**
         * \brief Sends every byte in a buffer through the connected stream.
         *
         * This method continues after partial native sends until the complete
         * buffer has been accepted or an error occurs. A null buffer or zero
         * length is a no-op.
         *
         * \param data Bytes to send.
         * \param length Number of bytes to send.
         *
         * \throws SocketException If the native send operation fails.
         */
        void Send(const void* data, unsigned int length) const;

        /**
         * \brief Receives available bytes from the connected stream.
         *
         * A successful result of zero for a non-empty buffer means the remote
         * peer performed an orderly shutdown. A null buffer or zero length
         * returns zero without calling the native socket API.
         *
         * \param data Buffer to fill.
         * \param length Maximum number of bytes to receive.
         *
         * \return Number of bytes received, which can be less than \p length.
         *
         * \throws SocketException If the native receive operation fails.
         */
        unsigned int Receive(void* data, unsigned int length) const;

        /**
         * \brief Closes the connection and clears its stored endpoint snapshots.
         */
        void Close();

        /**
         * \brief Gets the connected peer address when one is known.
         *
         * \return An Optional containing a borrowed peer address, or an empty
         *         Optional when the client has no remote endpoint.
         */
        Optional<const IpAddress&> GetRemoteAddress() const;

        /**
         * \brief Gets the connected peer port when one is known.
         *
         * \return An Optional containing the peer port in host byte order, or an
         *         empty Optional when the client has no remote endpoint.
         */
        Optional<unsigned short> GetRemotePort() const;

        /**
         * \brief Checks whether small writes are sent without being coalesced.
         *
         * \pre The client owns a connected socket.
         *
         * \return \c true when coalescing is disabled, \c false otherwise.
         *
         * \throws SocketException If reading the option fails.
         */
        bool GetNoDelay() const;

        /**
         * \brief Sends small writes immediately instead of coalescing them.
         *
         * Disables the delay a TCP stack applies to gather small writes into
         * fewer segments. Worth enabling for latency-sensitive exchanges of
         * short messages, at the cost of putting more packets on the wire. The
         * setting concerns an established connection and may be changed at any
         * point during its lifetime.
         *
         * \pre The client owns a connected socket.
         *
         * \param noDelay \c true to send each write as soon as the transport
         *        allows, \c false to let the stack coalesce them.
         *
         * \throws SocketException If writing the option fails.
         */
        void SetNoDelay(bool noDelay) const;

        /**
         * \brief Checks whether TCP keep-alive probes are enabled.
         *
         * Keep-alive lets the transport detect an unavailable peer while a
         * connection is otherwise idle. The idle delay, probe interval, and
         * probe count are configured by the operating system and are not
         * exposed by TcpClient.
         *
         * \pre The client owns a valid TCP socket.
         *
         * \return \c true when keep-alive is enabled, \c false otherwise.
         *
         * \throws SocketException If reading the option fails.
         */
        bool GetKeepAlive() const;

        /**
         * \brief Enables or disables TCP keep-alive probes.
         *
         * \pre The client owns a valid TCP socket.
         *
         * \param keepAlive \c true to enable idle-peer probes, \c false to
         *        disable them.
         *
         * \throws SocketException If writing the option fails.
         */
        void SetKeepAlive(bool keepAlive) const;

        /**
         * \brief Checks whether urgent data is delivered in the receive stream.
         *
         * When disabled, urgent data requires an out-of-band read, which
         * TcpClient does not expose.
         *
         * \pre The client owns a valid TCP socket.
         *
         * \return \c true when urgent data is delivered inline, \c false
         *         otherwise.
         *
         * \throws SocketException If reading the option fails.
         */
        bool GetOutOfBandInline() const;

        /**
         * \brief Chooses whether urgent data is delivered in the receive stream.
         *
         * \pre The client owns a valid TCP socket.
         *
         * \param outOfBandInline \c true to deliver urgent data inline, \c false
         *        to require an out-of-band read.
         *
         * \throws SocketException If writing the option fails.
         */
        void SetOutOfBandInline(bool outOfBandInline) const;

        /**
         * \brief Checks whether local-address reuse is enabled.
         *
         * \pre The client owns a valid TCP socket.
         *
         * \return \c true when address reuse is enabled, \c false otherwise.
         *
         * \throws SocketException If reading the option fails.
         */
        bool GetReuseAddress() const;

        /**
         * \brief Requests reuse of a local address while binding.
         *
         * The operating system consults this option while binding. TcpClient
         * creates and binds its socket in the same Bind call, so setting this
         * option cannot affect a bind that has already occurred, including an
         * implicit bind performed by Connect. Its exact permission differs
         * between platforms.
         *
         * \pre The client owns a valid TCP socket.
         *
         * \param reuseAddress \c true to request reuse, \c false to disable it.
         *
         * \throws SocketException If writing the option fails.
         */
        void SetReuseAddress(bool reuseAddress) const;

        /**
         * \brief Gets the operating system receive-buffer size in bytes.
         *
         * The reported value can differ from the value requested through
         * SetReceiveBufferSize because the operating system may clamp, round,
         * or account for its own bookkeeping.
         *
         * \pre The client owns a valid TCP socket.
         *
         * \return The currently reported receive-buffer size in bytes.
         *
         * \throws SocketException If reading the option fails.
         */
        unsigned int GetReceiveBufferSize() const;

        /**
         * \brief Requests a receive-buffer size in bytes.
         *
         * The operating system may clamp or round the request. Read the option
         * back to obtain the size it reports.
         *
         * \pre The client owns a valid TCP socket.
         *
         * \param bufferSize Requested non-negative receive-buffer size in bytes.
         *
         * \throws SocketException If writing the option fails.
         */
        void SetReceiveBufferSize(unsigned int bufferSize) const;

        /**
         * \brief Gets the operating system send-buffer size in bytes.
         *
         * The reported value can differ from the value requested through
         * SetSendBufferSize because the operating system may clamp, round, or
         * account for its own bookkeeping.
         *
         * \pre The client owns a valid TCP socket.
         *
         * \return The currently reported send-buffer size in bytes.
         *
         * \throws SocketException If reading the option fails.
         */
        unsigned int GetSendBufferSize() const;

        /**
         * \brief Requests a send-buffer size in bytes.
         *
         * The operating system may clamp or round the request. Read the option
         * back to obtain the size it reports.
         *
         * \pre The client owns a valid TCP socket.
         *
         * \param bufferSize Requested non-negative send-buffer size in bytes.
         *
         * \throws SocketException If writing the option fails.
         */
        void SetSendBufferSize(unsigned int bufferSize) const;

        /**
         * \brief Gets how closing the socket handles pending data.
         *
         * \pre The client owns a valid TCP socket.
         *
         * \return The current lingering configuration.
         *
         * \throws SocketException If reading the option fails.
         */
        SocketLinger GetLinger() const;

        /**
         * \brief Configures how closing the socket handles pending data.
         *
         * When lingering is disabled, closing returns immediately and the
         * transport flushes pending data in the background. When enabled, a
         * blocking close waits until the data is sent and acknowledged or the
         * timeout expires. A zero timeout discards pending data and resets the
         * connection. The native timeout has whole-second resolution, and the
         * behaviour of a non-blocking close is platform-defined.
         *
         * \pre The client owns a valid TCP socket.
         *
         * \param linger Closing behaviour to apply.
         *
         * \throws SocketException If writing the option fails.
         */
        void SetLinger(const SocketLinger& linger) const;

        /**
         * \brief Gets the timeout for blocking receive operations.
         *
         * Duration::Infinite represents the native zero value, so a zero-length
         * Duration cannot be distinguished from no timeout. The reported value
         * may be rounded by the operating system.
         *
         * \pre The client owns a valid TCP socket.
         *
         * \return The currently reported receive timeout.
         *
         * \throws SocketException If reading the option fails.
         */
        Duration GetReceiveTimeout() const;

        /**
         * \brief Sets the timeout for blocking receive operations.
         *
         * Non-blocking operations report SocketErrorCode::WouldBlock without
         * waiting. A timeout can be reported as SocketErrorCode::WouldBlock or
         * SocketErrorCode::TimedOut depending on the platform.
         *
         * \pre The client owns a valid TCP socket.
         *
         * \param timeout Maximum blocking receive duration. \c Duration::Infinite
         *        and a zero duration both disable the timeout.
         *
         * \throws SocketException If writing the option fails.
         */
        void SetReceiveTimeout(const Duration& timeout) const;

        /**
         * \brief Gets the timeout for blocking send operations.
         *
         * Duration::Infinite represents the native zero value, so a zero-length
         * Duration cannot be distinguished from no timeout. The reported value
         * may be rounded by the operating system.
         *
         * \pre The client owns a valid TCP socket.
         *
         * \return The currently reported send timeout.
         *
         * \throws SocketException If reading the option fails.
         */
        Duration GetSendTimeout() const;

        /**
         * \brief Sets the timeout for blocking send operations.
         *
         * Non-blocking operations report SocketErrorCode::WouldBlock without
         * waiting. A send that times out can leave an undefined number of bytes
         * transferred, so the stream should be considered unusable afterwards.
         *
         * \pre The client owns a valid TCP socket.
         *
         * \param timeout Maximum blocking send duration. \c Duration::Infinite
         *        and a zero duration both disable the timeout.
         *
         * \throws SocketException If writing the option fails.
         */
        void SetSendTimeout(const Duration& timeout) const;

        /**
         * \brief Gets the number of bytes currently queued for reading.
         *
         * \return Snapshot of immediately readable bytes.
         *
         * \throws SystemException If the underlying native query fails.
         */
        unsigned int GetPendingLength() const;

    private:

        friend class TcpListener;

        /**
         * \brief Takes ownership of an accepted socket and its peer endpoint.
         *
         * \param socket Connected socket whose ownership is transferred here.
         * \param remoteAddress Endpoint of the connected peer.
         */
        TcpClient(Socket socket, SocketAddress remoteAddress);

        SocketAddress mRemoteAddress;
    };

    /**
     * \brief Presents a TcpClient receive stream as an InputStream.
     *
     * The referenced TcpClient must outlive this stream. A read that returns
     * zero marks this stream as EOF and subsequent reads return zero.
     */
    class TcpClientInputStream : public InputStream<>
    {
    public:

        /**
         * \brief Attaches the stream to a TCP client without taking ownership.
         *
         * \param tcpClient Client from which bytes are read.
         */
        explicit TcpClientInputStream(const TcpClient& tcpClient) :
            mTcpClient(&tcpClient),
            mReceiveClosed(false)
        {
            /// Nothing
        }

        /**
         * \brief Reads bytes from the attached TCP client.
         *
         * \param buffer Buffer to fill.
         * \param length Maximum number of bytes to read.
         *
         * \return Number of bytes received, or zero at EOF or for a zero length.
         */
        SizeType Read(Byte* buffer, SizeType length) override
        {
            if (mReceiveClosed || length == 0)
                return 0;

            SizeType received = mTcpClient->Receive(buffer, length);
            if (received == 0)
                mReceiveClosed = true;

            return received;
        }

        /**
         * \brief Indicates that TCP streams cannot seek.
         *
         * \return \c false.
         */
        bool HasCursor() const override
        {
            return false;
        }

        /**
         * \brief Is unsupported for TCP streams.
         */
        Uint64 Seek(Uint64 position) override
        {
            ExceptionUtils::ThrowNotImplemented();
        }

        /**
         * \brief Is unsupported for TCP streams.
         */
        void Rewind() override
        {
            ExceptionUtils::ThrowNotImplemented();
        }

        /**
         * \brief Is unsupported for TCP streams.
         */
        Uint64 Tell() const override
        {
            ExceptionUtils::ThrowNotImplemented();
        }

        /**
         * \brief Gets the current receive-queue size.
         *
         * \return Zero at EOF; otherwise, a snapshot of bytes immediately ready
         *         to read from the attached client.
         */
        SizeType GetSize() const override
        {
            if (mReceiveClosed)
                return 0;

            return mTcpClient->GetPendingLength();
        }

        /**
         * \brief Checks whether this stream observed a remote shutdown.
         *
         * \return \c true after Read has returned zero for the attached client.
         */
        bool IsEof() const override
        {
            return mReceiveClosed;
        }

    private:

        const TcpClient* mTcpClient;
        bool mReceiveClosed;
    };

    /**
     * \brief Presents a TcpClient send stream as an OutputStream.
     *
     * The referenced TcpClient must outlive this stream. Writes delegate to
     * TcpClient::Send, which attempts to send the entire buffer.
     */
    class TcpClientOutputStream : public OutputStream<>
    {
    public:

        /**
         * \brief Attaches the stream to a TCP client without taking ownership.
         *
         * \param tcpClient Client through which bytes are written.
         */
        explicit TcpClientOutputStream(const TcpClient& tcpClient) :
            mTcpClient(&tcpClient)
        {
            /// Nothing
        }

        /**
         * \brief Sends the supplied bytes through the attached client.
         *
         * \param data Bytes to send.
         * \param length Number of bytes to send.
         *
         * \return \p length after a successful send.
         */
        SizeType Write(const Byte* data, SizeType length) override
        {
            mTcpClient->Send(data, length);
            return length;
        }

        /**
         * \brief Performs no operation because socket sends are immediate.
         */
        void Flush() override
        {
        }

    private:

        const TcpClient* mTcpClient;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_TCPCLIENT_HPP
