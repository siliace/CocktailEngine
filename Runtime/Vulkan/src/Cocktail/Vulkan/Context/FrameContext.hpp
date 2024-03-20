#ifndef COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP
#define COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP

#include <Cocktail/Vulkan/Semaphore.hpp>
#include <Cocktail/Vulkan/Command/CommandList.hpp>
#include <Cocktail/Vulkan/Command/Fence.hpp>
#include <Cocktail/Vulkan/Context/FrameContextCreateInfo.hpp>
#include <Cocktail/Vulkan/Context/RenderSurface.hpp>
#include <Cocktail/Vulkan/Framebuffer/Framebuffer.hpp>

namespace Ck::Vulkan
{
	class RenderContext;
	
	class FrameContext : public Inherit<FrameContext, Object, Renderer::FrameContext>
	{
	public:

		/**
		 * \brief
		 * \param renderDevice
		 * \param createInfo
		 */
		FrameContext(const Ref<RenderDevice>& renderDevice, const FrameContextCreateInfo& createInfo);

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

		Ref<RenderDevice> mRenderDevice;
		Optional<unsigned int> mCurrentFramebufferIndex;
		Ref<CommandListPool> mCommandListPool;
		std::vector<Ref<Renderer::CommandList>> mCommandLists;
		Ref<Semaphore> mImageAvailable;
		Ref<Semaphore> mImagePresentable;
		Ref<Fence> mFrameFence;
	};
}

#endif // COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP
