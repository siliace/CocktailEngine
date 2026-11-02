#include <CocktailEngine/Core/System/Network/Address/IpAddress.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketAddress.hpp>

namespace Ck
{
    SocketAddress::SocketAddress() :
        mPort(0)
    {
        /// Nothing
    }

    SocketAddress::SocketAddress(const IpAddress& ipAddress, unsigned short port) :
        mPort(port)
    {
        switch (ipAddress.GetProtocol())
        {
            case IpAddress::Protocol::Ipv4: mAddress.Emplace<IpAddressV4>(static_cast<const IpAddressV4&>(ipAddress)); break;
            case IpAddress::Protocol::Ipv6: mAddress.Emplace<IpAddressV6>(static_cast<const IpAddressV6&>(ipAddress)); break;
        }

        assert(mAddress.HasValue());
    }

    unsigned short SocketAddress::GetPort() const
    {
        return mPort;
    }

    IpAddress::Protocol SocketAddress::GetProtocol() const
    {
        assert(mAddress.HasValue());
        return mAddress.GetIndex() == Variant<IpAddressV4, IpAddressV6>::IndexFor<IpAddressV4>() ? IpAddress::Protocol::Ipv4 : IpAddress::Protocol::Ipv6;
    }

    const IpAddressV4& SocketAddress::GetAddressV4() const
    {
        return mAddress.Get<IpAddressV4>();
    }

    const IpAddressV6& SocketAddress::GetAddressV6() const
    {
        return mAddress.Get<IpAddressV6>();
    }
}
