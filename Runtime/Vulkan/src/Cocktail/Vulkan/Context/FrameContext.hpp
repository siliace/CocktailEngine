#ifndef COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP
#define COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Utility/CompositeKey.hpp>

namespace Ck::Vulkan
{
	class BufferAllocator;
	class CommandList;
	class Framebuffer;
	class RenderContext;

	class FrameContext
	{
	public:

		/**
		 * \brief
		 * \param renderContext
		 * \param allocationCallbacks
		 */
		FrameContext(RenderContext* renderContext, const VkAllocationCallbacks* allocationCallbacks);

        ~FrameContext();

		/**
		 * \brief
		 */
		void Synchronize() const;

		/**
		 * \brief Clear any remaining resources from a previous frame
		 */
		void Reset();

		/**
		 * \brief
		 * \param renderSurface
		 * \return
		 */
		Framebuffer* AcquireNextFramebuffer(const RenderSurface* renderSurface);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		CommandList* CreateCommandList(const Renderer::CommandListCreateInfo& createInfo);

		/**
		 * \brief
		 * \param usage
		 * \param memoryType
		 * \return
		 */
		Renderer::BufferAllocator* GetBufferAllocator(Renderer::BufferUsageFlags usage, Renderer::MemoryType memoryType);

		/**
		 * \brief
		 * \param queue
		 */
		void Present(VkQueue queue);

	private:

		using BufferAllocatorKey = CompositeKey<Renderer::BufferUsageFlags, Renderer::MemoryType>;

		struct AcquiredImage
		{
			Optional<unsigned int> ImageIndex;
			SharedPtr<Semaphore> ImageAvailable;
			SharedPtr<Semaphore> ImagePresentable;
		};

		RenderContext* mRenderContext;
		SharedPtr<CommandListPool> mCommandListPool;
		Array<SharedPtr<CommandList>> mCommandLists;
		SharedPtr<Fence> mFrameFence;
		HashMap<const RenderSurface*, AcquiredImage> mAcquiredImages;
		HashMap<BufferAllocatorKey, SharedPtr<BufferAllocator>> mBufferAllocators;
		bool mSubmitted;
	};
}

#endif // COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP
