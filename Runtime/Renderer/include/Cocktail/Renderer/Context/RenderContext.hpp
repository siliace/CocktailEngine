#ifndef COCKTAIL_RENDERER_CONTEXT_RENDERCONTEXT_HPP
#define COCKTAIL_RENDERER_CONTEXT_RENDERCONTEXT_HPP

#include <Cocktail/Renderer/Command/CommandQueueType.hpp>
#include <Cocktail/Renderer/Command/Allocator/CommandListPoolCreateInfo.hpp>

namespace Ck::Renderer
{
	class CommandList;
	class CommandListPool;
	class Fence;
	class FrameContext;

	/**
	 * \brief 
	 */
	class RenderContext : public Implements<RenderContext, RenderDeviceObject>
	{
	public:
			
		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		virtual Ref<CommandListPool> CreateCommandListPool(const CommandListPoolCreateInfo& createInfo) = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual FrameContext* BeginFrame() = 0;

		/**
		 * \brief 
		 */
		virtual void SignalQueue(CommandQueueType queue) = 0;

		/**
		 * \brief 
		 * \param waitingQueue 
		 * \param waitedQueue 
		 */
		virtual void WaitQueue(CommandQueueType waitingQueue, CommandQueueType waitedQueue) = 0;

		/**
		 * \brief Execute command lists
		 * \param queue The queue where command lists should be executed
		 * \param commandListCount The number of command list to execute
		 * \param commandLists Command lists to execute
		 * \param fence A fence to signal when every command list as been completed
		 */
		virtual void ExecuteCommandLists(CommandQueueType queue, unsigned int commandListCount, Ref<CommandList>* commandLists, Ref<Fence> fence) = 0;

		/**
		 * \brief 
		 */
		virtual void EndFrame() = 0;

		/**
		 * \brief 
		 */
		virtual void Flush() = 0;

		/**
		 * \brief Wait for all pending operations to be terminated
		 */
		virtual void Synchronize() = 0;
	};
}

#endif // COCKTAIL_RENDERER_CONTEXT_RENDERCONTEXT_HPP
