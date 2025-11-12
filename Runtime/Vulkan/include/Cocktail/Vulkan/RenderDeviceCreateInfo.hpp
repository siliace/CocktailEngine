#ifndef COCKTAIL_VULKAN_RENDERDEVICECREATEINFO_HPP
#define COCKTAIL_VULKAN_RENDERDEVICECREATEINFO_HPP

#include <Cocktail/Core/String.hpp>

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
		AnsiStringView ApplicationName;

		/**
		 * \brief
		 */
		VersionDescriptor ApplicationVersion = { 0, 0, 0 };

		/**
		 * \brief 
		 */
		ApiVersion ApiVersion = ApiVersion::Version_1_0;

		/**
		 * \brief Size of device memory allocation block
		 * Vulkan based render device will allocate video memory by block of this size
		 * and will perform sub allocation in these block to bind memory to a resource.
		 */
		unsigned int DeviceMemoryBlockSize = 1024 * 1024 * 64;

		/**
		 * \brief Flag enabling the activation of a validation layer
		 * Even if this flag is set to true, if the system has no validation layer installed,
		 * ths application will still run without it.
		 */
		bool EnableValidation = false;
	};
}

#endif // COCKTAIL_VULKAN_RENDERDEVICECREATEINFO_HPP
