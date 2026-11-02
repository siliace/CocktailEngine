#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKETADDRESSCONVERTER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKETADDRESSCONVERTER_HPP

#include <CocktailEngine/Core/System/Network/Address/IpAddressV4.hpp>
#include <CocktailEngine/Core/System/Network/Address/IpAddressV6.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketHeader.hpp>

namespace Ck
{
    /**
     * \brief Adapts engine IP addresses to and from native socket addresses.
     *
     * This internal type keeps platform sockaddr definitions out of the public
     * network API. It stores either a sockaddr_in or sockaddr_in6 and preserves
     * the native address length. Ports are converted between host and network
     * byte order at this boundary.
     */
    class SocketAddressConverter
    {
    public:

        /**
         * \brief Builds a native socket address from a numeric IP address and port.
         *
         * \pre An IPv4 address exposes exactly four bytes and an IPv6 address
         *      exposes exactly sixteen bytes.
         *
         * \param ipAddress Numeric IPv4 or IPv6 address to convert.
         * \param port Port in host byte order.
         *
         * \return A converter containing the corresponding native address.
         */
        static SocketAddressConverter Of(const IpAddress& ipAddress, unsigned short port);

        /**
         * \brief Copies an IPv4 native socket address.
         *
         * \param sockaddr Valid IPv4 native address to copy.
         */
        explicit SocketAddressConverter(const sockaddr_in& sockaddr);

        /**
         * \brief Copies an IPv6 native socket address.
         *
         * \param sockaddr Valid IPv6 native address to copy.
         */
        explicit SocketAddressConverter(const sockaddr_in6& sockaddr);

        /**
         * \brief Copies a generic native IPv4 or IPv6 socket address.
         *
         * \pre \p sockAddr has family AF_INET or AF_INET6.
         * \pre \p sockAddrLength exactly matches the corresponding native address
         *      structure size.
         *
         * \param sockAddr Native address to copy.
         * \param sockAddrLength Byte length reported by the native socket API.
         */
        SocketAddressConverter(const sockaddr& sockAddr, int sockAddrLength);

        /**
         * \brief Gets a borrowed pointer to the stored native socket address.
         *
         * \return Pointer valid while this converter remains alive and unmodified.
         */
        const sockaddr* AsSockAddr() const;

        /**
         * \brief Gets the IP protocol represented by the native address.
         *
         * \return Ipv4 for AF_INET or Ipv6 for AF_INET6.
         */
        IpAddress::Protocol GetProtocol() const;

        /**
         * \brief Allocates an engine IP address copied from the native address.
         *
         * \return An IpAddressV4 or IpAddressV6 selected by GetProtocol.
         */
        UniquePtr<IpAddress> ToIpAddress() const;

        /**
         * \brief Copies the stored IPv4 address into engine representation.
         *
         * \pre GetProtocol returns IpAddress::Protocol::Ipv4.
         *
         * \return The IPv4 address in host representation.
         */
        IpAddressV4 ToIpAddressV4() const;

        /**
         * \brief Copies the stored IPv6 address into engine representation.
         *
         * \pre GetProtocol returns IpAddress::Protocol::Ipv6.
         *
         * \return The IPv6 address in host representation.
         */
        IpAddressV6 ToIpAddressV6() const;

        /**
         * \brief Gets the stored port in host byte order.
         *
         * \return The port converted from the native network-byte-order value.
         */
        unsigned short GetPort() const;

        /**
         * \brief Gets the byte length of the stored native address.
         *
         * \return sizeof(sockaddr_in) or sizeof(sockaddr_in6).
         */
        int GetLength() const;

    private:

        union
        {
            sockaddr_in V4;
            sockaddr_in6 V6;
        } mAddress;

        int mLength;
    };
}
#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKETADDRESSCONVERTER_HPP
