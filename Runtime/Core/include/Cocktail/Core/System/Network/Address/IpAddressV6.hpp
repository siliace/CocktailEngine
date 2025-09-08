#ifndef COCKTAIL_CORE_SYSTEM_NETWORK_ADDRESS_IPADDRESSV6_HPP
#define COCKTAIL_CORE_SYSTEM_NETWORK_ADDRESS_IPADDRESSV6_HPP

#include <array>

#include <Cocktail/Core/System/Network/Address/IpAddress.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API IpAddressV6 final : public IpAddress
	{
	public:

		/**
		 * \brief 
		 */
		static const IpAddressV6 Any;

		/**
		 * \brief 
		 */
		static const IpAddressV6 Localhost;

		/**
		 * \brief 
		 */
		IpAddressV6() = default;

		/**
		 * \brief 
		 * \param a 
		 * \param b 
		 * \param c 
		 * \param d 
		 * \param e 
		 * \param f 
		 * \param g 
		 * \param h 
		 */
		IpAddressV6(Uint16 a, Uint16 b, Uint16 c, Uint16 d, Uint16 e, Uint16 f, Uint16 g, Uint16 h);

		/**
		 * \brief 
		 * \param other 
		 */
		IpAddressV6(const IpAddressV6& other) = default;

		/**
		 * \brief 
		 * \param other 
		 */
		IpAddressV6(IpAddressV6&& other) noexcept = default;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		IpAddressV6& operator=(const IpAddressV6& other) = default;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		IpAddressV6& operator=(IpAddressV6&& other) noexcept = default;

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetLength() const override;

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		Uint8 GetByte(std::size_t index) const override;

		/**
		 * \brief 
		 * \param index 
		 * \param value 
		 */
		void SetByte(std::size_t index, Uint8 value) override;

		/**
		 * \brief
		 * \param index
		 * \return
		 */
		Uint16 GetShort(std::size_t index) const;

		/**
		 * \brief
		 * \param index
		 * \param value
		 */
		void SetShort(std::size_t index, Uint16 value);

		/**
		 * \brief 
		 * \return 
		 */
		Protocol GetProtocol() const override;

		/**
		 * \brief 
		 * \return 
		 */
		String ToString() const override;

	private:

		union
		{
			std::array<Uint8, 16> Bytes;
			std::array<Uint16, 8> Shorts;
		};
	};
}

#endif // COCKTAIL_CORE_SYSTEM_NETWORK_ADDRESS_IPADDRESSV6_HPP
