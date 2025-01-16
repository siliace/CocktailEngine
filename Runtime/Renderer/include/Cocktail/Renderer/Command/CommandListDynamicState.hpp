#ifndef COCKTAIL_RENDERER_COMMAND_COMMANDLISTDYNAMICSTATE_HPP
#define COCKTAIL_RENDERER_COMMAND_COMMANDLISTDYNAMICSTATE_HPP

#include <Cocktail/Core/Flags.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief 
	 */
	enum class CommandListDynamicStateBits
	{
		/**
		 * \brief 
		 */
		Viewport = Bit(0),

		/**
		 * \brief 
		 */
		Scissor = Bit(1),

		/**
		 * \brief 
		 */
		LineWidth = Bit(2),
	};

	using CommandListDynamicState = Flags<CommandListDynamicStateBits>;
}

#endif // COCKTAIL_RENDERER_COMMAND_COMMANDLISTDYNAMICSTATE_HPP
