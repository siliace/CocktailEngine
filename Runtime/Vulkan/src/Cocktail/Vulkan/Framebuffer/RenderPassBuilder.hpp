#ifndef COCKTAIL_VULKAN_FRAMEBUFFER_RENDERPASSBUILDER_HPP
#define COCKTAIL_VULKAN_FRAMEBUFFER_RENDERPASSBUILDER_HPP

#include <Cocktail/Core/Image/PixelFormat.hpp>

#include <Cocktail/Renderer/RasterizationSamples.hpp>

#include <Cocktail/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief Helper class to ease create of VkRenderPassCreateInfo struct
	 */
	class RenderPassBuilder
	{
	public:

		/**
		 * \brief Constructor
		 * Create a new RenderPassBuilder
		 * \param samples The number of samples used by attachments to resolve
		 */
		explicit RenderPassBuilder(Renderer::RasterizationSamples samples);

		/**
		 * \brief Register a new attachment in the the render pass
		 * \param format 
		 * \param loadOp 
		 * \param storeOp 
		 * \param presentable 
		 */
		void CreateAttachment(const PixelFormat& format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, bool presentable);

		/**
		 * \brief 
		 * \param bindPoint 
		 */
		void CreateSubpass(VkPipelineBindPoint bindPoint);

		/**
		 * \brief 
		 */
		void CreateDependency();

		/**
		 * \brief 
		 * \return 
		 */
		VkRenderPassCreateInfo ToCreateInfo() const;

	private:

		/**
		 * \brief 
		 * \param format 
		 * \param samples 
		 * \param loadOp 
		 * \param storeOp 
		 * \param presentable 
		 * \return 
		 */
		static VkAttachmentDescription CreateAttachmentDescription(const PixelFormat& format, Renderer::RasterizationSamples samples, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, bool presentable);

		Renderer::RasterizationSamples mSamples;
		std::vector<VkAttachmentDescription> mAttachmentDescriptions;
		std::vector<VkAttachmentReference> mColorAttachmentReference;
		VkAttachmentReference mDepthStencilAttachmentReference;
		std::vector<VkAttachmentReference> mResolveAttachmentReference;
		std::vector<VkSubpassDescription> mSubpassDescriptions;
		std::vector<VkSubpassDependency> mSubpassDependencies;
	};
}
#endif // COCKTAIL_VULKAN_FRAMEBUFFER_RENDERPASSBUILDER_HPP
