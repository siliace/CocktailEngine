#ifndef COCKTAIL_RENDERER_CONTEXT_RENDERCONTEXT_HPP
#define COCKTAIL_RENDERER_CONTEXT_RENDERCONTEXT_HPP

#include <Cocktail/Renderer/RenderDeviceObject.hpp>
#include <Cocktail/Renderer/Buffer/BufferUsage.hpp>
#include <Cocktail/Renderer/Command/CommandListCreateInfo.hpp>
#include <Cocktail/Renderer/Command/CommandQueueType.hpp>
#include <Cocktail/Renderer/Memory/MemoryType.hpp>

namespace Ck::Renderer
{
	class BufferAllocator;
	class CommandList;
	class Fence;
	class Framebuffer;
	class RenderSurface;

	/**
	 * \brief Interface to create and submit CommandList to the GPU
	 * This class is not thread safe and should be thread local
	 */
	class RenderContext : public RenderDeviceObject
	{
	public:

		/**
		 * \brief
		 * \param usage
		 * \param memoryType
		 * \return
		 */
		virtual BufferAllocator* GetBufferAllocator(BufferUsageFlags usage, MemoryType memoryType) = 0;

		/**
		 * \brief Acquire the framebuffer to render from a RenderSurface
		 * The returned framebuffer might change depending on the implementation and should not be kept
		 * This function can return nullptr if there is no framebuffer available (i.e. for a RenderSurface using a minimized Window)
		 * \param renderSurface The RenderSurface to acquire
		 * \return The acquired framebuffer
		 */
		virtual Framebuffer* AcquireFramebuffer(RenderSurface* renderSurface) = 0;

		/**
		 * \brief Allocate a new CommandList
		 * Implementations might reuse CommandList created in previous frames so keeping track CommandList is not a valid usage
         * \param createInfo Parameters to use to create the CommandList
		 * \return The created CommandList
		 */
		virtual CommandList* CreateCommandList(const CommandListCreateInfo& createInfo) = 0;

		/**
		 * \brief Signal a queue
		 * A signaled queue indicates this queue can be waited to synchronize resource usage
		 * \param queue The queue to signal
		 * \see WaitQueue
		 */
		virtual void SignalQueue(CommandQueueType queue) = 0;

		/**
		 * \brief Wait for a queue
		 * Waiting a queue will stop every operation of this queue until another one reach the latest signal point.
		 * if \p waitedQueue has never been signaled, \p waitingQueue won't wait.
		 * \param waitingQueue The queue to make wait
		 * \param waitedQueue The queue \p waitingQueue should wait
		 * \see SignalQueue
		 */
		virtual void WaitQueue(CommandQueueType waitingQueue, CommandQueueType waitedQueue) = 0;

		/**
		 * \brief Schedule execution of command lists
		 * \param queue The queue where command lists should be executed
		 * \param commandListCount The number of command list to execute
		 * \param commandLists Command lists to execute
		 * \param fence A fence to signal when every command list as been completed
		 */
		virtual void SubmitCommandLists(CommandQueueType queue, unsigned int commandListCount, CommandList** commandLists, Fence* fence) = 0;

		/**
		 * \brief Flush the execution of every CommandList submitted so far
		 * If any RenderSurface has been acquired using AcquireNextFramebuffer, it will be presented
		 */
		virtual void Flush() = 0;

		/**
		 * \brief Wait for all pending operations to be terminated
		 */
		virtual void Synchronize() = 0;
	};
}

#endif // COCKTAIL_RENDERER_CONTEXT_RENDERCONTEXT_HPP
