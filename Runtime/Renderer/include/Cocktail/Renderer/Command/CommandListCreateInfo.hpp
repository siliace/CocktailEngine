#ifndef COCKTAIL_RENDERER_COMMNAD_COMMANDLISTCREATEINFO_HPP
#define COCKTAIL_RENDERER_COMMNAD_COMMANDLISTCREATEINFO_HPP

#include <Cocktail/Renderer/Command/CommandListUsage.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief CommandList descriptor structure
	 * Describe the set of parameters to create a CommandList
	 * \see RenderDevice::CreateCommandList
	 */
	struct CommandListCreateInfo
	{
		/**
		 * \brief The usage of the CommandList to create
		 */
		CommandListUsageBits Usage = CommandListUsageBits::Graphic;

		/**
		 * \brief Specifies whether the CommandList to create will be submitted as a secondary CommandList
         * If this parameter is true, the CommandList must be submitted using the \c Execute function of a primary command buffer.
         * \see CommandBuffer::Execute
		 */
		bool Secondary = false;

		/**
		 * \brief Specifies the name of the Fence to create
		 * If the extension Debug is not supported by the RenderDevice creating the CommandList, this parameter is ignored
		 * \see RenderDeviceObject::GetName
		 */
		const char* Name = nullptr;
	};
}

#endif // COCKTAIL_RENDERER_COMMNAD_COMMANDLISTCREATEINFO_HPP
