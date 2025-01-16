#ifndef COCKTAIL_RENDERER_COMMAND_ALLOCATOR_COMMANDLISTPOOLALLOCATOR_HPP
#define COCKTAIL_RENDERER_COMMAND_ALLOCATOR_COMMANDLISTPOOLALLOCATOR_HPP

#include <Cocktail/Core/Flags.hpp>

#include <Cocktail/Renderer/Command/CommandListUsage.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief 
	 */
	struct CommandListPoolCreateInfo
	{
		/**
		 * \brief 
		 */
		Flags<CommandListUsage> SupportedUsage = Flags<CommandListUsage>::All();

		/**
		 * \brief 
		 */
		bool Transient = true;

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

#endif // COCKTAIL_RENDERER_COMMAND_ALLOCATOR_COMMANDLISTPOOLALLOCATOR_HPP
