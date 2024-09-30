#ifndef COCKTAIL_VULKAN_COMMANDLIST_ALLOCATOR_COMMANDPOOLCREATEINFO_HPP
#define COCKTAIL_VULKAN_COMMANDLIST_ALLOCATOR_COMMANDPOOLCREATEINFO_HPP

#include <Cocktail/Renderer/Command/CommandQueueType.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief
	 */
	struct CommandPoolCreateInfo
	{
		/**
		 * \brief
		 */
		Renderer::CommandQueueType QueueType = Renderer::CommandQueueType::Graphic;

		/**
		 * \brief
		 */
		bool Transient = false;

		/**
		 * \brief
		 */
		bool Reset = false;

		/**
		 * \brief
		 */
		const char* Name = nullptr;
	};
}

#endif // COCKTAIL_VULKAN_COMMANDLIST_ALLOCATOR_COMMANDPOOLCREATEINFO_HPP
