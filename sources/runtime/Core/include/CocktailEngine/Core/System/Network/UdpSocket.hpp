#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_UDPSOCKET_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_UDPSOCKET_HPP

#include <CocktailEngine/Core/System/Network/Socket/Socket.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketOwner.hpp>

namespace Ck
{
    /**
     * \brief Provides a throwing, connectionless UDP datagram API.
     *
     * A UdpSocket can be constructed for an IP protocol or created lazily by
     * Bind. Each Send transmits one datagram to its explicit destination, while
     * Receive returns the sender endpoint through a SocketAddress. Socket
     * failures are reported as SocketException.
     *
     * A UdpSocket is not safe for concurrent operations without external
     * synchronization.
     */
    class COCKTAILENGINE_CORE_API UdpSocket : public SocketOwner
    {
    public:

        /**
         * \brief Constructs an invalid socket with no local binding.
         */
        UdpSocket() = default;

        /**
         * \brief Creates an unbound UDP socket for an IP protocol.
         *
         * \param protocol IP protocol used to select IPv4 or IPv6.
         *
         * \throws SocketException If the native socket cannot be created.
         */
        explicit UdpSocket(IpAddress::Protocol protocol);

        /**
         * \brief Creates and binds a UDP socket to a local endpoint.
         *
         * \param localAddress Numeric IPv4 or IPv6 local address.
         * \param localPort Local port in host byte order.
         *
         * \throws SocketException If the socket cannot be created or bound.
         */
        UdpSocket(const IpAddress& localAddress, unsigned short localPort);

        /**
         * \brief Binds the socket to a numeric local IP address and port.
         *
         * Creates the underlying UDP socket if necessary.
         *
         * \param localAddress Numeric IPv4 or IPv6 local address.
         * \param localPort Local port in host byte order. A value of zero asks
         *        the operating system to select an ephemeral port.
         *
         * \throws SocketException If the socket cannot be created or bound.
         */
        void Bind(const IpAddress& localAddress, unsigned short localPort);

        /**
         * \brief Receives one datagram and its sender endpoint.
         *
         * A null buffer or zero length returns zero without reading a datagram.
         * A successful zero-byte receive with a non-empty buffer can represent an
         * empty datagram.
         *
         * \param data Buffer to fill with datagram content.
         * \param length Maximum number of bytes to receive.
         * \param clientAddress Receives a copy of the sender endpoint on success.
         *
         * \return Number of bytes received from the datagram.
         *
         * \throws SocketException If receiving the datagram fails.
         */
        unsigned int Receive(void* data, std::size_t length, SocketAddress& clientAddress) const;

        /**
         * \brief Sends one datagram to a numeric remote endpoint.
         *
         * A null buffer or zero length is a no-op. The datagram must fit the
         * transport and platform message-size limits.
         *
         * \param data Bytes to send.
         * \param length Number of bytes in the datagram.
         * \param address Numeric IPv4 or IPv6 destination address.
         * \param port Destination port in host byte order.
         *
         * \throws SocketException If sending the datagram fails.
         */
        void Send(const void* data, std::size_t length, const IpAddress& address, unsigned short port) const;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_UDPSOCKET_HPP
