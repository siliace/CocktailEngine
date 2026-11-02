#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_TCPLISTENER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_TCPLISTENER_HPP

#include <CocktailEngine/Core/System/Network/TcpClient.hpp>
#include <CocktailEngine/Core/Utility/Time/Duration.hpp>

namespace Ck
{
    /**
     * \brief Listens for incoming TCP connections.
     *
     * A listener starts without a socket. Bind creates and binds its TCP socket;
     * Listen enables incoming connections. Accepted connections are returned as
     * TcpClient instances that own their respective client sockets. Socket
     * failures are reported as SocketException.
     *
     * A TcpListener is not safe for concurrent operations without external
     * synchronization.
     */
    class COCKTAILENGINE_CORE_API TcpListener : public SocketOwner
    {
    public:

        /**
         * \brief Constructs an unbound listener.
         */
        TcpListener() = default;

        /**
         * \brief Creates and binds a listener to a local endpoint.
         *
         * This constructor does not enable listening. Call Listen before
         * accepting connections.
         *
         * \param localPort Local port in host byte order.
         * \param localAddress Numeric IPv4 or IPv6 local address.
         *
         * \throws SocketException If the socket cannot be created or bound.
         */
        TcpListener(unsigned short localPort, const IpAddress& localAddress);

        /**
         * \brief Creates, binds, and starts a listener.
         *
         * \param localPort Local port in host byte order.
         * \param localAddress Numeric IPv4 or IPv6 local address.
         * \param backlog Maximum queued connection requests.
         *
         * \throws SocketException If the socket cannot be created, bound, or
         *         put into listening mode.
         */
        TcpListener(unsigned short localPort, const IpAddress& localAddress, int backlog);

        /**
         * \brief Creates the underlying TCP socket without binding it.
         *
         * Configure socket options that must take effect before Bind, such as
         * address reuse or IPv6-only mode, after this call and before binding.
         *
         * \pre The listener owns no socket.
         *
         * \param protocol IP protocol used to select IPv4 or IPv6.
         *
         * \throws SocketException If the socket cannot be created.
         */
        void Create(IpAddress::Protocol protocol);

        /**
         * \brief Binds the listener to a numeric local IP address and port.
         *
         * Creates the underlying TCP socket if necessary. Binding alone does not
         * enable incoming connections; call Listen afterwards.
         *
         * \param localAddress Numeric IPv4 or IPv6 local address.
         * \param localPort Local port in host byte order. A value of zero asks
         *        the operating system to select an ephemeral port.
         *
         * \throws SocketException If the socket cannot be created or bound.
         */
        void Bind(const IpAddress& localAddress, unsigned short localPort);

        /**
         * \brief Enables incoming TCP connections.
         *
         * \pre The listener owns a socket created by Create or Bind.
         * \pre \p backlog is not zero.
         *
         * \param backlog Maximum queued connection requests. Platform rules apply
         *        to negative values.
         *
         * \throws SocketException If the platform listen operation fails.
         */
        void Listen(int backlog = -1) const;

        /**
         * \brief Waits for and accepts the next incoming connection.
         *
         * The listener is temporarily switched to blocking mode for this call
         * and its prior mode is restored before the method returns or throws.
         *
         * \pre The listener has been bound and put into listening mode.
         *
         * \return A TcpClient that owns the accepted connection.
         *
         * \throws SocketException If accepting the connection fails.
         * \throws SystemException If changing the blocking mode fails.
         */
        TcpClient Accept();

        /**
         * \brief Polls for an incoming connection until a timeout expires.
         *
         * The listener is temporarily switched to non-blocking mode and its
         * prior mode is restored before the method returns or throws. Interrupted
         * accepts are retried immediately; an unavailable connection waits for
         * \p interval before the next attempt.
         *
         * \pre The listener has been bound and put into listening mode.
         *
         * \param timeout Maximum duration to wait for a connection.
         * \param interval Delay between unsuccessful non-blocking accept attempts.
         *
         * \return A TcpClient for the accepted connection, or an empty Optional
         *         when \p timeout expires.
         *
         * \throws SocketException If accepting fails for a reason other than an
         *         interrupted or unavailable non-blocking operation.
         * \throws SystemException If changing the blocking mode fails.
         */
        Optional<TcpClient> TryAccept(const Duration& timeout, const Duration& interval);

        /**
         * \brief Checks whether the listener may bind an address already in use.
         *
         * \pre The listener owns a valid TCP socket.
         *
         * \return \c true when address reuse is enabled, \c false otherwise.
         *
         * \throws SocketException If reading the option fails.
         */
        bool GetReuseAddress() const;

        /**
         * \brief Allows the listener to bind a local address that is not fully free.
         *
         * Address reuse lets a listener take over a port left in a wait state by
         * a previous connection, which is what a restarting server usually
         * wants. The permission granted differs between platforms: Unix-like
         * systems mainly relax that wait state, whereas Windows allows binding
         * an address another socket already holds.
         *
         * The operating system only consults this option while binding. Call
         * Create, then SetReuseAddress, before calling Bind to affect the
         * binding. Calling this method after Bind changes the option on the
         * socket but not the binding already in place.
         *
         * \pre The listener owns a valid TCP socket.
         *
         * \param reuseAddress \c true to permit reuse, \c false to require a
         *        fully free address.
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
         * This setting applies to the listening socket. It does not explicitly
         * configure any TcpClient returned by Accept.
         *
         * \pre The listener owns a valid TCP socket.
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
         * This setting applies to the listening socket. It does not explicitly
         * configure any TcpClient returned by Accept.
         *
         * \pre The listener owns a valid TCP socket.
         *
         * \param bufferSize Requested non-negative receive-buffer size in bytes.
         *
         * \throws SocketException If writing the option fails.
         */
        void SetReceiveBufferSize(unsigned int bufferSize) const;

        /**
         * \brief Gets the timeout for blocking receive operations.
         *
         * Duration::Infinite represents the native zero value, so a zero-length
         * Duration cannot be distinguished from no timeout. The reported value
         * may be rounded by the operating system.
         *
         * This setting applies to the listening socket. It does not explicitly
         * configure any TcpClient returned by Accept.
         *
         * \pre The listener owns a valid TCP socket.
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
         * This setting applies to the listening socket. It does not explicitly
         * configure any TcpClient returned by Accept.
         *
         * \pre The listener owns a valid TCP socket.
         *
         * \param timeout Maximum blocking receive duration. \c Duration::Infinite
         *        and a zero duration both disable the timeout.
         *
         * \throws SocketException If writing the option fails.
         */
        void SetReceiveTimeout(const Duration& timeout) const;

        /**
         * \brief Checks whether the IPv6 listener accepts only IPv6 connections.
         *
         * \pre The listener owns a valid IPv6 TCP socket.
         *
         * \return \c true when IPv4-mapped connections are disabled, \c false
         *         otherwise.
         *
         * \throws SocketException If reading the option fails.
         */
        bool GetIPv6Only() const;

        /**
         * \brief Chooses whether an IPv6 listener accepts IPv4-mapped connections.
         *
         * Configure this option after Create(IpAddress::Protocol::Ipv6) and
         * before Bind. Its default differs between platforms.
         *
         * \pre The listener owns a valid IPv6 TCP socket.
         *
         * \param ipv6Only \c true to accept only IPv6 connections, \c false to
         *        permit IPv4-mapped connections when the operating system allows.
         *
         * \throws SocketException If writing the option fails.
         */
        void SetIpV6Only(bool ipv6Only) const;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_TCPLISTENER_HPP
