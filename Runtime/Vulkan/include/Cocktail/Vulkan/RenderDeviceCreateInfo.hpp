#ifndef COCKTAIL_VULKAN_RENDERDEVICECREATEINFO_HPP
#define COCKTAIL_VULKAN_RENDERDEVICECREATEINFO_HPP

#include <Cocktail/Vulkan/ApiVersion.hpp>
#include <Cocktail/Vulkan/VersionDescriptor.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	struct RenderDeviceCreateInfo
	{
		/**
		 * \brief Specifies the name of the application to register in the vulkan's instance
		 */
		std::string_view ApplicationName;

		/**
		 * \brief Specifies the version of the application to regiter in the vulkan's instance
		 */
		VersionDescriptor ApplicationVersion = { 0, 0, 0 };

		/**
		 * \brief Specifies the version of vulkan to use
		 */
		ApiVersion ApiVersion = ApiVersion::Version_1_0;

		/**
		 * \brief Specifies the size of GPU memory chunk to allocate
		 */
		unsigned int DeviceMemoryChunkSize = 1024 * 1024 * 16;

		/**
		 * \brief Flag to enable a validation layer if supported by the system
		 */
		bool EnableValidation = false;
	};
}

#endif // COCKTAIL_VULKAN_RENDERDEVICECREATEINFO_HPP
