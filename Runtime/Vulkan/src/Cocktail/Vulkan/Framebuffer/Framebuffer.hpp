#ifndef COCKTAIL_VULKAN_FRAMEBUFFER_FRAMEBUFFER_HPP
#define COCKTAIL_VULKAN_FRAMEBUFFER_FRAMEBUFFER_HPP

#include <Cocktail/Renderer/Framebuffer/Framebuffer.hpp>
#include <Cocktail/Renderer/Framebuffer/FramebufferCreateInfo.hpp>

#include <Cocktail/Vulkan/Framebuffer/AttachmentBuffer.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	class Framebuffer : public Renderer::Framebuffer
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param renderPass 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		Framebuffer(RenderDevice* renderDevice, SharedPtr<RenderPass> renderPass, const Renderer::FramebufferCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~Framebuffer() override;

		/**
		 * \brief 
		 * \param name 
		 */
		void SetObjectName(const char* name) const override;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::RenderDevice* GetRenderDevice() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Extent3D<unsigned int> GetSize() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::RasterizationSamples GetSamples() const override;

		/**
		 * \brief
		 * \param index
		 * \return
		 */
		SharedPtr<Renderer::TextureView> GetColorMultisampleAttachment(unsigned index) const;

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		SharedPtr<Renderer::TextureView> GetColorAttachment(unsigned index) const override;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetColorAttachmentCount() const override;

		/**
		 * \brief
		 * \return
		 */
		SharedPtr<Renderer::TextureView> GetDepthStencilMultisampleAttachment() const;

		/**
		 * \brief 
		 * \return 
		 */
		SharedPtr<Renderer::TextureView> GetDepthStencilAttachment() const override;

		/**
		 * \brief
		 * \return 
		 */
		SharedPtr<RenderPass> GetRenderPass() const;

		/**
		 * \brief 
		 * \return 
		 */
		VkFramebuffer GetHandle() const;

	private:

		RenderDevice* mRenderDevice;
		SharedPtr<RenderPass> mRenderPass;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkFramebuffer mHandle;
		Extent3D<unsigned int> mSize;
		unsigned int mColorBufferCount;
		UniquePtr<AttachmentBuffer> mColorBuffers[MaxColorAttachmentCount];
		UniquePtr<AttachmentBuffer> mDepthStencilBuffer;
	};
}

#endif // COCKTAIL_VULKAN_FRAMEBUFFER_FRAMEBUFFER_HPP
