#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_TCPCLIENT_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_TCPCLIENT_HPP

#include <CocktailEngine/Core/System/Network/Socket.hpp>
#include <CocktailEngine/Core/Utility/ByteArray.hpp>

namespace Ck
{
	class COCKTAILENGINE_CORE_API TcpClient : public Socket
	{
	public:

		/**
		 * \brief 
		 */
		TcpClient(const IpAddress& remoteAddress, unsigned short port);

		/**
		 * \brief 
		 * \param data 
		 * \param length 
		 */
		void Send(const void* data, std::size_t length) const;

		/**
		 * \brief 
		 * \param data 
		 * \param length 
		 * \return 
		 */
		std::size_t Receive(void* data, std::size_t length) const;

		/**
		 * \brief 
		 * \return 
		 */
		const IpAddress* GetRemoteAddress() const;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned short GetRemotePort() const;

	private:

		friend class TcpServer;

		/**
		 * \brief 
		 * \param handle 
		 * \param remoteAddress 
		 * \param remotePort 
		 */
		TcpClient(Handle handle, UniquePtr<IpAddress> remoteAddress, unsigned short remotePort);

		UniquePtr<IpAddress> mRemoteAddress;
		unsigned short mRemotePort;
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_TCPCLIENT_HPP
