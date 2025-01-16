#ifndef COCKTAIL_RENDERER_COMMAND_ALLOCATOR_COMMANDLISTPOOL_HPP
#define COCKTAIL_RENDERER_COMMAND_ALLOCATOR_COMMANDLISTPOOL_HPP

#include <Cocktail/Renderer/Command/Allocator/CommandListAllocator.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief 
	 */
	class CommandListPool : public Implements<CommandListPool, CommandListAllocator>
	{
	public:

		/**
		 * \brief 
		 * \param releaseResources 
		 */
		virtual void Reset(bool releaseResources) = 0;
	};
}

#endif // COCKTAIL_RENDERER_COMMAND_ALLOCATOR_COMMANDLISTPOOL_HPP
