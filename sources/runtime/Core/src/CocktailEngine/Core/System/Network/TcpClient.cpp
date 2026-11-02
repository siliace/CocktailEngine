#include <CocktailEngine/Core/System/Network/Socket/SocketAddress.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketException.hpp>
#include <CocktailEngine/Core/System/Network/TcpClient.hpp>

namespace Ck
{
    TcpClient::TcpClient(const IpAddress& address, unsigned short port)
    {
        Connect(address, port);
    }

    TcpClient::TcpClient(const IpAddress& address, unsigned short port, const IpAddress& localAddress, unsigned short localPort)
    {
        Bind(localAddress, localPort);
        Connect(address, port);
    }

    void TcpClient::Create(IpAddress::Protocol protocol)
    {
       SocketOwner::Create(protocol, Socket::Type::Tcp);
    }

    void TcpClient::Bind(const IpAddress& localAddress, unsigned short localPort)
    {
        assert(localPort != 0);

        if (!mSocket.IsValid())
            Create(localAddress.GetProtocol());

        Result<void, SocketErrorCode> bindResult = mSocket.Bind(localAddress, localPort);
        if (bindResult.IsFailure())
            throw SocketException(bindResult.GetError());
    }

    void TcpClient::Connect(const IpAddress& address, unsigned short port)
    {
        assert(port != 0);

        if (!mSocket.IsValid())
            Create(address.GetProtocol());

        Result<void, SocketErrorCode> connectResult = mSocket.Connect(address, port);
        if (connectResult.IsFailure())
            throw SocketException(connectResult.GetError());

        mRemoteAddress = SocketAddress(address, port);
    }

    void TcpClient::Send(const void* data, unsigned int length) const
    {
        assert(mSocket.IsValid());

        if (data && length)
        {
            for (unsigned int sent = 0; sent < length;)
            {
                Result<unsigned int, SocketErrorCode> result = mSocket.Send(static_cast<const Byte*>(data) + sent, length - sent);
                if (result.IsFailure())
                    throw SocketException(result.GetError());

                sent += result.GetValue();
            }
        }
    }

    unsigned int TcpClient::Receive(void* data, unsigned int length) const
    {
        assert(mSocket.IsValid());

        if (data && length)
        {
            Result<unsigned int, SocketErrorCode> result = mSocket.Receive(data, length);
            if (result.IsFailure())
                throw SocketException(result.GetError());

            return result.GetValue();
        }

        return 0;
    }

    void TcpClient::Close()
    {
        SocketOwner::Close();
        mRemoteAddress = {};
    }

    Optional<const IpAddress&> TcpClient::GetRemoteAddress() const
    {
        if (!mSocket.IsValid())
            return Optional<const IpAddress&>::Empty();

        switch (mRemoteAddress.GetProtocol())
        {
            case IpAddress::Protocol::Ipv4: return Optional<const IpAddress&>::Of(mRemoteAddress.GetAddressV4());
            case IpAddress::Protocol::Ipv6: return Optional<const IpAddress&>::Of(mRemoteAddress.GetAddressV6());
        }

        return Optional<const IpAddress&>::Empty();
    }

    Optional<unsigned short> TcpClient::GetRemotePort() const
    {
        if (!mSocket.IsValid())
            return Optional<unsigned short>::Empty();

        return Optional<unsigned short>::Of(mRemoteAddress.GetPort());
    }

    bool TcpClient::GetNoDelay() const
    {
        return GetSocketOption(SocketOptions::TcpNoDelay);
    }

    void TcpClient::SetNoDelay(bool noDelay) const
    {
        SetSocketOption(SocketOptions::TcpNoDelay, noDelay);
    }

    bool TcpClient::GetKeepAlive() const
    {
        return GetSocketOption(SocketOptions::KeepAlive);
    }

    void TcpClient::SetKeepAlive(bool keepAlive) const
    {
        SetSocketOption(SocketOptions::KeepAlive, keepAlive);
    }

    bool TcpClient::GetOutOfBandInline() const
    {
        return GetSocketOption(SocketOptions::OutOfBandInline);
    }

    void TcpClient::SetOutOfBandInline(bool outOfBandInline) const
    {
        SetSocketOption(SocketOptions::OutOfBandInline, outOfBandInline);
    }

    bool TcpClient::GetReuseAddress() const
    {
        return GetSocketOption(SocketOptions::ReuseAddress);
    }

    void TcpClient::SetReuseAddress(bool reuseAddress) const
    {
        SetSocketOption(SocketOptions::ReuseAddress, reuseAddress);
    }

    unsigned int TcpClient::GetReceiveBufferSize() const
    {
        return GetSocketOption(SocketOptions::ReceiveBuffer);
    }

    void TcpClient::SetReceiveBufferSize(unsigned int bufferSize) const
    {
        SetSocketOption(SocketOptions::ReceiveBuffer, bufferSize);
    }

    unsigned int TcpClient::GetSendBufferSize() const
    {
        return GetSocketOption(SocketOptions::SendBuffer);
    }

    void TcpClient::SetSendBufferSize(unsigned int bufferSize) const
    {
        SetSocketOption(SocketOptions::SendBuffer, bufferSize);
    }

    SocketLinger TcpClient::GetLinger() const
    {
        return GetSocketOption(SocketOptions::Linger);
    }

    void TcpClient::SetLinger(const SocketLinger& linger) const
    {
        SetSocketOption(SocketOptions::Linger, linger);
    }

    Duration TcpClient::GetReceiveTimeout() const
    {
        return GetSocketOption(SocketOptions::ReceiveTimeout);
    }

    void TcpClient::SetReceiveTimeout(const Duration& timeout) const
    {
        SetSocketOption(SocketOptions::ReceiveTimeout, timeout);
    }

    Duration TcpClient::GetSendTimeout() const
    {
        return GetSocketOption(SocketOptions::SendTimeout);
    }

    void TcpClient::SetSendTimeout(const Duration& timeout) const
    {
        SetSocketOption(SocketOptions::SendTimeout, timeout);
    }

    unsigned int TcpClient::GetPendingLength() const
    {
        return mSocket.GetPendingLength();
    }

    TcpClient::TcpClient(Socket socket, SocketAddress remoteAddress) :
        SocketOwner(Move(socket)),
        mRemoteAddress(Move(remoteAddress))
    {
        assert(mSocket.IsValid());
    }
}
