#ifndef COCKTAIL_VULKAN_SEMAPHORECREATEINFO_HPP
#define COCKTAIL_VULKAN_SEMAPHORECREATEINFO_HPP

#include <Cocktail/Core/Cocktail.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief
	 */
	struct SemaphoreCreateInfo
	{
		/**
		 * \brief
		 */
		bool Binary = true;

		/**
		 * \brief
		 */
		Uint64 InitialValue = 0;

		/**
		 * \brief
		 */
		const char* Name = nullptr;
	};
}

#endif // COCKTAIL_VULKAN_SEMAPHORECREATEINFO_HPP
