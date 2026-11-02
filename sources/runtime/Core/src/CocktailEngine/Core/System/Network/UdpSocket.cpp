#include <CocktailEngine/Core/System/Network/Socket/SocketAddress.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketException.hpp>
#include <CocktailEngine/Core/System/Network/UdpSocket.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>

namespace Ck
{
    UdpSocket::UdpSocket(IpAddress::Protocol protocol)
    {
        Create(protocol, Socket::Type::Udp);
    }

    UdpSocket::UdpSocket(const IpAddress& localAddress, unsigned short localPort)
    {
        Bind(localAddress, localPort);
    }

    void UdpSocket::Bind(const IpAddress& localAddress, unsigned short localPort)
    {
        if (!mSocket.IsValid())
            Create(localAddress.GetProtocol(), Socket::Type::Udp);

        Result<void, SocketErrorCode> bindResult = mSocket.Bind(localAddress, localPort);
        if (bindResult.IsFailure())
            throw SocketException(bindResult.GetError());
    }

    unsigned int UdpSocket::Receive(void* data, std::size_t length, SocketAddress& clientAddress) const
    {
        assert(mSocket.IsValid());

        if (data && length)
        {
            Result<unsigned int, SocketErrorCode> result = mSocket.ReceiveFrom(data, length, clientAddress);
            if (result.IsFailure())
                throw SocketException(result.GetError());

            return result.GetValue();
        }

        return 0;
    }

    void UdpSocket::Send(const void* data, std::size_t length, const IpAddress& address, unsigned short port) const
    {
        assert(mSocket.IsValid());

        if (!data || length == 0)
            return;

        Result<unsigned int, SocketErrorCode> result = mSocket.SendTo(data, static_cast<unsigned int>(length), address, port);
        if (result.IsFailure())
            throw SocketException(result.GetError());

        assert(result.GetValue() == length);
    }
}
