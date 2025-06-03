#ifndef COCKTAIL_RENDERER_CONTEXT_RENDERCONTEXT_HPP
#define COCKTAIL_RENDERER_CONTEXT_RENDERCONTEXT_HPP

#include <Cocktail/Renderer/RenderDeviceObject.hpp>
#include <Cocktail/Renderer/Command/CommandQueueType.hpp>
#include <Cocktail/Renderer/Command/CommandListCreateInfo.hpp>

namespace Ck::Renderer
{
	class CommandList;
	class Fence;
	class Framebuffer;
	class RenderSurface;

	/**
	 * \brief 
	 */
	class RenderContext : public RenderDeviceObject
	{
	public:

		/**
		 * \brief 
		 * \param renderSurface 
		 * \return 
		 */
		virtual Framebuffer* AcquireNextFramebuffer(RenderSurface* renderSurface) = 0;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		virtual CommandList* CreateCommandList(const CommandListCreateInfo& createInfo) = 0;

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
		virtual void ExecuteCommandLists(CommandQueueType queue, unsigned int commandListCount, CommandList** commandLists, Fence* fence) = 0;

		/**
		 * \brief 
		 */
		virtual void Flush() = 0;
	};
}

#endif // COCKTAIL_RENDERER_CONTEXT_RENDERCONTEXT_HPP
