#ifndef COCKTAIL_CORE_SYSTEM_NETWORK_SOCKETPOLLER_HPP
#define COCKTAIL_CORE_SYSTEM_NETWORK_SOCKETPOLLER_HPP

#include <Cocktail/Core/Flags.hpp>
#include <Cocktail/Core/System/Network/Socket.hpp>
#include <Cocktail/Core/Utility/Time/Duration.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API SocketPoller
	{
	public:

		enum class EventFlagBits
		{
			Read = Bit(0),
			Write = Bit(1)
		};

		using EventFlags = Flags<EventFlagBits>;

		/**
		 * \brief 
		 */
		SocketPoller();

		/**
		 * \brief 
		 */
		~SocketPoller();

		/**
		 * \brief 
		 * \param socket 
		 * \param eventFlags 
		 */
		void Add(const Socket& socket, EventFlags eventFlags) const;

		/**
		 * \brief 
		 * \param socket 
		 */
		void Remove(const Socket& socket) const;

		/**
		 * \brief 
		 */
		void Clear() const;

		/**
		 * \brief 
		 * \param timeout 
		 * \return
		 */
		bool Wait(Duration timeout = Duration::Infinite()) const;

		/**
		 * \brief 
		 * \param socket 
		 * \return 
		 */
		bool IsDisconnected(const Socket& socket) const;

		/**
		 * \brief 
		 * \param socket 
		 * \return
		 */
		bool IsReadyToRead(const Socket& socket) const;

		/**
		 * \brief 
		 * \param socket
		 * \return
		 */
		bool IsReadyToWrite(const Socket& socket) const;

	private:

		/**
		 * \brief 
		 */
		class Impl;
		Impl* mImpl;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_NETWORK_SOCKETPOLLER_HPP
