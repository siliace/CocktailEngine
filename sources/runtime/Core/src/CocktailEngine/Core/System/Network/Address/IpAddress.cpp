#include <CocktailEngine/Core/System/Network/Address/IpAddress.hpp>
#include <CocktailEngine/Core/System/Network/Address/IpAddressV4.hpp>
#include <CocktailEngine/Core/System/Network/Address/IpAddressV6.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketHeader.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>
#include <CocktailEngine/Core/Utility/Finally.hpp>

namespace Ck
{
    String AddressResolutionErrorCodeTranslator::TranslateErrorCode(int errorCode) const
    {
#ifdef COCKTAIL_OS_WINDOWS
        return SystemErrorCodeTranslator::Instance.TranslateErrorCode(static_cast<unsigned int>(errorCode));
#else
        const char* description = ::gai_strerror(errorCode);
        if (description != nullptr)
            return String(reinterpret_cast<const Utf8Char*>(description));

        return String::Format(CK_TEXT("Unknown address-resolution error (%d)"), errorCode);
#endif
    }

#ifdef COCKTAIL_OS_WINDOWS
    Array<UniquePtr<IpAddress>> IpAddress::Resolve(StringView hostname)
    {
        ADDRINFOW hints = {};
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        ADDRINFOW* results = nullptr;
        int errorCode = GetAddrInfoW(hostname.GetData(), nullptr, &hints, &results);
        if (errorCode != 0)
            throw AddressResolutionException(errorCode);

        Finally finally([results] {
            FreeAddrInfoW(results);
        });

        Array<UniquePtr<IpAddress>> ipAddresses;
        for (ADDRINFOW* result = results; result; result = result->ai_next)
        {
            UniquePtr<IpAddress> ipAddress;
            if (result->ai_family == AF_INET)
            {
                sockaddr_in* ipv4 = reinterpret_cast<sockaddr_in*>(result->ai_addr);

                Uint8* bytes = reinterpret_cast<Uint8*>(&ipv4->sin_addr);

                ipAddress = MakeUnique<IpAddressV4>(0, 0, 0, 0);
                for (std::size_t i = 0; i < 4; i++)
                    ipAddress->SetByte(i, bytes[i]);
            }
            else if (result->ai_family == AF_INET6)
            {
                sockaddr_in6* ipv6 = reinterpret_cast<sockaddr_in6*>(result->ai_addr);

                Uint8* bytes = reinterpret_cast<Uint8*>(&ipv6->sin6_addr);

                ipAddress = MakeUnique<IpAddressV6>();
                for (std::size_t i = 0; i < 16; i++)
                    ipAddress->SetByte(i, bytes[i]);
            }
            else
            {
                continue;
            }

            ipAddresses.Add(Move(ipAddress));
        }

        return ipAddresses;
    }
#else
    Array<UniquePtr<IpAddress>> IpAddress::Resolve(StringView hostname)
    {
        addrinfo hints = {};
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        addrinfo* results = nullptr;
        int errorCode = ::getaddrinfo(reinterpret_cast<const AnsiChar*>(hostname.GetData()), nullptr, &hints, &results);
        if (errorCode != 0)
            throw AddressResolutionException(errorCode);

        Finally finally([results] {
            ::freeaddrinfo(results);
        });

        Array<UniquePtr<IpAddress>> ipAddresses;
        for (addrinfo* result = results; result; result = result->ai_next)
        {
            UniquePtr<IpAddress> ipAddress;
            if (result->ai_family == AF_INET)
            {
                sockaddr_in* ipv4 = reinterpret_cast<sockaddr_in*>(result->ai_addr);

                Uint8* bytes = reinterpret_cast<Uint8*>(&ipv4->sin_addr);

                ipAddress = MakeUnique<IpAddressV4>(0, 0, 0, 0);
                for (std::size_t i = 0; i < 4; i++)
                    ipAddress->SetByte(i, bytes[i]);
            }
            else if (result->ai_family == AF_INET6)
            {
                sockaddr_in6* ipv6 = reinterpret_cast<sockaddr_in6*>(result->ai_addr);

                Uint8* bytes = reinterpret_cast<Uint8*>(&ipv6->sin6_addr);

                ipAddress = MakeUnique<IpAddressV6>();
                for (std::size_t i = 0; i < 16; i++)
                    ipAddress->SetByte(i, bytes[i]);
            }
            else
            {
                continue;
            }

            ipAddresses.Add(Move(ipAddress));
        }

        return ipAddresses;
    }
#endif
}
