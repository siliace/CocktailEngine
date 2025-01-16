#ifndef COCKTAIL_RENDERER_COMMAND_COMMANDQUEUE_HPP
#define COCKTAIL_RENDERER_COMMAND_COMMANDQUEUE_HPP

#include <Cocktail/Core/Flags.hpp>

#include <Cocktail/Renderer/Export.hpp>
#include <Cocktail/Renderer/Command/CommandListUsage.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief 
	 */
	enum class CommandQueueType
	{
		/**
		 * \brief 
		 */
		Graphic = Bit(0),

		/**
		 * \brief 
		 */
		Transfer = Bit(1),

		/**
		 * \brief 
		 */
		Compute = Bit(2),
	};

	/**
	 * \brief Help function to select the right command queue for a given command list usage
	 * \param usage 
	 * \return 
	 */
	COCKTAIL_RENDERER_API CommandQueueType SelectQueueForUsage(const Flags<CommandListUsage>& usage);
}

#endif // COCKTAIL_RENDERER_COMMAND_COMMANDQUEUE_HPP
