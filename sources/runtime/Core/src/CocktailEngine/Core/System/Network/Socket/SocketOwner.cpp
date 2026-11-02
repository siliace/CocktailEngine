#include <CocktailEngine/Core/System/Network/Socket/SocketOwner.hpp>

namespace Ck
{
    SocketOwner::~SocketOwner()
    {
        SocketOwner::Close();
    }

    void SocketOwner::Close()
    {
        mSocket.Close();
        mLocalAddress = {};
    }

    Optional<const IpAddress&> SocketOwner::GetLocalAddress() const
    {
        return GetLocalSocketAddress().Map([](const SocketAddress& localAddress) -> const IpAddress& {
            switch (localAddress.GetProtocol())
            {
                case IpAddress::Protocol::Ipv4: return localAddress.GetAddressV4();
                case IpAddress::Protocol::Ipv6: return localAddress.GetAddressV6();
            }

            COCKTAIL_UNREACHABLE();
        });
    }

    Optional<unsigned short> SocketOwner::GetLocalPort() const
    {
        return GetLocalSocketAddress().Map([](const SocketAddress& localAddress) {
            return localAddress.GetPort();
        });
    }

    SocketOwner::SocketOwner(Socket socket) :
        mSocket(Move(socket))
    {
        /// Nothing
    }

    void SocketOwner::Create(IpAddress::Protocol protocol, Socket::Type type)
    {
        assert(!mSocket.IsValid());

        Result<void, SocketErrorCode> createResult = mSocket.Create(protocol, type);
        if (createResult.IsFailure())
            throw SocketException(createResult.GetError());
    }

    Optional<const SocketAddress&> SocketOwner::GetLocalSocketAddress() const
    {
        if (!mSocket.IsValid())
            return Optional<const SocketAddress&>::Empty();

        Result<void, SocketErrorCode> result = mSocket.GetLocalAddress(mLocalAddress);
        if (result.IsFailure())
            throw SocketException(result.GetError());

        return Optional<const SocketAddress&>::Of(mLocalAddress);
    }
}
