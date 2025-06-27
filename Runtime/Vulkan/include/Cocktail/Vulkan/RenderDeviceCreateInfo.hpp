#ifndef COCKTAIL_VULKAN_RENDERDEVICECREATEINFO_HPP
#define COCKTAIL_VULKAN_RENDERDEVICECREATEINFO_HPP

#include <string_view>

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
		 * \brief 
		 */
		std::string_view ApplicationName;

		/**
		 * \brief 
		 */
		VersionDescriptor ApplicationVersion = { 0, 0, 0 };

		/**
		 * \brief 
		 */
		ApiVersion ApiVersion = ApiVersion::Version_1_0;

		/**
		 * \brief 
		 */
		unsigned int DeviceMemoryBlockSize = 1024 * 1024 * 64;

		/**
		 * \brief
		 */
		bool EnableValidation = false;
	};
}

#endif // COCKTAIL_VULKAN_RENDERDEVICECREATEINFO_HPP
