#ifndef COCKTAIL_CORE_SYSTEM_NETWORK_ADDRESS_IPADDRESS_HPP
#define COCKTAIL_CORE_SYSTEM_NETWORK_ADDRESS_IPADDRESS_HPP

#include <memory>

#include <Cocktail/Core/String.hpp>

namespace Ck
{
    /**
     * \brief Base interface for ip address classes
     */
    class COCKTAIL_CORE_API IpAddress
    {
    public:

	    /**
         * \brief 
         */
        enum class Protocol
        {
	        /**
             * \brief 
             */
            Ipv4,

            /**
             * \brief 
             */
            Ipv6
        };

        /**
         * \brief 
         * \param hostname 
         * \return 
         */
        static Array<std::unique_ptr<IpAddress>> Resolve(StringView hostname);

        /**
         * \brief Destructor
         */
        virtual ~IpAddress() = default;

	    /**
         * \brief 
         * \return 
         */
        virtual std::size_t GetLength() const = 0;

	    /**
         * \brief 
         * \param index 
         * \return 
         */
        virtual Uint8 GetByte(std::size_t index) const = 0;

	    /**
         * \brief 
         * \param index 
         * \param byte 
         */
        virtual void SetByte(std::size_t index, Uint8 byte) = 0;

        /**
         * \brief Get the protocol used by the ip address
         * \return The protocol
         */
        virtual Protocol GetProtocol() const = 0;

        /**
         * \brief Convert the ip address to a string representation
         * \return
         */
        virtual String ToString() const = 0;   
    };
}

#endif // COCKTAIL_CORE_SYSTEM_NETWORK_ADDRESS_IPADDRESS_HPP
