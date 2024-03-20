#ifndef COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP
#define COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP

#include <Cocktail/Core/Utility/CompositeKey.hpp>

#include <Cocktail/Vulkan/Semaphore.hpp>
#include <Cocktail/Vulkan/Command/CommandList.hpp>
#include <Cocktail/Vulkan/Command/Fence.hpp>
#include <Cocktail/Vulkan/Context/FrameContextCreateInfo.hpp>
#include <Cocktail/Vulkan/Context/RenderSurface.hpp>
#include <Cocktail/Vulkan/Framebuffer/Framebuffer.hpp>

namespace Ck::Vulkan
{
	class RenderContext;

	class BufferPool : public Extends<BufferPool, Buffer>
	{
	public:

		/**
		 * \brief
		 * \param renderDevice
		 * \param createInfo
		 * \param allocationCallbacks
		 */
		BufferPool(Ref<RenderDevice> renderDevice, const Renderer::BufferCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief
		 * \param alignment
		 * \param length
		 * \param data
		 * \return
		 */
		std::size_t PushData(std::size_t alignment, std::size_t length, const void* data);

		/**
		 * \brief
		 * \param alignment
		 * \return
		 */
		std::size_t ComputePadding(std::size_t alignment) const;

		/**
		 * \brief
		 */
		void Reset();

		/**
		 * \brief
		 * \return
		 */
		std::size_t GetRemainingCapacity() const;

	private:

		std::size_t mRemainingCapacity;
	};

	class BufferAllocator : public Inherit<BufferAllocator, Object, Renderer::BufferAllocator>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param usage 
		 * \param bufferSize 
		 * \param memoryType 
		 */
		BufferAllocator(Ref<RenderDevice> renderDevice, Renderer::BufferUsageFlags usage, std::size_t bufferSize, Renderer::MemoryType memoryType);

		/**
		 * \brief 
		 * \param size 
		 * \param data 
		 * \return 
		 */
		Renderer::BufferArea PushData(std::size_t size, const void* data) override;

		/**
		 * \brief 
		 * \param size 
		 */
		void Reserve(std::size_t size) override;

		/**
		 * \brief
		 * \param release
		 */
		void Reset(bool release = false);

		/**
		 * \brief
		 * \return
		 */
		std::size_t GetBufferSize() const;

	private:

		/**
		 * \brief 
		 * \param size 
		 * \return 
		 */
		Ref<BufferPool> AcquirePool(std::size_t size);

		Ref<RenderDevice> mRenderDevice;
		Renderer::BufferUsageFlags mUsage;
		std::size_t mBufferSize;
		Renderer::MemoryType mMemoryType;
		std::size_t mMinAlignment;
		std::vector<Ref<BufferPool>> mAcquiredBufferPools;
		std::vector<Ref<BufferPool>> mAvailableBufferPools;
		TlsObjectPool<BufferPool> mBufferPool;
	};

	class FrameContext : public Inherit<FrameContext, Object, Renderer::FrameContext>
	{
	public:

		/**
		 * \brief
		 * \param renderDevice
		 * \param createInfo
		 */
		FrameContext(Ref<RenderDevice> renderDevice, const FrameContextCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param name 
		 */
		void SetObjectName(const char* name) const override;

		/**
		 * \brief 
		 * \return 
		 */
		Ref<Renderer::RenderDevice> GetRenderDevice() const override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<Renderer::CommandList> CreateCommandList(const Renderer::CommandListCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param usage 
		 * \param memoryType 
		 * \return 
		 */
		Renderer::BufferAllocator* GetBufferAllocator(Renderer::BufferUsageFlags usage, Renderer::MemoryType memoryType) override;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::FrameToken GetToken() const override;

		/**
		 * \brief 
		 */
		void Synchronize() const;

		/**
		 * \brief 
		 * \param renderSurface
		 * \return 
		 */
		Ref<Framebuffer> AcquireNextFramebuffer(const Ref<RenderSurface>& renderSurface);

		/**
		 * \brief 
		 * \param renderContext 
		 * \param commandQueue 
		 */
		void WaitQueue(RenderContext& renderContext, Renderer::CommandQueueType commandQueue) const;

		/**
		 * \brief Clear any remaining resources from a previous frame
		 */
		void Reset() const;

		/**
		 * \brief 
		 * \param renderSurface 
		 * \param renderContext 
		 */
		void Present(const Ref<RenderSurface>& renderSurface, RenderContext* renderContext);

	private:

		using BufferAllocatorKey = CompositeKey<Renderer::BufferUsageFlags, Renderer::MemoryType>;

		Ref<RenderDevice> mRenderDevice;
		Optional<unsigned int> mCurrentFramebufferIndex;
		Ref<CommandListPool> mCommandListPool;
		std::vector<Ref<Renderer::CommandList>> mCommandLists;
		std::unordered_map<BufferAllocatorKey, Ref<BufferAllocator>> mBufferAllocators;
		Ref<Semaphore> mImageAvailable;
		Ref<Semaphore> mImagePresentable;
		Ref<Fence> mFrameFence;
	};
}

#endif // COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP
