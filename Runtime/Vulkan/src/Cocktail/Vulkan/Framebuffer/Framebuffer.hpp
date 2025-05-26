#ifndef COCKTAIL_VULKAN_FRAMEBUFFER_FRAMEBUFFER_HPP
#define COCKTAIL_VULKAN_FRAMEBUFFER_FRAMEBUFFER_HPP

#include <Cocktail/Renderer/Framebuffer/Framebuffer.hpp>
#include <Cocktail/Renderer/Framebuffer/FramebufferCreateInfo.hpp>

#include <Cocktail/Vulkan/Framebuffer/AttachmentBuffer.hpp>
#include <Cocktail/Vulkan/Texture/TextureView.hpp>

namespace Ck::Vulkan
{
	class RenderPass;

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
		Framebuffer(std::shared_ptr<RenderDevice> renderDevice, std::shared_ptr<RenderPass> renderPass, const Renderer::FramebufferCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		std::shared_ptr<Renderer::RenderDevice> GetRenderDevice() const override;

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
		std::shared_ptr<Renderer::TextureView> GetColorMultisampleAttachment(unsigned index) const;

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		std::shared_ptr<Renderer::TextureView> GetColorAttachment(unsigned index) const override;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetColorAttachmentCount() const override;

		/**
		 * \brief
		 * \return
		 */
		std::shared_ptr<Renderer::TextureView> GetDepthStencilMultisampleAttachment() const;

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<Renderer::TextureView> GetDepthStencilAttachment() const override;

		/**
		 * \brief
		 * \return 
		 */
		std::shared_ptr<RenderPass> GetRenderPass() const;

		/**
		 * \brief 
		 * \return 
		 */
		VkFramebuffer GetHandle() const;

	private:

		std::shared_ptr<RenderDevice> mRenderDevice;
		std::shared_ptr<RenderPass> mRenderPass;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkFramebuffer mHandle;
		Extent3D<unsigned int> mSize;
		unsigned int mColorBufferCount;
		std::unique_ptr<AttachmentBuffer> mColorBuffers[MaxColorAttachmentCount];
		std::unique_ptr<AttachmentBuffer> mDepthStencilBuffer;
	};
}

#endif // COCKTAIL_VULKAN_FRAMEBUFFER_FRAMEBUFFER_HPP
