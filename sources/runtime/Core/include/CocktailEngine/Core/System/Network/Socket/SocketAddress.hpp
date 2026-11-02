#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETADDRESS_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETADDRESS_HPP

#include <CocktailEngine/Core/System/Network/Address/IpAddressV4.hpp>
#include <CocktailEngine/Core/System/Network/Address/IpAddressV6.hpp>
#include <CocktailEngine/Core/Utility/Variant.hpp>

namespace Ck
{
    /**
     * \brief Stores a numeric IPv4 or IPv6 endpoint in host representation.
     *
     * SocketAddress owns a copy of the numeric IP address and stores its port in
     * host byte order. The default constructor produces an unset value intended
     * for output parameters such as Socket::Accept and Socket::ReceiveFrom. Do
     * not query the protocol or address of an unset value.
     *
     * The type represents only numeric IPv4 and IPv6 addresses. It does not
     * perform name resolution and does not retain IPv6 scope or flow metadata.
     */
    class COCKTAILENGINE_CORE_API SocketAddress
    {
    public:

        /**
         * \brief Constructs an unset address with port zero.
         *
         * GetProtocol, GetAddressV4, and GetAddressV6 require an address
         * constructed from an IP address or populated by a socket operation.
         */
        SocketAddress();

        /**
         * \brief Constructs an endpoint by copying a numeric IP address and port.
         *
         * \param ipAddress Numeric IPv4 or IPv6 address to copy.
         * \param port Port in host byte order.
         */
        SocketAddress(const IpAddress& ipAddress, unsigned short port);

        /**
         * \brief Gets the port in host byte order.
         *
         * \return The stored port, or zero for a default-constructed address.
         */
        unsigned short GetPort() const;

        /**
         * \brief Gets the protocol of the stored address.
         *
         * \pre This address is set.
         *
         * \return Ipv4 or Ipv6 according to the stored address.
         */
        IpAddress::Protocol GetProtocol() const;

        /**
         * \brief Gets the stored IPv4 address.
         *
         * \pre GetProtocol returns IpAddress::Protocol::Ipv4.
         *
         * \return Reference to the stored IPv4 address, valid while this
         *         SocketAddress remains alive and unmodified.
         */
        const IpAddressV4& GetAddressV4() const;

        /**
         * \brief Gets the stored IPv6 address.
         *
         * \pre GetProtocol returns IpAddress::Protocol::Ipv6.
         *
         * \return Reference to the stored IPv6 address, valid while this
         *         SocketAddress remains alive and unmodified.
         */
        const IpAddressV6& GetAddressV6() const;

    private:

        unsigned short mPort;
        Variant<IpAddressV4, IpAddressV6> mAddress;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETADDRESS_HPP
