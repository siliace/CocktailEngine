#ifndef COCKTAIL_CORE_SYSTEM_NETWORK_SOCKETADDRESS_HPP
#define COCKTAIL_CORE_SYSTEM_NETWORK_SOCKETADDRESS_HPP

#include <Cocktail/Core/System/Network/SocketHeader.hpp>
#include <Cocktail/Core/System/Network/Address/IpAddressV4.hpp>
#include <Cocktail/Core/System/Network/Address/IpAddressV6.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class SocketAddress
	{
	public:

		/**
		 * \brief 
		 * \param ipAddress 
		 * \param port 
		 * \return 
		 */
		static SocketAddress Of(const IpAddress& ipAddress, unsigned short port);

		/**
		 * \brief
		 * \param sockaddr
		 */
		explicit SocketAddress(const sockaddr_in& sockaddr);

		/**
		 * \brief 
		 * \param sockaddr 
		 */
		explicit SocketAddress(const sockaddr_in6& sockaddr);

		/**
		 * \brief 
		 * \param sockAddr 
		 * \param sockAddrLength 
		 */
		SocketAddress(const sockaddr& sockAddr, int sockAddrLength);

		/**
		 * \brief 
		 * \return 
		 */
		const sockaddr* AsSockAddr() const;

		/**
		 * \brief 
		 * \return 
		 */
		UniquePtr<IpAddress> ToIpAddress() const;

		/**
		 * \brief
		 * \return
		 */
		unsigned short GetPort() const;

		/**
		 * \brief
		 * \return
		 */
		int GetLength() const;

	private:

		/**
		 * \brief 
		 * \return 
		 */
		UniquePtr<IpAddressV4> ToIpAddressV4() const;

		/**
		 * \brief 
		 * \return 
		 */
		UniquePtr<IpAddressV6> ToIpAddressV6() const;

		union
		{
			sockaddr_in V4;
			sockaddr_in6 V6;
		} mAddress;

		int mLength;
	};
}
#endif // COCKTAIL_CORE_SYSTEM_NETWORK_SOCKETADDRESS_HPP
