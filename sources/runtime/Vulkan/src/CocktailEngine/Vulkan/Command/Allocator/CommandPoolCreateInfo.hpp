#ifndef COCKTAILENGINE_VULKAN_COMMANDLIST_ALLOCATOR_COMMANDPOOLCREATEINFO_HPP
#define COCKTAILENGINE_VULKAN_COMMANDLIST_ALLOCATOR_COMMANDPOOLCREATEINFO_HPP

#include <CocktailEngine/Renderer/Command/CommandQueueType.hpp>

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

#endif // COCKTAILENGINE_VULKAN_COMMANDLIST_ALLOCATOR_COMMANDPOOLCREATEINFO_HPP
