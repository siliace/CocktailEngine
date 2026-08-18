#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_UDPSOCKET_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_UDPSOCKET_HPP

#include <CocktailEngine/Core/System/Network/Socket.hpp>
#include <CocktailEngine/Core/System/Network/Address/IpAddressV4.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAILENGINE_CORE_API UdpSocket : public Socket
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
		std::tuple<std::size_t, UniquePtr<IpAddress>, unsigned short> Receive(void* data, std::size_t length) const;

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

#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_UDPSOCKET_HPP
