#include <CocktailEngine/Core/System/Network/SocketAddress.hpp>
#include <CocktailEngine/Core/System/Network/TcpServer.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>

#include "CocktailEngine/Core/System/Concurrency/Thread.hpp"
#include "CocktailEngine/Core/Utility/Finally.hpp"

namespace Ck
{
    TcpServer::TcpServer(unsigned short port, const IpAddress& host, int backlog) :
        Socket(host.GetProtocol(), Type::Tcp)
    {
        assert(port != 0);
        assert(backlog != 0);
        assert(mHandle != InvalidHandle);

        SocketAddress socketAddress = SocketAddress::Of(host, port);
        int error = ::bind(mHandle, socketAddress.AsSockAddr(), socketAddress.GetLength());
        if (error == -1)
            throw SystemError::GetLastError();

        ::listen(mHandle, backlog);
        if (error == -1)
            throw SystemError::GetLastError();
    }

    TcpClient TcpServer::Accept() const
    {
        assert(mHandle != InvalidHandle);

        sockaddr_in6 sockAddr;
        int sockaddrlength = sizeof(sockaddr_in6);
        Handle clientHandle = ::accept(mHandle, reinterpret_cast<sockaddr*>(&sockAddr), (socklen_t*) &sockaddrlength);
        if (clientHandle == InvalidHandle)
            throw SystemError::GetLastError();

        SocketAddress socketAddress(reinterpret_cast<const sockaddr&>(sockAddr), sockaddrlength);
        return TcpClient(clientHandle, socketAddress.ToIpAddress(), socketAddress.GetPort());
    }

    Optional<TcpClient> TcpServer::TryAccept(const Duration& timeout, const Duration& interval) const
    {
        assert(mHandle != InvalidHandle);

        Instant start = Instant::Now();

        Handle clientHandle = InvalidHandle;
        while (Duration::Between(start, Instant::Now()) < timeout)
        {
            sockaddr_in6 sockAddr;
            int sockaddrlength = sizeof(sockaddr_in6);
            clientHandle = ::accept(mHandle, reinterpret_cast<sockaddr*>(&sockAddr), (socklen_t*) &sockaddrlength);
            if (clientHandle == InvalidHandle)
            {
                unsigned int error = SystemError::GetLastErrorCode();
                if (error == EAGAIN || error == EWOULDBLOCK || error == EINTR)
                {
                    if (error != EINTR)
                        Thread::SleepFor(interval);

                    continue;
                }

                throw std::system_error((int) error, SystemError::GetSystemErrorCategory());
            }

            SocketAddress socketAddress(reinterpret_cast<const sockaddr&>(sockAddr), sockaddrlength);
            return Optional<TcpClient>::Of(TcpClient(clientHandle, socketAddress.ToIpAddress(), socketAddress.GetPort()));
        }

        return Optional<TcpClient>::Empty();
    }
}
