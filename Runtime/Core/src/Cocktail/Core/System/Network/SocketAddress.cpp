#include <cstring>

#include <Cocktail/Core/System/Network/SocketAddress.hpp>

namespace Ck
{
	SocketAddress SocketAddress::Of(const IpAddress& ipAddress, unsigned short port)
	{
		if (ipAddress.GetProtocol() == IpAddress::Protocol::Ipv4)
		{
			sockaddr_in sockAddr;
			std::memset(&sockAddr, 0, sizeof(sockaddr_in));

			Uint8* bytes = reinterpret_cast<Uint8*>(&sockAddr.sin_addr);

			sockAddr.sin_family = AF_INET;
			sockAddr.sin_port = htons(port);
			for (std::size_t i = 0; i < ipAddress.GetLength(); i++)
				bytes[i] = ipAddress.GetByte(i);

			return SocketAddress(sockAddr);
		}
		
		sockaddr_in6 sockAddr;	
		std::memset(&sockAddr, 0, sizeof(sockaddr_in6));
		sockAddr.sin6_family = AF_INET6;
		sockAddr.sin6_port = htons(port);

		Uint8* bytes = reinterpret_cast<Uint8*>(&sockAddr.sin6_addr);
	
		for (std::size_t i = 0; i < ipAddress.GetLength(); i++)
			bytes[i] = ipAddress.GetByte(i);
	
		return SocketAddress(sockAddr);
	}

	SocketAddress::SocketAddress(const sockaddr_in& sockaddr) :
		mLength(sizeof(sockaddr_in))
	{
		mAddress.V4 = sockaddr;
	}

	SocketAddress::SocketAddress(const sockaddr_in6& sockaddr) :
		mLength(sizeof(sockaddr_in6))
	{
		mAddress.V6 = sockaddr;
	}

	SocketAddress::SocketAddress(const sockaddr& sockAddr, int sockAddrLength)
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

	const sockaddr* SocketAddress::AsSockAddr() const
	{
		return reinterpret_cast<const sockaddr*>(&mAddress);
	}

	std::unique_ptr<IpAddress> SocketAddress::ToIpAddress() const
	{
		switch (AsSockAddr()->sa_family)
		{
		case AF_INET:
			return ToIpAddressV4();

		case AF_INET6:
			return ToIpAddressV6();
		}

		return nullptr;
	}

	unsigned short SocketAddress::GetPort() const
	{
		switch (AsSockAddr()->sa_family)
		{
		case AF_INET:
			return ntohs(reinterpret_cast<const sockaddr_in*>(this)->sin_port);

		case AF_INET6:
			return ntohs(reinterpret_cast<const sockaddr_in6*>(this)->sin6_port);
		}

		return 0;
	}

	int SocketAddress::GetLength() const
	{
		return mLength;
	}

	std::unique_ptr<IpAddressV4> SocketAddress::ToIpAddressV4() const
	{
		const sockaddr_in* addr = reinterpret_cast<const sockaddr_in*>(this);
		const Uint8* bytes = reinterpret_cast<const Uint8*>(&addr->sin_addr);

		return std::make_unique<IpAddressV4>(bytes[0], bytes[1], bytes[2], bytes[3]);
	}

	std::unique_ptr<IpAddressV6> SocketAddress::ToIpAddressV6() const
	{
		const sockaddr_in6* addr = reinterpret_cast<const sockaddr_in6*>(this);
		const Uint8* bytes = reinterpret_cast<const Uint8*>(&addr->sin6_addr);

		std::unique_ptr<IpAddressV6> ipAddress = std::make_unique<IpAddressV6>();
		for (std::size_t i = 0; i < 16; i++)
			ipAddress->SetByte(i, bytes[i]);

		return ipAddress;
	}
}
