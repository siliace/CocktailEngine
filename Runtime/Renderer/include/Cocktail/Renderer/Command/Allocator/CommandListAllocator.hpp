#ifndef COCKTAIL_RENDERER_COMMAND_ALLOCATOR_COMMANDLISTALLOCATOR_HPP
#define COCKTAIL_RENDERER_COMMAND_ALLOCATOR_COMMANDLISTALLOCATOR_HPP

#include <Cocktail/Renderer/Command/CommandList.hpp>
#include <Cocktail/Renderer/Command/CommandListCreateInfo.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief Interface shared by classes capable of allocating command lists
	 */
	class CommandListAllocator : public Implements<CommandListAllocator, RenderDeviceObject>
	{
	public:

		/**
		 * \brief Allocate a new command list
		 * \param createInfo 
		 * \return 
		 */
		virtual Ref<CommandList> CreateCommandList(const CommandListCreateInfo& createInfo) = 0;
	};
}

#endif // COCKTAIL_RENDERER_COMMAND_ALLOCATOR_COMMANDLISTALLOCATOR_HPP
