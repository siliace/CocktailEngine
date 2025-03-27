#ifndef COCKTAIL_CORE_SYSTEM_NETWORK_UNIX_SOCKETPOLLERIMPL_HPP
#define COCKTAIL_CORE_SYSTEM_NETWORK_UNIX_SOCKETPOLLERIMPL_HPP

#include <poll.h>

#include <Cocktail/Core/System/Network/SocketPoller.hpp>

namespace Ck
{
	class SocketPoller::Impl
	{
	public:

		/**
		 * \brief
		 * \param socketHandle
		 * \param flags
		 */
		void Add(Socket::Handle socketHandle, EventFlags flags);

		/**
		 * \brief
		 * \param socketHandle
		 */
		void Remove(Socket::Handle socketHandle);

		/**
		 * \brief
		 */
		void Clear();

		/**
		 * \brief
		 * \param socketHandle
		 * \return
		 */
		bool Has(Socket::Handle socketHandle) const;

		/**
		 * \brief
		 * \param duration
		 * \return
		 */
		bool Wait(const Duration& duration);

		/**
		 * \brief 
		 * \param socket 
		 * \return 
		 */
		bool IsDisconnected(Socket::Handle socket) const;

		/**
		 * \brief 
		 * \param socket 
		 * \return 
		 */
		bool IsReadyToRead(Socket::Handle socket) const;

		/**
		 * \brief 
		 * \param socket 
		 * \return 
		 */
		bool IsReadyToWrite(Socket::Handle socket) const;

	private:

		const pollfd* GetSocketDescriptor(Socket::Handle socket) const;

		std::vector<pollfd> mDescriptors;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_NETWORK_UNIX_SOCKETPOLLERIMPL_HPP
