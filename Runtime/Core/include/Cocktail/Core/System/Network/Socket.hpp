#ifndef COCKTAIL_CORE_SYSTEM_NETWORK_SOCKET_HPP
#define COCKTAIL_CORE_SYSTEM_NETWORK_SOCKET_HPP

#include <Cocktail/Core/System/Network/Address/IpAddress.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API Socket
	{
	public:

#ifdef COCKTAIL_OS_WINDOWS
		using Handle = Uint64;
#else
		using Handle = int;
#endif

		/**
		 * \brief 
		 */
		enum class Type
		{
			/**
			 * \brief 
			 */
			Tcp,

			/**
			 * \brief 
			 */
			Udp,

			/**
			 * \brief 
			 */
			Raw
		};

		/**
		 * \brief 
		 */
		static constexpr Handle InvalidHandle = static_cast<Handle>(-1);

		/**
		 * \brief 
		 * \param protocol 
		 * \param type 
		 */
		Socket(IpAddress::Protocol protocol, Type type);

		/**
		 * \brief 
		 * \param other 
		 */
		Socket(const Socket& other) = delete;

		/**
		 * \brief 
		 * \param other 
		 */
		Socket(Socket&& other) noexcept;

		/**
		 * \brief 
		 */
		virtual ~Socket();

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		Socket& operator=(const Socket& other) = delete;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		Socket& operator=(Socket&& other) noexcept;

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetPendingLength() const;

		/**
		 * \brief 
		 * \param enable 
		 */
		void EnableBlockingMode(bool enable) const;

		/**
		 * \brief 
		 * \return 
		 */
		Handle GetSystemHandle() const;

	protected:

		/**
		 * \brief
		 * \param handle
		 */
		explicit Socket(Handle handle);

		Handle mHandle;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_NETWORK_SOCKET_HPP
