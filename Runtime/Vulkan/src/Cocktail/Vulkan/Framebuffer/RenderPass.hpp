#ifndef COCKTAIL_VULKAN_FRAMEBUFFER_RENDERPASS_HPP
#define COCKTAIL_VULKAN_FRAMEBUFFER_RENDERPASS_HPP

#include <volk.h>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Renderer/Command/RenderPassBeginInfo.hpp>

#include <Cocktail/Vulkan/Framebuffer/RenderPassCreateInfo.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class RenderPass : public Inherit<RenderPass, Object, Renderer::RenderDeviceObject>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		RenderPass(const Ref<RenderDevice>& renderDevice, const RenderPassCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		Ref<Renderer::RenderDevice> GetRenderDevice() const override;

		/**
		 * \brief 
		 * \return 
		 */
		bool SupportDepthStencilResolve() const;

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
		VkRenderPass GetHandle(Renderer::RenderPassMode mode) const;

	private:
		
		Ref<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		EnumMap<Renderer::RenderPassMode, VkRenderPass> mHandle;
		unsigned int mColorAttachmentCount;
		Renderer::RasterizationSamples mSamples;
	};
}

#endif // COCKTAIL_VULKAN_FRAMEBUFFER_RENDERPASS_HPP
