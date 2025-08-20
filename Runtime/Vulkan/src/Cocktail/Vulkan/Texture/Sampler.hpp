#ifndef COCKTAIL_VULKAN_TEXTURE_SAMPLER_HPP
#define COCKTAIL_VULKAN_TEXTURE_SAMPLER_HPP

#include <Cocktail/Renderer/Texture/Sampler.hpp>

#include <Cocktail/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class Sampler : public Renderer::Sampler
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		Sampler(RenderDevice* renderDevice, const Renderer::SamplerCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~Sampler() override;

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
		Renderer::Filter GetMagFilter() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::Filter GetMinFilter() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::SamplerMipmapMode GetMipmapMode() const override;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsAnisotropyEnabled() const override;

		/**
		 * \brief 
		 * \return 
		 */
		float GetAnisotropy() const override;

		/**
		 * \brief 
		 * \return 
		 */
		float GetMinLod() const override;

		/**
		 * \brief 
		 * \return 
		 */
		float GetMaxLod() const override;

		/**
		 * \brief 
		 * \return 
		 */
		VkSampler GetHandle() const;

	private:

		RenderDevice* mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkSampler mHandle;
		Renderer::Filter mMagFilter;
		Renderer::Filter mMinFilter;
		Renderer::SamplerMipmapMode mMipmapMode;
		Renderer::SamplerAddressMode mAddressModeU;
		Renderer::SamplerAddressMode mAddressModeV;
		Renderer::SamplerAddressMode mAddressModeW;
		bool mAnisotropyEnable;
		float mAnisotropy;
		float mMinLoad;
		float mMaxLoad;
	};
}

#endif // COCKTAIL_VULKAN_TEXTURE_SAMPLER_HPP
