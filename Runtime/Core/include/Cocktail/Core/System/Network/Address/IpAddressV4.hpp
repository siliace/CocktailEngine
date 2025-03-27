#ifndef COCKTAIL_CORE_SYSTEM_NETWORK_ADDRESS_IPADDRESSV4_HPP
#define COCKTAIL_CORE_SYSTEM_NETWORK_ADDRESS_IPADDRESSV4_HPP

#include <array>

#include <Cocktail/Core/Export.hpp>
#include <Cocktail/Core/System/Network/Address/IpAddress.hpp>

namespace Ck
{
	/**
     * \brief 
     */
    class COCKTAIL_CORE_API IpAddressV4 final : public IpAddress
    {
    public:

	    /**
		 * \brief 
		 */
		static const IpAddressV4 Any;

	    /**
         * \brief 
         */
        static const IpAddressV4 Localhost;

	    /**
		 * \brief 
		 */
		static const IpAddressV4 Broadcast;

        /**
         * \brief Constructor
         */
        explicit IpAddressV4(Uint32 ipAddress);

        /**
         * \brief Constructor
         * \param first
         * \param second
         * \param third
         * \param forth
         */
        IpAddressV4(Uint8 first, Uint8 second, Uint8 third, Uint8 forth);

	    /**
	     * \brief 
	     * \param other 
	     */
	    IpAddressV4(const IpAddressV4& other) = default;

	    /**
	     * \brief 
	     * \param other 
	     */
	    IpAddressV4(IpAddressV4&& other) noexcept = default;

	    /**
	     * \brief 
	     * \param other 
	     * \return 
	     */
	    IpAddressV4& operator=(const IpAddressV4& other) = default;

	    /**
	     * \brief 
	     * \param other 
	     * \return 
	     */
	    IpAddressV4& operator=(IpAddressV4&& other) noexcept = default;

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
	     * \param byte 
	     */
	    void SetByte(std::size_t index, Uint8 byte) override;

	    /**
         * \brief 
         * \return 
         */
        Uint32 ToInteger() const;

        /**
         * \brief Get the protocol used by the ip address
         * \return The protocol
         */
        Protocol GetProtocol() const override;

        /**
         * \brief
         * \return
         */
        std::string ToString() const override;

	    /**
         * \brief 
         * \param rhs 
         * \return 
         */
        bool operator==(const IpAddressV4& rhs) const;

	    /**
	     * \brief 
	     * \param rhs 
	     * \return 
	     */
	    bool operator!=(const IpAddressV4& rhs) const;

    private:
        
        std::array<Uint8, 4> mBytes;
    };
}
#endif // COCKTAIL_CORE_SYSTEM_NETWORK_ADDRESS_IPADDRESSV4_HPP
