#ifndef COCKTAIL_RENDERER_COMMAND_COMMANDLISTDYNAMICSTATE_HPP
#define COCKTAIL_RENDERER_COMMAND_COMMANDLISTDYNAMICSTATE_HPP

#include <Cocktail/Core/Flags.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief Enumeration of possible dynamic states in a CommandList
	 * These flags might not be used by some implementations.
	 */
	enum class CommandListDynamicStateBits
	{
		/**
		 * \brief Specifies the viewport state is dynamic
		 */
		Viewport = Bit(0),

		/**
		 * \brief Specifies the scissor state is dynamic
		 */
		Scissor = Bit(1),

		/**
		 * \brief Specifies the line width state is dynamic
		 */
		LineWidth = Bit(2),
	};

	using CommandListDynamicState = Flags<CommandListDynamicStateBits>;
}

#endif // COCKTAIL_RENDERER_COMMAND_COMMANDLISTDYNAMICSTATE_HPP
