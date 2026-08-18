#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_TCPSERVER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_TCPSERVER_HPP

#include <CocktailEngine/Core/System/Network/TcpClient.hpp>
#include <CocktailEngine/Core/System/Network/Address/IpAddressV4.hpp>

namespace Ck
{
	class COCKTAILENGINE_CORE_API TcpServer : public Socket
	{
	public:

		/**
		 * \brief 
		 * \param port 
		 * \param host 
		 * \param backlog 
		 */
		explicit TcpServer(unsigned short port, const IpAddress& host = IpAddressV4::Any, int backlog = -1);

		/**
		 * \brief 
		 * \return 
		 */
		TcpClient Accept() const;
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_TCPSERVER_HPP
