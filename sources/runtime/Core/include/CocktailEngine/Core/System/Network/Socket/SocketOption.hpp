#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETOPTION_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETOPTION_HPP

#include <CocktailEngine/Core/System/Network/Address/IpAddressV4.hpp>
#include <CocktailEngine/Core/Utility/Time/Duration.hpp>

namespace Ck
{
    /**
     * \brief Identifies the protocol layer that interprets a socket option.
     *
     * An option is only meaningful at one level, and the pair formed by a level
     * and a SocketOptionId is fixed by the SocketOptions constants. Callers
     * select an option through those constants rather than by combining a level
     * and an identifier themselves.
     */
    enum class SocketOptionLevel
    {
        Socket, /**< Option handled by the socket layer itself, regardless of protocol. */
        IPv4, /**< Option handled by the IPv4 layer. */
        IPv6, /**< Option handled by the IPv6 layer. */
        Tcp /**< Option handled by the TCP layer. */
    };

    /**
     * \brief Identifies a socket option within its SocketOptionLevel.
     *
     * Each value names the native option it is translated to. The translation is
     * performed inside Socket, so this enumeration stays free of platform
     * headers. Several values have no matching SocketOptions constant yet
     * because they need a payload type the public API does not model; they are
     * marked accordingly and must not be passed to Socket.
     */
    enum class SocketOptionId
    {
        // Socket level
        AcceptConnection, /**< Maps to SO_ACCEPTCONN. */
        ReuseAddress, /**< Maps to SO_REUSEADDR. */
        KeepAlive, /**< Maps to SO_KEEPALIVE. */
        Broadcast, /**< Maps to SO_BROADCAST. */
        SendBuffer, /**< Maps to SO_SNDBUF. */
        ReceiveBuffer, /**< Maps to SO_RCVBUF. */
        SendTimeout, /**< Maps to SO_SNDTIMEO. */
        ReceiveTimeout, /**< Maps to SO_RCVTIMEO. */
        Linger, /**< Maps to SO_LINGER. */
        OutOfBandInline, /**< Maps to SO_OOBINLINE. */
        Error, /**< Maps to SO_ERROR. */
        Type, /**< Maps to SO_TYPE. */

        // IPv4 level
        IpHeaderIncluded, /**< Maps to IP_HDRINCL. */
        IpOptions, /**< Maps to IP_OPTIONS. Not exposed: needs a byte-buffer payload. */
        IpTimeToLive, /**< Maps to IP_TTL. */
        IpMulticastInterface, /**< Maps to IP_MULTICAST_IF. */
        IpMulticastTimeToLive, /**< Maps to IP_MULTICAST_TTL. */
        IpMulticastLoop, /**< Maps to IP_MULTICAST_LOOP. */
        IpAddMembership, /**< Maps to IP_ADD_MEMBERSHIP. Not exposed: needs an ip_mreq payload. */
        IpDropMembership, /**< Maps to IP_DROP_MEMBERSHIP. Not exposed: needs an ip_mreq payload. */

        // IPv6 level
        IPv6Only, /**< Maps to IPV6_V6ONLY. */
        IPv6UnicastHopLimit, /**< Maps to IPV6_UNICAST_HOPS. */
        IPv6MulticastInterface, /**< Maps to IPV6_MULTICAST_IF. */
        IPv6MulticastHopLimit, /**< Maps to IPV6_MULTICAST_HOPS. */
        IPv6MulticastLoop, /**< Maps to IPV6_MULTICAST_LOOP. */
        IPv6AddMembership, /**< Maps to IPV6_JOIN_GROUP. Not exposed: needs an ipv6_mreq payload. */
        IPv6DropMembership, /**< Maps to IPV6_LEAVE_GROUP. Not exposed: needs an ipv6_mreq payload. */

        // TCP level
        TcpNoDelay /**< Maps to TCP_NODELAY. */
    };

    /**
     * \brief Names a socket option together with the type of its value.
     *
     * A SocketOption carries no state beyond the level and identifier that
     * select a native option, and binds them to the C++ type used to read and
     * write that option. Socket::SetOption and Socket::GetOption take a
     * SocketOption and deduce the value type from it, so an option cannot be
     * read or written through a type it was not declared with.
     *
     * Instances are meant to be the constants declared in the SocketOptions
     * namespace. Building one directly is possible but pairs a level and an
     * identifier without any check that the combination is valid.
     *
     * \tparam T Type exchanged with the caller for this option. Socket converts
     *         it to and from the layout the native API expects.
     */
    template <typename T>
    class SocketOption
    {
    public:

        /**
         * \brief Binds an option identifier to the level that interprets it.
         *
         * \pre \p id is an option of the layer denoted by \p level.
         *
         * \param level Protocol layer that interprets the option.
         * \param id Option to designate within that layer.
         */
        constexpr SocketOption(SocketOptionLevel level, SocketOptionId id) :
            mLevel(level),
            mId(id)
        {
            /// Nothing
        }

        /**
         * \brief Gets the protocol layer that interprets this option.
         *
         * \return The option level.
         */
        constexpr SocketOptionLevel GetLevel() const
        {
            return mLevel;
        }

        /**
         * \brief Gets the identifier of this option within its level.
         *
         * \return The option identifier.
         */
        constexpr SocketOptionId GetId() const
        {
            return mId;
        }

    private:

        SocketOptionLevel mLevel;
        SocketOptionId mId;
    };

    /**
     * \brief Value of the Linger option, which governs how a socket closes.
     *
     * When lingering is disabled, closing a socket returns immediately and the
     * transport flushes any pending data in the background. When it is enabled,
     * closing blocks until the data has been sent and acknowledged or until the
     * timeout expires; a timeout of zero discards pending data and resets the
     * connection instead of shutting it down in an orderly way.
     *
     * Lingering only delays a close on a blocking socket. The behaviour of a
     * non-blocking close with lingering enabled is platform-defined.
     */
    struct SocketLinger
    {
        bool Enabled = false; /**< Whether closing the socket waits for pending data. */
        Duration Timeout = Duration::Seconds(0); /**< Duration a close may block while \c Enabled is \c true. */
    };

    /**
     * \brief Options accepted by Socket::SetOption and Socket::GetOption.
     *
     * Every constant fixes an option level, an option identifier and the value
     * type to exchange with the caller. Only options that every target platform
     * implements with the same observable effect are exposed here. An option
     * that one platform lacks, silently ignores or documents as unusable is
     * deliberately absent rather than offered and left to fail, so that code
     * written against this API behaves the same everywhere.
     *
     * Being exposed does not make an option applicable to every socket. Support
     * still depends on the protocol and on the state of the socket: several
     * options are meaningful only for a stream or only for a datagram socket,
     * and some take effect only before a bind or before a connect. A call that
     * the operating system rejects reports the error rather than being ignored.
     *
     * Reading an option back does not necessarily return the value that was
     * written. Buffer sizes and timeouts in particular may be rounded, clamped
     * or doubled for internal bookkeeping by the operating system.
     *
     * Options marked read-only are rejected by the operating system when
     * written, since the type system does not currently prevent it.
     */
    namespace SocketOptions
    {
        /** \brief Read-only. Whether the socket is listening for connections. */
        inline constexpr SocketOption<bool> AcceptConnection(SocketOptionLevel::Socket, SocketOptionId::AcceptConnection);

        /**
         * \brief Allows binding a local address that is not fully free.
         *
         * Set before Socket::Bind to rebind a port left in a wait state by a
         * previous connection, which a listener normally wants. The exact
         * permission granted differs between platforms: Unix-like systems mainly
         * relax the wait state and allow several sockets to join the same
         * multicast port, whereas Windows allows binding an address already in
         * use by another socket.
         */
        inline constexpr SocketOption<bool> ReuseAddress(SocketOptionLevel::Socket, SocketOptionId::ReuseAddress);

        /**
         * \brief Sends periodic probes on an idle connection.
         *
         * Lets a peer failure be detected on a connection that carries no
         * traffic. The idle delay and probe count are configured outside this
         * option and default to values measured in hours on most systems.
         */
        inline constexpr SocketOption<bool> KeepAlive(SocketOptionLevel::Socket, SocketOptionId::KeepAlive);

        /**
         * \brief Permits sending to a broadcast address.
         *
         * Required before a datagram socket may send to a broadcast address, and
         * meaningful for IPv4 only, as IPv6 has no broadcast.
         */
        inline constexpr SocketOption<bool> Broadcast(SocketOptionLevel::Socket, SocketOptionId::Broadcast);

        /**
         * \brief Size in bytes of the send buffer reserved for the socket.
         *
         * Treated as a request: the operating system may clamp it to a
         * configured maximum, and Linux stores roughly twice the requested size
         * to account for its own bookkeeping, so reading the option back
         * commonly reports a different value.
         */
        inline constexpr SocketOption<unsigned int> SendBuffer(SocketOptionLevel::Socket, SocketOptionId::SendBuffer);

        /**
         * \brief Size in bytes of the receive buffer reserved for the socket.
         *
         * Subject to the same clamping and rounding as SendBuffer. On a TCP
         * socket it influences, but does not define, the advertised receive
         * window.
         */
        inline constexpr SocketOption<unsigned int> ReceiveBuffer(SocketOptionLevel::Socket, SocketOptionId::ReceiveBuffer);

        /**
         * \brief Time in milliseconds a blocking send may wait before failing.
         *
         * A value of zero, the default, means a send waits indefinitely. The
         * timeout applies to blocking sockets; a non-blocking socket reports
         * SocketErrorCode::WouldBlock without waiting. A send that times out
         * leaves an undefined number of bytes transferred, so a stream socket
         * should be considered unusable afterwards.
         */
        inline constexpr SocketOption<Duration> SendTimeout(SocketOptionLevel::Socket, SocketOptionId::SendTimeout);

        /**
         * \brief Time in milliseconds a blocking receive may wait before failing.
         *
         * A value of zero, the default, means a receive waits indefinitely. A
         * receive that reaches the timeout fails with
         * SocketErrorCode::WouldBlock or SocketErrorCode::TimedOut depending on
         * the platform, so callers that rely on the timeout should accept both.
         */
        inline constexpr SocketOption<Duration> ReceiveTimeout(SocketOptionLevel::Socket, SocketOptionId::ReceiveTimeout);

        /** \brief How closing the socket handles data still pending. See SocketLinger. */
        inline constexpr SocketOption<SocketLinger> Linger(SocketOptionLevel::Socket, SocketOptionId::Linger);

        /**
         * \brief Delivers out-of-band data in the normal receive stream.
         *
         * When disabled, urgent data is held aside and only readable through a
         * dedicated out-of-band read, which Socket does not expose.
         */
        inline constexpr SocketOption<bool> OutOfBandInline(SocketOptionLevel::Socket, SocketOptionId::OutOfBandInline);

        /**
         * \brief Read-only. Pending error on the socket, as a native error code.
         *
         * Reading this option also clears the pending error, so a second read
         * reports none. It is the usual way to learn the outcome of a
         * non-blocking Socket::Connect once the socket reports as writable: zero
         * means the connection succeeded. The value is the native code and not a
         * SocketErrorCode.
         */
        inline constexpr SocketOption<int> Error(SocketOptionLevel::Socket, SocketOptionId::Error);

        /**
         * \brief Read-only. Transport type of the socket, as a native value.
         *
         * Reports the native constant, such as SOCK_STREAM or SOCK_DGRAM, and
         * not the Socket::Type value the socket was created with.
         */
        inline constexpr SocketOption<int> Type(SocketOptionLevel::Socket, SocketOptionId::Type);

        /**
         * \brief The application supplies the IPv4 header of outgoing packets.
         *
         * Applies to raw sockets, whose creation usually requires privileges.
         */
        inline constexpr SocketOption<bool> IpHeaderIncluded(SocketOptionLevel::IPv4, SocketOptionId::IpHeaderIncluded);

        /** \brief Hop count written in the time-to-live field of outgoing IPv4 headers. */
        inline constexpr SocketOption<int> IpTimeToLive(SocketOptionLevel::IPv4, SocketOptionId::IpTimeToLive);

        /**
         * \brief Local interface used to send IPv4 multicast, given by its address.
         *
         * Selects which interface carries outgoing multicast when the host has
         * several. The address designates a local interface, not a destination.
         */
        inline constexpr SocketOption<IpAddressV4> IpMulticastInterface(SocketOptionLevel::IPv4, SocketOptionId::IpMulticastInterface);

        /**
         * \brief Hop count applied to outgoing IPv4 multicast.
         *
         * Defaults to one, which confines multicast to the local link. Raising
         * it lets datagrams cross routers that forward multicast.
         */
        inline constexpr SocketOption<int> IpMulticastTimeToLive(SocketOptionLevel::IPv4, SocketOptionId::IpMulticastTimeToLive);

        /**
         * \brief Delivers outgoing IPv4 multicast back to local member sockets.
         *
         * Enabled by default, so a sender that also joined the group receives
         * its own datagrams.
         */
        inline constexpr SocketOption<bool> IpMulticastLoop(SocketOptionLevel::IPv4, SocketOptionId::IpMulticastLoop);

        /**
         * \brief Restricts an IPv6 socket to IPv6 traffic.
         *
         * When disabled, a socket bound to the IPv6 wildcard address also
         * accepts IPv4 peers through addresses mapped into IPv6. The default
         * differs by platform: Windows enables this option, while Linux usually
         * disables it and accepts both families. Set it explicitly whenever the
         * behaviour matters.
         */
        inline constexpr SocketOption<bool> IPv6Only(SocketOptionLevel::IPv6, SocketOptionId::IPv6Only);

        /** \brief Hop limit written in outgoing IPv6 unicast headers. */
        inline constexpr SocketOption<int> IPv6UnicastHopLimit(SocketOptionLevel::IPv6, SocketOptionId::IPv6UnicastHopLimit);

        /**
         * \brief Local interface used to send IPv6 multicast, given by its index.
         *
         * Unlike IpMulticastInterface, which takes an address, IPv6 designates
         * the interface by the index the operating system assigns it. A value of
         * zero lets the system choose.
         */
        inline constexpr SocketOption<unsigned int> IPv6MulticastInterface(SocketOptionLevel::IPv6, SocketOptionId::IPv6MulticastInterface);

        /**
         * \brief Hop limit applied to outgoing IPv6 multicast.
         *
         * Defaults to one, confining multicast to the local link.
         */
        inline constexpr SocketOption<int> IPv6MulticastHopLimit(SocketOptionLevel::IPv6, SocketOptionId::IPv6MulticastHopLimit);

        /** \brief Delivers outgoing IPv6 multicast back to local member sockets. */
        inline constexpr SocketOption<bool> IPv6MulticastLoop(SocketOptionLevel::IPv6, SocketOptionId::IPv6MulticastLoop);

        /**
         * \brief Sends small writes immediately instead of coalescing them.
         *
         * Disables the delay a TCP stack applies to gather small writes into
         * fewer segments. Worth enabling for latency-sensitive exchanges of
         * short messages, at the cost of more packets on the wire.
         */
        inline constexpr SocketOption<bool> TcpNoDelay(SocketOptionLevel::Tcp, SocketOptionId::TcpNoDelay);
    }
}

#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETOPTION_HPP
