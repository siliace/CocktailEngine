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
		 * \brief 
		 */
		Version_1_0,

		/**
		 * \brief 
		 */
		Version_1_1,

		/**
		 * \brief 
		 */
		Version_1_2,

		/**
		 * \brief 
		 */
		Version_1_3,
	};

	/**
	 * \brief 
	 */
	static constexpr ApiVersion LatestApiVersion = ApiVersion::Version_1_3;
}

#endif // COCKTAIL_VULKAN_APIVERSION_HPP
