#ifndef COCKTAIL_VULKAN_APIVERSION_HPP
#define COCKTAIL_VULKAN_APIVERSION_HPP

namespace Ck::Vulkan
{
	/**
	 * \brief Enumerate of supported versions of the Vulkan API
	 * Old drivers and hardware might not support them all
	 * \see GetSupportedApiVersion
	 */
	enum class ApiVersion
	{
		/**
		 * \brief Vulkan 1.0
		 */
		Version_1_0,

		/**
		 * \brief Vulkan 1.1 
		 */
		Version_1_1,

		/**
		 * \brief Vulkan 1.2  
		 */
		Version_1_2,

		/**
		 * \brief Vulkan 1.3
		 */
		Version_1_3,

		/**
		 * \brief Vulkan 1.4
		 */
		Version_1_4,
	};

	/**
	 * \brief Latest version of Vulkan supported
	 */
	static constexpr ApiVersion LatestApiVersion = ApiVersion::Version_1_4;
}

#endif // COCKTAIL_VULKAN_APIVERSION_HPP
