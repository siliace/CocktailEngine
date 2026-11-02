#include <CocktailEngine/Core/System/Concurrency/Thread.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketAddress.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketException.hpp>
#include <CocktailEngine/Core/System/Network/TcpListener.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>
#include <CocktailEngine/Core/Utility/Finally.hpp>

namespace Ck
{
    TcpListener::TcpListener(unsigned short localPort, const IpAddress& localAddress)
    {
        Bind(localAddress, localPort);
    }

    TcpListener::TcpListener(unsigned short localPort, const IpAddress& localAddress, int backlog)
    {
        Bind(localAddress, localPort);
        Listen(backlog);
    }

    void TcpListener::Create(IpAddress::Protocol protocol)
    {
        SocketOwner::Create(protocol, Socket::Type::Tcp);
    }

    void TcpListener::Bind(const IpAddress& localAddress, unsigned short localPort)
    {
        if (!mSocket.IsValid())
            Create(localAddress.GetProtocol());

        Result<void, SocketErrorCode> bindResult = mSocket.Bind(localAddress, localPort);
        if (bindResult.IsFailure())
            throw SocketException(bindResult.GetError());
    }

    void TcpListener::Listen(int backlog) const
    {
        assert(backlog != 0);
        assert(mSocket.IsValid());

        Result<void, SocketErrorCode> listenResult = mSocket.Listen(backlog);
        if (listenResult.IsFailure())
            throw SocketException(listenResult.GetError());
    }

    TcpClient TcpListener::Accept()
    {
        assert(mSocket.IsValid());

        bool wasBlocking = mSocket.IsBlocking();
        mSocket.SetBlocking(true);
        Finally finally([&] {
            mSocket.SetBlocking(wasBlocking);
        });

        SocketAddress clientAddress;
        Result<Socket, SocketErrorCode> result = mSocket.Accept(clientAddress);
        if (result.IsFailure())
            throw SocketException(result.GetError());

        return TcpClient(Move(result.GetValue()), Move(clientAddress));
    }

    Optional<TcpClient> TcpListener::TryAccept(const Duration& timeout, const Duration& interval)
    {
        assert(mSocket.IsValid());

        bool wasBlocking = mSocket.IsBlocking();
        mSocket.SetBlocking(false);
        Finally finally([&] {
            mSocket.SetBlocking(wasBlocking);
        });

        Instant start = Instant::Now();

        SocketAddress clientAddress;
        while (Duration::Between(start, Instant::Now()) < timeout)
        {
            Result<Socket, SocketErrorCode> result = mSocket.Accept(clientAddress);
            if (result.IsSuccess())
            {
                TcpClient client(Move(result.GetValue()), Move(clientAddress));
                return Optional<TcpClient>::Of(Move(client));
            }

            SocketErrorCode error = result.GetError();
            if (error == SocketErrorCode::WouldBlock || error == SocketErrorCode::Interrupted)
            {
                if (error != SocketErrorCode::Interrupted)
                    Thread::SleepFor(interval);

                continue;
            }

            throw SocketException(error);
        }

        return Optional<TcpClient>::Empty();
    }

    bool TcpListener::GetReuseAddress() const
    {
        return GetSocketOption(SocketOptions::ReuseAddress);
    }

    void TcpListener::SetReuseAddress(bool reuseAddress) const
    {
        SetSocketOption(SocketOptions::ReuseAddress, reuseAddress);
    }

    unsigned int TcpListener::GetReceiveBufferSize() const
    {
        return GetSocketOption(SocketOptions::ReceiveBuffer);
    }

    void TcpListener::SetReceiveBufferSize(unsigned int bufferSize) const
    {
        SetSocketOption(SocketOptions::ReceiveBuffer, bufferSize);
    }

    Duration TcpListener::GetReceiveTimeout() const
    {
        return GetSocketOption(SocketOptions::ReceiveTimeout);
    }

    void TcpListener::SetReceiveTimeout(const Duration& timeout) const
    {
        SetSocketOption(SocketOptions::ReceiveTimeout, timeout);
    }

    bool TcpListener::GetIPv6Only() const
    {
        return GetSocketOption(SocketOptions::IPv6Only);
    }

    void TcpListener::SetIpV6Only(bool ipv6Only) const
    {
        SetSocketOption(SocketOptions::IPv6Only, ipv6Only);
    }
}
