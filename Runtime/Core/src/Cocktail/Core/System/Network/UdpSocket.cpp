#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Network/SocketAddress.hpp>
#include <Cocktail/Core/System/Network/UdpSocket.hpp>

namespace Ck
{
	UdpSocket::UdpSocket(IpAddress::Protocol protocol) :
		Socket(protocol, Type::Udp)
	{
		assert(mHandle != InvalidHandle);
	}

	UdpSocket::UdpSocket(unsigned short port, const IpAddress& host) :
		Socket(host.GetProtocol(), Type::Udp)
	{
		assert(port != 0);
		assert(mHandle != InvalidHandle);

		SocketAddress socketAddress = SocketAddress::Of(host, port);
		int error = ::bind(mHandle, socketAddress.AsSockAddr(), socketAddress.GetLength());
		if (error == -1)
			throw SystemError::GetLastError();
	}

	std::tuple<std::size_t, UniquePtr<IpAddress>, unsigned short> UdpSocket::Receive(void* data, std::size_t length) const
	{
		assert(data && length);
		assert(mHandle != InvalidHandle);

		sockaddr_in6 sockAddr;
		int sockaddrlength = sizeof(sockaddr_in6);
		int received = ::recvfrom(mHandle, static_cast<char*>(data), static_cast<int>(length), 0, reinterpret_cast<sockaddr*>(&sockAddr), (socklen_t*)&sockaddrlength);
		if (received == -1)
			throw SystemError::GetLastError();

		SocketAddress socketAddress(reinterpret_cast<const sockaddr&>(sockAddr), sockaddrlength);
		return std::make_tuple(received, socketAddress.ToIpAddress(), socketAddress.GetPort());
	}

	std::size_t UdpSocket::Send(const void* data, std::size_t length, const IpAddress& remoteAddress, unsigned short port) const
	{
		assert(data && length);
		assert(mHandle != InvalidHandle);

		SocketAddress socketAddress = SocketAddress::Of(remoteAddress, port);
		int sent = ::sendto(mHandle, static_cast<const char*>(data), static_cast<int>(length), 0, socketAddress.AsSockAddr(), socketAddress.GetLength());
		if (sent == -1)
			throw SystemError::GetLastError();

		return sent;
	}
}
