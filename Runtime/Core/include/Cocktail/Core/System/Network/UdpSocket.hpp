#ifndef COCKTAIL_CORE_SYSTEM_NETWORK_UDPSOCKET_HPP
#define COCKTAIL_CORE_SYSTEM_NETWORK_UDPSOCKET_HPP

#include <Cocktail/Core/System/Network/Socket.hpp>
#include <Cocktail/Core/System/Network/Address/IpAddressV4.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API UdpSocket : public Socket
	{
	public:

		/**
		 * \brief 
		 * \param protocol 
		 */
		explicit UdpSocket(IpAddress::Protocol protocol);

		/**
		 * \brief 
		 * \param port 
		 * \param host 
		 */
		explicit UdpSocket(unsigned short port, const IpAddress& host = IpAddressV4::Any);

		/**
		 * \brief 
		 * \param data 
		 * \param length
		 * \return
		 */
		std::tuple<std::size_t, std::unique_ptr<IpAddress>, unsigned short> Receive(void* data, std::size_t length) const;

		/**
		 * \brief 
		 * \param data 
		 * \param length 
		 * \param remoteAddress 
		 * \param port 
		 * \return 
		 */
		std::size_t Send(const void* data, std::size_t length, const IpAddress& remoteAddress, unsigned short port) const;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_NETWORK_UDPSOCKET_HPP
