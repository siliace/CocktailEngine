#ifndef COCKTAILENGINE_VULKAN_FRAMEBUFFER_RENDERPASSBUILDER_HPP
#define COCKTAILENGINE_VULKAN_FRAMEBUFFER_RENDERPASSBUILDER_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Renderer/PixelFormat.hpp>

#include <CocktailEngine/Renderer/RasterizationSamples.hpp>

#include <CocktailEngine/Vulkan/Volk.hpp>

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
		Array<VkAttachmentDescription> mAttachmentDescriptions;
		Array<VkAttachmentReference> mColorAttachmentReference;
		VkAttachmentReference mDepthStencilAttachmentReference;
		Array<VkAttachmentReference> mResolveAttachmentReference;
		Array<VkSubpassDescription> mSubpassDescriptions;
		Array<VkSubpassDependency> mSubpassDependencies;
	};
}
#endif // COCKTAILENGINE_VULKAN_FRAMEBUFFER_RENDERPASSBUILDER_HPP
