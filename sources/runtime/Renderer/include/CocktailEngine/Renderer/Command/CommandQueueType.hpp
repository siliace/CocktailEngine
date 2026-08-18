#ifndef COCKTAILENGINE_RENDERER_COMMAND_COMMANDQUEUE_HPP
#define COCKTAILENGINE_RENDERER_COMMAND_COMMANDQUEUE_HPP

#include <CocktailEngine/Core/Flags.hpp>

#include <CocktailEngine/Renderer/Export.hpp>
#include <CocktailEngine/Renderer/Command/CommandListUsage.hpp>

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
	COCKTAILENGINE_RENDERER_API CommandQueueType SelectQueueForUsage(CommandListUsage usage);
}

#endif // COCKTAILENGINE_RENDERER_COMMAND_COMMANDQUEUE_HPP
