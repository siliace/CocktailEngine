#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Network/Address/IpAddress.hpp>
#include <Cocktail/Core/System/Network/Address/IpAddressV4.hpp>
#include <Cocktail/Core/System/Network/Address/IpAddressV6.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck
{
	std::vector<std::unique_ptr<IpAddress>> IpAddress::Resolve(const std::string& hostname)
	{
		addrinfo hints = {};
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;

		addrinfo* results;
		if (getaddrinfo(hostname.c_str(), nullptr, &hints, &results) != 0)
			throw SystemError::GetLastError();

		std::vector<std::unique_ptr<IpAddress>> ipAddresses;
		for (addrinfo* result = results; result; result = result->ai_next)
		{
			std::unique_ptr<IpAddress> ipAddress;
			if (result->ai_family == AF_INET)
			{
				sockaddr_in* ipv4 = reinterpret_cast<sockaddr_in*>(result->ai_addr);
				ipAddress = std::make_unique<IpAddressV4>(ipv4->sin_addr.S_un.S_addr);
			}
			else if (result->ai_family == AF_INET6)
			{
				sockaddr_in6* ipv6 = reinterpret_cast<sockaddr_in6*>(result->ai_addr);

				ipAddress = std::make_unique<IpAddressV6>();
				for (std::size_t i = 0; i < 16; i++)
					ipAddress->SetByte(i, ipv6->sin6_addr.u.Byte[i]);
			}
			else
			{
				continue;
			}

			ipAddresses.push_back(std::move(ipAddress));
		}

		return ipAddresses;
	}
}
