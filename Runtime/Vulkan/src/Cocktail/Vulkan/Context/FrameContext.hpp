#ifndef COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP
#define COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP

#include <Cocktail/Core/Utility/CompositeKey.hpp>
#include <Cocktail/Core/Utility/FixedArray.hpp>

#include <Cocktail/Renderer/Buffer/BufferAllocator.hpp>

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
			std::shared_ptr<Semaphore> ImageAvailable;
			std::shared_ptr<Semaphore> ImagePresentable;
		};

		RenderContext* mRenderContext;
		std::shared_ptr<CommandListPool> mCommandListPool;
		std::vector<std::shared_ptr<CommandList>> mCommandLists;
		std::shared_ptr<Fence> mFrameFence;
		std::unordered_map<const RenderSurface*, AcquiredImage> mAcquiredImages;
		std::unordered_map<BufferAllocatorKey, std::shared_ptr<BufferAllocator>> mBufferAllocators;
		bool mSubmitted;
	};
}

#endif // COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP
