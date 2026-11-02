#include <utility>

#include <cstddef>

#include <CocktailEngine/Core/System/Network/Address/IpAddressV4.hpp>
#include <CocktailEngine/Core/System/Network/Address/IpAddressV6.hpp>
#include <CocktailEngine/Core/System/Network/Socket/Socket.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketAddressConverter.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>

namespace Ck
{
    // The option traits in Socket.hpp describe native layouts without being able
    // to see the system headers. These checks compare each of them against the
    // real structure, so a platform whose definition differs breaks the build
    // here instead of silently truncating an option at runtime.
    namespace
    {
        using NativeLinger = Detail::SocketOptionTraits<SocketLinger>::NativeType;
        using NativeTimeout = Detail::SocketOptionTraits<Duration>::NativeType;
        using NativeIpAddressV4 = Detail::SocketOptionTraits<IpAddressV4>::NativeType;

        static_assert(sizeof(NativeIpAddressV4) == sizeof(::in_addr));

#ifdef COCKTAIL_OS_WINDOWS
        static_assert(sizeof(NativeLinger) == sizeof(::linger));
        static_assert(sizeof(NativeTimeout) == sizeof(DWORD));
#else
        static_assert(sizeof(NativeLinger) == sizeof(struct ::linger));
        static_assert(sizeof(NativeTimeout) == sizeof(struct ::timeval));

        static_assert(offsetof(NativeLinger, OnOff) == offsetof(struct ::linger, l_onoff));
        static_assert(offsetof(NativeLinger, Linger) == offsetof(struct ::linger, l_linger));
        static_assert(sizeof(NativeLinger::OnOff) == sizeof(decltype(::linger::l_onoff)));
        static_assert(sizeof(NativeLinger::Linger) == sizeof(decltype(::linger::l_linger)));

        static_assert(offsetof(NativeTimeout, Seconds) == offsetof(struct ::timeval, tv_sec));
        static_assert(offsetof(NativeTimeout, MicroSeconds) == offsetof(struct ::timeval, tv_usec));
        static_assert(sizeof(NativeTimeout::Seconds) == sizeof(decltype(::timeval::tv_sec)));
        static_assert(sizeof(NativeTimeout::MicroSeconds) == sizeof(decltype(::timeval::tv_usec)));
#endif
    }

    namespace
    {
#ifdef COCKTAIL_OS_WINDOWS
        using SocketAddressLength = int;
#else
        using SocketAddressLength = socklen_t;
#endif

        int ToAddressFamily(IpAddress::Protocol protocol)
        {
            switch (protocol)
            {
                case IpAddress::Protocol::Ipv4: return AF_INET;
                case IpAddress::Protocol::Ipv6: return AF_INET6;
            }

            COCKTAIL_UNREACHABLE();
        }

        int ToNativeType(Socket::Type type)
        {
            switch (type)
            {
                case Socket::Type::Tcp: return SOCK_STREAM;
                case Socket::Type::Udp: return SOCK_DGRAM;
                case Socket::Type::Raw: return SOCK_RAW;
            }

            COCKTAIL_UNREACHABLE();
        }

        int ToNativeOptionLevel(SocketOptionLevel level)
        {
            switch (level)
            {
                case SocketOptionLevel::Socket: return SOL_SOCKET;
                case SocketOptionLevel::IPv4: return IPPROTO_IP;
                case SocketOptionLevel::IPv6: return IPPROTO_IPV6;
                case SocketOptionLevel::Tcp: return IPPROTO_TCP;
            }

            COCKTAIL_UNREACHABLE();
        }

        int ToNativeOption(SocketOptionId optionId)
        {
            switch (optionId)
            {
                case SocketOptionId::AcceptConnection: return SO_ACCEPTCONN;
                case SocketOptionId::ReuseAddress: return SO_REUSEADDR;
                case SocketOptionId::KeepAlive: return SO_KEEPALIVE;
                case SocketOptionId::Broadcast: return SO_BROADCAST;
                case SocketOptionId::SendBuffer: return SO_SNDBUF;
                case SocketOptionId::ReceiveBuffer: return SO_RCVBUF;
                case SocketOptionId::SendTimeout: return SO_SNDTIMEO;
                case SocketOptionId::ReceiveTimeout: return SO_RCVTIMEO;
                case SocketOptionId::Linger: return SO_LINGER;
                case SocketOptionId::OutOfBandInline: return SO_OOBINLINE;
                case SocketOptionId::Error: return SO_ERROR;
                case SocketOptionId::Type: return SO_TYPE;

                case SocketOptionId::IpHeaderIncluded: return IP_HDRINCL;
                case SocketOptionId::IpOptions: return IP_OPTIONS;
                case SocketOptionId::IpTimeToLive: return IP_TTL;
                case SocketOptionId::IpMulticastInterface: return IP_MULTICAST_IF;
                case SocketOptionId::IpMulticastTimeToLive: return IP_MULTICAST_TTL;
                case SocketOptionId::IpMulticastLoop: return IP_MULTICAST_LOOP;
                case SocketOptionId::IpAddMembership: return IP_ADD_MEMBERSHIP;
                case SocketOptionId::IpDropMembership: return IP_DROP_MEMBERSHIP;

                case SocketOptionId::IPv6Only: return IPV6_V6ONLY;
                case SocketOptionId::IPv6UnicastHopLimit: return IPV6_UNICAST_HOPS;
                case SocketOptionId::IPv6MulticastInterface: return IPV6_MULTICAST_IF;
                case SocketOptionId::IPv6MulticastHopLimit: return IPV6_MULTICAST_HOPS;
                case SocketOptionId::IPv6MulticastLoop: return IPV6_MULTICAST_LOOP;
                case SocketOptionId::IPv6AddMembership: return IPV6_JOIN_GROUP;
                case SocketOptionId::IPv6DropMembership: return IPV6_LEAVE_GROUP;

                case SocketOptionId::TcpNoDelay: return TCP_NODELAY;
            }

            COCKTAIL_UNREACHABLE();
        }

#ifdef COCKTAIL_OS_WINDOWS
        SocketErrorCode GetLastSocketError()
        {
            switch (WSAGetLastError())
            {
                case WSAEINTR: return SocketErrorCode::Interrupted;
                case WSAEBADF: return SocketErrorCode::BadFileDescriptor;
                case WSAEACCES: return SocketErrorCode::AccessDenied;
                case WSAEFAULT: return SocketErrorCode::Fault;
                case WSAEINVAL: return SocketErrorCode::InvalidArgument;
                case WSAEMFILE: return SocketErrorCode::TooManyOpenFiles;

                case WSAEWOULDBLOCK: return SocketErrorCode::WouldBlock;
                case WSAEINPROGRESS: return SocketErrorCode::InProgress;
                case WSAEALREADY: return SocketErrorCode::AlreadyInProgress;

                case WSAENOTSOCK: return SocketErrorCode::NotSocket;
                case WSAEDESTADDRREQ: return SocketErrorCode::DestinationAddressRequired;
                case WSAEMSGSIZE: return SocketErrorCode::MessageTooLarge;

                case WSAEPROTOTYPE: return SocketErrorCode::ProtocolWrongType;
                case WSAENOPROTOOPT: return SocketErrorCode::ProtocolOptionNotSupported;
                case WSAEPROTONOSUPPORT: return SocketErrorCode::ProtocolNotSupported;
                case WSAESOCKTNOSUPPORT: return SocketErrorCode::SocketTypeNotSupported;
                case WSAEOPNOTSUPP: return SocketErrorCode::OperationNotSupported;
                case WSAEAFNOSUPPORT: return SocketErrorCode::AddressFamilyNotSupported;

                case WSAEADDRINUSE: return SocketErrorCode::AddressInUse;
                case WSAEADDRNOTAVAIL: return SocketErrorCode::AddressNotAvailable;

                case WSAENETDOWN: return SocketErrorCode::NetworkDown;
                case WSAENETUNREACH: return SocketErrorCode::NetworkUnreachable;
                case WSAENETRESET: return SocketErrorCode::NetworkReset;
                case WSAECONNABORTED: return SocketErrorCode::ConnectionAborted;
                case WSAECONNRESET: return SocketErrorCode::ConnectionReset;
                case WSAENOBUFS: return SocketErrorCode::NoBufferSpace;
                case WSAEISCONN: return SocketErrorCode::IsConnected;
                case WSAENOTCONN: return SocketErrorCode::NotConnected;
                case WSAESHUTDOWN: return SocketErrorCode::Shutdown;
                case WSAETIMEDOUT: return SocketErrorCode::TimedOut;
                case WSAECONNREFUSED: return SocketErrorCode::ConnectionRefused;

                case WSAEHOSTDOWN: return SocketErrorCode::HostDown;
                case WSAEHOSTUNREACH: return SocketErrorCode::HostUnreachable;
            }

            COCKTAIL_UNREACHABLE();
        }
#else
        SocketErrorCode GetLastSocketError()
        {
            switch (errno)
            {
                case EINTR: return SocketErrorCode::Interrupted;
                case EBADF: return SocketErrorCode::BadFileDescriptor;
                case EACCES: return SocketErrorCode::AccessDenied;
                case EFAULT: return SocketErrorCode::Fault;
                case EINVAL: return SocketErrorCode::InvalidArgument;
                case EMFILE: return SocketErrorCode::TooManyOpenFiles;

                case EWOULDBLOCK: return SocketErrorCode::WouldBlock;
                case EINPROGRESS: return SocketErrorCode::InProgress;
                case EALREADY: return SocketErrorCode::AlreadyInProgress;

                case ENOTSOCK: return SocketErrorCode::NotSocket;
                case EDESTADDRREQ: return SocketErrorCode::DestinationAddressRequired;
                case EMSGSIZE: return SocketErrorCode::MessageTooLarge;

                case EPROTOTYPE: return SocketErrorCode::ProtocolWrongType;
                case ENOPROTOOPT: return SocketErrorCode::ProtocolOptionNotSupported;
                case EPROTONOSUPPORT: return SocketErrorCode::ProtocolNotSupported;
                case ESOCKTNOSUPPORT: return SocketErrorCode::SocketTypeNotSupported;
                case EOPNOTSUPP: return SocketErrorCode::OperationNotSupported;
                case EAFNOSUPPORT: return SocketErrorCode::AddressFamilyNotSupported;

                case EADDRINUSE: return SocketErrorCode::AddressInUse;
                case EADDRNOTAVAIL: return SocketErrorCode::AddressNotAvailable;

                case ENETDOWN: return SocketErrorCode::NetworkDown;
                case ENETUNREACH: return SocketErrorCode::NetworkUnreachable;
                case ENETRESET: return SocketErrorCode::NetworkReset;
                case ECONNABORTED: return SocketErrorCode::ConnectionAborted;
                case ECONNRESET: return SocketErrorCode::ConnectionReset;
                case ENOBUFS: return SocketErrorCode::NoBufferSpace;
                case EISCONN: return SocketErrorCode::IsConnected;
                case ENOTCONN: return SocketErrorCode::NotConnected;
                case ESHUTDOWN: return SocketErrorCode::Shutdown;
                case ETIMEDOUT: return SocketErrorCode::TimedOut;
                case ECONNREFUSED: return SocketErrorCode::ConnectionRefused;

                case EHOSTDOWN: return SocketErrorCode::HostDown;
                case EHOSTUNREACH: return SocketErrorCode::HostUnreachable;
            }

            COCKTAIL_UNREACHABLE();
        }
#endif
    }

    Socket::Socket() :
        mHandle(InvalidHandle),
        mBlocking(false)
    {
        /// Nothing
    }

    Socket::Socket(Socket&& other) noexcept :
        mHandle(InvalidHandle),
        mBlocking(false)
    {
        *this = Move(other);
    }

    Socket::~Socket()
    {
        Close();
    }

    Socket& Socket::operator=(Socket&& other) noexcept
    {
        if (this != &other)
        {
            Close();

            mHandle = std::exchange(other.mHandle, InvalidHandle);
            mBlocking = std::exchange(other.mBlocking, false);
        }

        return *this;
    }

    Result<void, SocketErrorCode> Socket::Create(IpAddress::Protocol protocol, Type type)
    {
        assert(mHandle == InvalidHandle);

        mHandle = ::socket(ToAddressFamily(protocol), ToNativeType(type), 0);
        if (mHandle == InvalidHandle)
            return Result<void, SocketErrorCode>::Failure(GetLastSocketError());

        return Result<void, SocketErrorCode>::Success();
    }

    void Socket::Close()
    {
        if (mHandle != InvalidHandle)
        {
#ifdef COCKTAIL_OS_WINDOWS
            closesocket(mHandle);
#else
            close(mHandle);
#endif
            mHandle = InvalidHandle;
            mBlocking = false;
        }
    }

    Result<Socket, SocketErrorCode> Socket::Accept(SocketAddress& socketAddress) const
    {
        assert(mHandle != InvalidHandle);

        sockaddr_in6 sockAddr;
        SocketAddressLength socketAddressLength = sizeof(sockAddr);
        Handle clientHandle = ::accept(mHandle, reinterpret_cast<sockaddr*>(&sockAddr), &socketAddressLength);
        if (clientHandle == InvalidHandle)
            return Result<Socket, SocketErrorCode>::Failure(GetLastSocketError());

        SocketAddressConverter socketAddressConverter(reinterpret_cast<const sockaddr&>(sockAddr), static_cast<int>(socketAddressLength));
        switch (socketAddressConverter.GetProtocol())
        {
            case IpAddress::Protocol::Ipv4: socketAddress = SocketAddress(socketAddressConverter.ToIpAddressV4(), socketAddressConverter.GetPort()); break;
            case IpAddress::Protocol::Ipv6: socketAddress = SocketAddress(socketAddressConverter.ToIpAddressV6(), socketAddressConverter.GetPort()); break;
        }

        return Result<Socket, SocketErrorCode>::Success(Socket(clientHandle, true));
    }

    Result<void, SocketErrorCode> Socket::Bind(const IpAddress& localAddress, unsigned short localPort) const
    {
        assert(mHandle != InvalidHandle);

        SocketAddressConverter socketAddressConverter = SocketAddressConverter::Of(localAddress, localPort);
        int error = ::bind(mHandle, socketAddressConverter.AsSockAddr(), socketAddressConverter.GetLength());
        if (error == SOCKET_ERROR)
            return Result<void, SocketErrorCode>::Failure(GetLastSocketError());

        return Result<void, SocketErrorCode>::Success();
    }

    Result<void, SocketErrorCode> Socket::Connect(const IpAddress& address, unsigned short port) const
    {
        assert(mHandle != InvalidHandle);

        SocketAddressConverter socketAddressConverter = SocketAddressConverter::Of(address, port);

        int error = ::connect(mHandle, socketAddressConverter.AsSockAddr(), socketAddressConverter.GetLength());
        if (error == SOCKET_ERROR)
            return Result<void, SocketErrorCode>::Failure(GetLastSocketError());

        return Result<void, SocketErrorCode>::Success();
    }

    Result<void, SocketErrorCode> Socket::Listen(int backlog) const
    {
        assert(mHandle != InvalidHandle);

        int error = ::listen(mHandle, backlog);
        if (error == SOCKET_ERROR)
            return Result<void, SocketErrorCode>::Failure(GetLastSocketError());

        return Result<void, SocketErrorCode>::Success();
    }

    Result<unsigned int, SocketErrorCode> Socket::Receive(void* data, unsigned int length) const
    {
        assert(data && length);
        assert(mHandle != InvalidHandle);

        int received = ::recv(mHandle, static_cast<char*>(data), static_cast<int>(length), 0);
        if (received == SOCKET_ERROR)
            return Result<unsigned int, SocketErrorCode>::Failure(GetLastSocketError());

        return Result<unsigned int, SocketErrorCode>::Success(static_cast<unsigned int>(received));
    }

    Result<unsigned int, SocketErrorCode> Socket::ReceiveFrom(void* data, unsigned int length, SocketAddress& socketAddress) const
    {
        assert(data && length);
        assert(mHandle != InvalidHandle);

        sockaddr_in6 sockAddr;
        SocketAddressLength socketAddressLength = sizeof(sockAddr);
        int received = ::recvfrom(mHandle, static_cast<char*>(data), static_cast<int>(length), 0, reinterpret_cast<sockaddr*>(&sockAddr), &socketAddressLength);
        if (received == SOCKET_ERROR)
            return Result<unsigned int, SocketErrorCode>::Failure(GetLastSocketError());

        SocketAddressConverter socketAddressConverter(reinterpret_cast<const sockaddr&>(sockAddr), static_cast<int>(socketAddressLength));
        switch (socketAddressConverter.GetProtocol())
        {
            case IpAddress::Protocol::Ipv4: socketAddress = SocketAddress(socketAddressConverter.ToIpAddressV4(), socketAddressConverter.GetPort()); break;
            case IpAddress::Protocol::Ipv6: socketAddress = SocketAddress(socketAddressConverter.ToIpAddressV6(), socketAddressConverter.GetPort()); break;
        }

        return Result<unsigned int, SocketErrorCode>::Success(static_cast<unsigned int>(received));
    }

    Result<unsigned int, SocketErrorCode> Socket::Send(const void* data, unsigned int length) const
    {
        assert(data && length);
        assert(mHandle != InvalidHandle);

        int sent = ::send(mHandle, static_cast<const char*>(data), static_cast<int>(length), 0);
        if (sent == SOCKET_ERROR)
            return Result<unsigned int, SocketErrorCode>::Failure(GetLastSocketError());

        return Result<unsigned int, SocketErrorCode>::Success(static_cast<unsigned int>(sent));
    }

    Result<unsigned int, SocketErrorCode> Socket::SendTo(const void* data, unsigned int length, const IpAddress& address, unsigned short port) const
    {
        assert(data && length);
        assert(mHandle != InvalidHandle);

        SocketAddressConverter socketAddressConverter = SocketAddressConverter::Of(address, port);
        int sent = ::sendto(mHandle, static_cast<const char*>(data), static_cast<int>(length), 0, socketAddressConverter.AsSockAddr(), socketAddressConverter.GetLength());
        if (sent == SOCKET_ERROR)
            return Result<unsigned int, SocketErrorCode>::Failure(GetLastSocketError());

        return Result<unsigned int, SocketErrorCode>::Success(static_cast<unsigned int>(sent));
    }

    Result<void, SocketErrorCode> Socket::GetLocalAddress(SocketAddress& socketAddress) const
    {
        assert(mHandle != InvalidHandle);

        sockaddr_in6 sockAddr;
        SocketAddressLength socketAddressLength = sizeof(sockAddr);
        int error = ::getsockname(mHandle, reinterpret_cast<sockaddr*>(&sockAddr), &socketAddressLength);
        if (error == SOCKET_ERROR)
            return Result<void, SocketErrorCode>::Failure(GetLastSocketError());

        SocketAddressConverter socketAddressConverter(reinterpret_cast<const sockaddr&>(sockAddr), static_cast<int>(socketAddressLength));
        switch (socketAddressConverter.GetProtocol())
        {
            case IpAddress::Protocol::Ipv4: socketAddress = SocketAddress(socketAddressConverter.ToIpAddressV4(), socketAddressConverter.GetPort()); break;
            case IpAddress::Protocol::Ipv6: socketAddress = SocketAddress(socketAddressConverter.ToIpAddressV6(), socketAddressConverter.GetPort()); break;
        }

        return Result<void, SocketErrorCode>::Success();
    }

    bool Socket::IsValid() const
    {
        return mHandle != InvalidHandle;
    }

    bool Socket::IsBlocking() const
    {
        return mBlocking;
    }

    void Socket::SetBlocking(bool enable)
    {
        u_long notBlocking = enable ? 0 : 1;

#ifdef COCKTAIL_OS_WINDOWS
        if (ioctlsocket(mHandle, FIONBIO, &notBlocking) != NO_ERROR)
#else
        if (ioctl(mHandle, FIONBIO, &notBlocking) != 0)
#endif
            throw SystemError::GetLastError();

        mBlocking = enable;
    }

    unsigned int Socket::GetPendingLength() const
    {
        assert(mHandle != InvalidHandle);

        u_long length = 0;
#ifdef COCKTAIL_OS_WINDOWS
        if (ioctlsocket(mHandle, FIONREAD, &length) != NO_ERROR)
#else
        if (ioctl(mHandle, FIONREAD, &length) != 0)
#endif
            throw SystemError::GetLastError();

        return length;
    }

    Socket::Handle Socket::GetSystemHandle() const
    {
        return mHandle;
    }

    Socket::Socket(Handle handle, bool blocking) :
        mHandle(handle),
        mBlocking(blocking)
    {
        assert(mHandle != InvalidHandle);
    }

    Result<void, SocketErrorCode> Socket::GetOption(SocketOptionLevel level, SocketOptionId option, void* value, std::size_t& size) const
    {
        assert(mHandle != InvalidHandle);

        SocketAddressLength length = static_cast<SocketAddressLength>(size);

        int error = ::getsockopt(mHandle, ToNativeOptionLevel(level), ToNativeOption(option), static_cast<char*>(value), &length);
        if (error == SOCKET_ERROR)
            return Result<void, SocketErrorCode>::Failure(GetLastSocketError());

        size = static_cast<std::size_t>(length);

        return Result<void, SocketErrorCode>::Success();
    }

    Result<void, SocketErrorCode> Socket::SetOption(SocketOptionLevel level, SocketOptionId option, const void* value, std::size_t size) const
    {
        assert(mHandle != InvalidHandle);

        SocketAddressLength length = static_cast<SocketAddressLength>(size);

        int error = ::setsockopt(mHandle, ToNativeOptionLevel(level), ToNativeOption(option), static_cast<const char*>(value), length);
        if (error == SOCKET_ERROR)
            return Result<void, SocketErrorCode>::Failure(GetLastSocketError());

        return Result<void, SocketErrorCode>::Success();
    }
}
