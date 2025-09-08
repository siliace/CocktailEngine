#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Network/SocketHeader.hpp>
#include <Cocktail/Core/System/Network/Address/IpAddress.hpp>
#include <Cocktail/Core/System/Network/Address/IpAddressV4.hpp>
#include <Cocktail/Core/System/Network/Address/IpAddressV6.hpp>
#include <Cocktail/Core/Utility/StringConvertion.hpp>

namespace Ck
{
#ifdef COCKTAIL_OS_WINDOWS
	Array<std::unique_ptr<IpAddress>> IpAddress::Resolve(StringView hostname)
	{
		ADDRINFOW hints = {};
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;

		ADDRINFOW* results;
		if (GetAddrInfoW(hostname.GetData(), nullptr, &hints, &results) != 0)
			throw SystemError::GetLastError();

		Array<std::unique_ptr<IpAddress>> ipAddresses;
		for (ADDRINFOW* result = results; result; result = result->ai_next)
		{
			std::unique_ptr<IpAddress> ipAddress;
			if (result->ai_family == AF_INET)
			{
				sockaddr_in* ipv4 = reinterpret_cast<sockaddr_in*>(result->ai_addr);

				Uint8* bytes = reinterpret_cast<Uint8*>(&ipv4->sin_addr);
				
				ipAddress = std::make_unique<IpAddressV4>(0, 0, 0, 0);
				for (std::size_t i = 0; i < 4; i++)
					ipAddress->SetByte(i, bytes[i]);
			}
			else if (result->ai_family == AF_INET6)
			{
				sockaddr_in6* ipv6 = reinterpret_cast<sockaddr_in6*>(result->ai_addr);

				Uint8* bytes = reinterpret_cast<Uint8*>(&ipv6->sin6_addr);

				ipAddress = std::make_unique<IpAddressV6>();
				for (std::size_t i = 0; i < 16; i++)
					ipAddress->SetByte(i, bytes[i]);
			}
			else
			{
				continue;
			}

			ipAddresses.Add(std::move(ipAddress));
		}

		return ipAddresses;
	}
#else
	Array<std::unique_ptr<IpAddress>> IpAddress::Resolve(StringView hostname)
	{
		addrinfo hints = {};
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;

		addrinfo* results;
		if (getaddrinfo(CK_TEXT_TO_ANSI(hostname.GetData()), nullptr, &hints, &results) != 0)
			throw SystemError::GetLastError();

		Array<std::unique_ptr<IpAddress>> ipAddresses;
		for (addrinfo* result = results; result; result = result->ai_next)
		{
			std::unique_ptr<IpAddress> ipAddress;
			if (result->ai_family == AF_INET)
			{
				sockaddr_in* ipv4 = reinterpret_cast<sockaddr_in*>(result->ai_addr);

				Uint8* bytes = reinterpret_cast<Uint8*>(&ipv4->sin_addr);

				ipAddress = std::make_unique<IpAddressV4>(0, 0, 0, 0);
				for (std::size_t i = 0; i < 4; i++)
					ipAddress->SetByte(i, bytes[i]);
			}
			else if (result->ai_family == AF_INET6)
			{
				sockaddr_in6* ipv6 = reinterpret_cast<sockaddr_in6*>(result->ai_addr);

				Uint8* bytes = reinterpret_cast<Uint8*>(&ipv6->sin6_addr);

				ipAddress = std::make_unique<IpAddressV6>();
				for (std::size_t i = 0; i < 16; i++)
					ipAddress->SetByte(i, bytes[i]);
			}
			else
			{
				continue;
			}

			ipAddresses.Add(std::move(ipAddress));
		}

		return ipAddresses;
	}
#endif
}
