#ifndef COCKTAIL_RENDERER_COMMAND_COMMANDLISTSTATE_HPP
#define COCKTAIL_RENDERER_COMMAND_COMMANDLISTSTATE_HPP

namespace Ck::Renderer
{
	/**
	 * \brief Enumeration of possible states of a CommandList
	 */
	enum class CommandListState
	{
		/**
		 * \brief The CommandList is in its inital state
		 * A CommandList will be in Initial state just after being created of resetted
		 * \see CommandList::Begin
		 * \see CommandList::End
		 */
		Initial,

		/**
		 * \brief The CommandList is recording commands
		 * A CommandList will be in Recording state after CommandList::Begin has been called
		 * \see CommandList::Begin
		 * \see CommandList::End
		 */
		Recording,

		/**
		 * \brief The CommandList is recording commands within a render pass
		 * A CommandList will be in RecordingRenderPass state after CommandList::BeginRenderPass has been called
		 * \see CommandList::BeginRenderPass
		 * \see CommandList::EndRenderPass
		 */
		RecordingRenderPass,

		/**
		 * \brief The CommandList is not recording commands anymore
		 * A CommandList will be in Recording state after CommandList::End has been called
		 * \see CommandList::Begin
		 * \see CommandList::End
		 */
		Executable,

		/**
		 * \brief The CommandList has been submitted in a CommandQueue
		 * \see CommandQueue::Submit 
		 */
		Pending,

		/**
		 * \brief The CommandList became invalid
		 */
		Invalid,
	};
}

#endif // COCKTAIL_RENDERER_COMMAND_COMMANDLISTSTATE_HPP
