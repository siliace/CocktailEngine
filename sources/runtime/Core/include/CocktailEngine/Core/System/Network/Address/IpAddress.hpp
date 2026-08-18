#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_ADDRESS_IPADDRESS_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_ADDRESS_IPADDRESS_HPP

#include <CocktailEngine/Core/String.hpp>

namespace Ck
{
    /**
     * \brief Base interface for ip address classes
     */
    class COCKTAILENGINE_CORE_API IpAddress
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
        static Array<UniquePtr<IpAddress>> Resolve(StringView hostname);

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

#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_ADDRESS_IPADDRESS_HPP
