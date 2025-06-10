#ifndef COCKTAIL_RENDERER_COMMAND_COMMANDLISTUSAGE_HPP
#define COCKTAIL_RENDERER_COMMAND_COMMANDLISTUSAGE_HPP

#include <Cocktail/Core/Cocktail.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief Enumeration of possible usages of a CommandList
	 */
	enum class CommandListUsageBits
	{
		/**
		 * \brief Specifies the CommandList can issue graphic operations
		 * If a CommandList is created with this usage, it will implicitly support transfer and compute usage
		 * \see CommandList::Draw
		 * \see CommandList::DrawIndexed
		 */
		Graphic = Bit(0),

		/**
		 * \brief Specifies the CommandList can issue compute operations
		 * If a CommandList is created with this usage, it will implicitly support transfer usage
		 * \see CommandList::Dispatch
		 */
		Compute = Bit(1),

		/**
		 * \brief Specifies the CommandList can issue transfer operations
		 */
		Transfer = Bit(2),
	};

	using CommandListUsage = Flags<CommandListUsageBits>;
}

#endif // COCKTAIL_RENDERER_COMMAND_COMMANDLISTUSAGE_HPP
