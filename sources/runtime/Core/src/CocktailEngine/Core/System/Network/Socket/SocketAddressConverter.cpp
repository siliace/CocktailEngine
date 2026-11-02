#include <CocktailEngine/Core/System/Network/Socket/SocketAddressConverter.hpp>

namespace Ck
{
    SocketAddressConverter SocketAddressConverter::Of(const IpAddress& ipAddress, unsigned short port)
    {
        if (ipAddress.GetProtocol() == IpAddress::Protocol::Ipv4)
        {
            sockaddr_in sockAddr;
            Memory::Zero(sockAddr);

            Uint8* bytes = reinterpret_cast<Uint8*>(&sockAddr.sin_addr);

            sockAddr.sin_family = AF_INET;
            sockAddr.sin_port = htons(port);
            for (std::size_t i = 0; i < ipAddress.GetLength(); i++)
                bytes[i] = ipAddress.GetByte(i);

            return SocketAddressConverter(sockAddr);
        }

        sockaddr_in6 sockAddr;
        Memory::Zero(sockAddr);
        sockAddr.sin6_family = AF_INET6;
        sockAddr.sin6_port = htons(port);

        Uint8* bytes = reinterpret_cast<Uint8*>(&sockAddr.sin6_addr);

        for (std::size_t i = 0; i < ipAddress.GetLength(); i++)
            bytes[i] = ipAddress.GetByte(i);

        return SocketAddressConverter(sockAddr);
    }

    SocketAddressConverter::SocketAddressConverter(const sockaddr_in& sockaddr) :
        mLength(sizeof(sockaddr_in))
    {
        mAddress.V4 = sockaddr;
    }

    SocketAddressConverter::SocketAddressConverter(const sockaddr_in6& sockaddr) :
        mLength(sizeof(sockaddr_in6))
    {
        mAddress.V6 = sockaddr;
    }

    SocketAddressConverter::SocketAddressConverter(const sockaddr& sockAddr, int sockAddrLength)
    {
        if (sockAddr.sa_family == AF_INET)
        {
            assert(sockAddrLength == sizeof(sockaddr_in));

            mLength = sockAddrLength;
            mAddress.V4 = reinterpret_cast<const sockaddr_in&>(sockAddr);
        }
        else if (sockAddr.sa_family == AF_INET6)
        {
            assert(sockAddrLength == sizeof(sockaddr_in6));

            mLength = sockAddrLength;
            mAddress.V6 = reinterpret_cast<const sockaddr_in6&>(sockAddr);
        }
    }

    const sockaddr* SocketAddressConverter::AsSockAddr() const
    {
        return reinterpret_cast<const sockaddr*>(&mAddress);
    }

    IpAddress::Protocol SocketAddressConverter::GetProtocol() const
    {
        switch (AsSockAddr()->sa_family)
        {
            case AF_INET: return IpAddress::Protocol::Ipv4;
            case AF_INET6: return IpAddress::Protocol::Ipv6;
        }

        COCKTAIL_UNREACHABLE();
    }

    UniquePtr<IpAddress> SocketAddressConverter::ToIpAddress() const
    {
        switch (AsSockAddr()->sa_family)
        {
            case AF_INET: return MakeUnique<IpAddressV4>(ToIpAddressV4());
            case AF_INET6: return MakeUnique<IpAddressV6>(ToIpAddressV6());
        }

        return nullptr;
    }

    IpAddressV4 SocketAddressConverter::ToIpAddressV4() const
    {
        const sockaddr_in* addr = reinterpret_cast<const sockaddr_in*>(this);
        const Uint8* bytes = reinterpret_cast<const Uint8*>(&addr->sin_addr);

        return IpAddressV4(bytes[0], bytes[1], bytes[2], bytes[3]);
    }

    IpAddressV6 SocketAddressConverter::ToIpAddressV6() const
    {
        const sockaddr_in6* addr = reinterpret_cast<const sockaddr_in6*>(this);
        const Uint8* bytes = reinterpret_cast<const Uint8*>(&addr->sin6_addr);

        IpAddressV6 ipAddress;
        for (std::size_t i = 0; i < 16; i++)
            ipAddress.SetByte(i, bytes[i]);

        return ipAddress;
    }

    unsigned short SocketAddressConverter::GetPort() const
    {
        switch (AsSockAddr()->sa_family)
        {
            case AF_INET: return ntohs(reinterpret_cast<const sockaddr_in*>(this)->sin_port);
            case AF_INET6: return ntohs(reinterpret_cast<const sockaddr_in6*>(this)->sin6_port);
        }

        return 0;
    }

    int SocketAddressConverter::GetLength() const
    {
        return mLength;
    }
}
