#ifndef COCKTAIL_RENDERER_COMMAND_COMMANDLISTUSAGE_HPP
#define COCKTAIL_RENDERER_COMMAND_COMMANDLISTUSAGE_HPP

#include <Cocktail/Core/Cocktail.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief 
	 */
	enum class CommandListUsage
	{
		/**
		 * \brief 
		 */
		Graphic = Bit(0),

		/**
		 * \brief 
		 */
		Compute = Bit(1),

		/**
		 * \brief 
		 */
		Transfer = Bit(2),
	};
}

#endif // COCKTAIL_RENDERER_COMMAND_COMMANDLISTUSAGE_HPP
