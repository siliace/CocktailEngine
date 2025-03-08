#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Network/Socket.hpp>
#include <Cocktail/Core/System/Network/SocketAddress.hpp>
#include <Cocktail/Core/System/Network/Address/IpAddressV4.hpp>
#include <Cocktail/Core/System/Network/Address/IpAddressV6.hpp>

namespace Ck
{
	namespace
	{
		int ToAddressFamily(IpAddress::Protocol protocol)
		{
			switch (protocol)
			{
			case IpAddress::Protocol::Ipv4:
				return AF_INET;

			case IpAddress::Protocol::Ipv6:
				return AF_INET6;
			}

			COCKTAIL_UNREACHABLE();
		}

		int ToSockType(Socket::Type type)
		{
			switch (type)
			{
			case Socket::Type::Tcp:
				return SOCK_STREAM;

			case Socket::Type::Udp:
				return SOCK_DGRAM;

			case Socket::Type::Raw:
				return SOCK_RAW;
			}

			COCKTAIL_UNREACHABLE();
		}
	}

	Socket::Socket(IpAddress::Protocol protocol, Type type)
	{
		mHandle = ::socket(ToAddressFamily(protocol), ToSockType(type), 0);
		if (!mHandle)
			throw SystemError::GetLastError();
	}

	Socket::Socket(Socket&& other) noexcept
	{
		*this = std::move(other);
	}

	Socket::~Socket()
	{
		closesocket(mHandle);
	}

	Socket& Socket::operator=(Socket&& other) noexcept
	{
		mHandle = std::exchange(other.mHandle, InvalidHandle);

		return *this;
	}

	std::size_t Socket::GetPendingLength() const
	{
		assert(mHandle != InvalidHandle);

		u_long length = 0;
		if (ioctlsocket(mHandle, FIONREAD, &length) != NO_ERROR)
			throw SystemError::GetLastError();

		return length;
	}

	void Socket::EnableBlockingMode(bool enable) const
	{
		u_long blocking = enable ? 1 : 0;

		if (ioctlsocket(mHandle, FIONBIO, &blocking) != NO_ERROR)
			throw SystemError::GetLastError();
	}

	Socket::Socket(Handle handle) :
		mHandle(handle)
	{
		assert(mHandle != InvalidHandle);
	}
}
