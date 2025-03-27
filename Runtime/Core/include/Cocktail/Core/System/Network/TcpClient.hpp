#ifndef COCKTAIL_CORE_SYSTEM_NETWORK_TCPCLIENT_HPP
#define COCKTAIL_CORE_SYSTEM_NETWORK_TCPCLIENT_HPP

#include <Cocktail/Core/System/Network/Socket.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

namespace Ck
{
	class COCKTAIL_CORE_API TcpClient : public Socket
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
		TcpClient(Handle handle, std::unique_ptr<IpAddress> remoteAddress, unsigned short remotePort);

		std::unique_ptr<IpAddress> mRemoteAddress;
		unsigned short mRemotePort;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_NETWORK_TCPCLIENT_HPP
