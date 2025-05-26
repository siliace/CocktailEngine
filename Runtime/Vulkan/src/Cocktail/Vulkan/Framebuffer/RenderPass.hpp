#ifndef COCKTAIL_VULKAN_FRAMEBUFFER_RENDERPASS_HPP
#define COCKTAIL_VULKAN_FRAMEBUFFER_RENDERPASS_HPP

#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Renderer/Command/RenderPassBeginInfo.hpp>

#include <Cocktail/Vulkan/Volk.hpp>
#include <Cocktail/Vulkan/Framebuffer/RenderPassCreateInfo.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class RenderPass : public Renderer::RenderDeviceObject
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		RenderPass(std::shared_ptr<RenderDevice> renderDevice, const RenderPassCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~RenderPass() override;

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
		bool ResolveDepthStencil() const;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetColorAttachmentCount() const;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::RasterizationSamples GetSamples() const;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::ResolveMode GetDepthResolveMode() const;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::ResolveMode GetStencilResolveMode() const;

		/**
		 * \brief 
		 * \return 
		 */
		VkRenderPass GetHandle(Renderer::RenderPassMode mode) const;

	private:
		
		std::shared_ptr<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		EnumMap<Renderer::RenderPassMode, VkRenderPass> mHandle;
		unsigned int mColorAttachmentCount;
		Renderer::RasterizationSamples mSamples;
		bool mResolveDepthStencil;
		Renderer::ResolveMode mDepthResolveMode;
		Renderer::ResolveMode mStencilResolveMode;
	};
}

#endif // COCKTAIL_VULKAN_FRAMEBUFFER_RENDERPASS_HPP
