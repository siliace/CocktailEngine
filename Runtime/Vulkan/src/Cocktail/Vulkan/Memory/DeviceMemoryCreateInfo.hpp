#ifndef COCKTAIL_VULKAN_MEMORY_DEVICEMEMORYCREATEINFO_HPP
#define COCKTAIL_VULKAN_MEMORY_DEVICEMEMORYCREATEINFO_HPP

#include <Cocktail/Vulkan/Buffer/Buffer.hpp>
#include <Cocktail/Vulkan/Texture/AbstractTexture.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	struct DeviceMemoryCreateInfo
	{
		/**
		 * \brief 
		 */
		std::size_t Size = 0;

		/**
		 * \brief 
		 */
		unsigned int MemoryTypeIndex = 0;

		/**
		 * \brief 
		 */
		bool Dedicated = false;

		/**
		 * \brief 
		 */
		const AbstractTexture* Texture;

		/**
		 * \brief 
		 */
		const Buffer* Buffer;
	};
}

#endif // COCKTAIL_VULKAN_MEMORY_DEVICEMEMORYCREATEINFO_HPP
