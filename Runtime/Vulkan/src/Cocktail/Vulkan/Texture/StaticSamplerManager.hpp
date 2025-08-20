#ifndef COCKTAIL_VULKAN_TEXTURE_STATICSAMPLERMANAGER_HPP
#define COCKTAIL_VULKAN_TEXTURE_STATICSAMPLERMANAGER_HPP

#include <memory>

#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Renderer/Texture/StaticSampler.hpp>

#include <Cocktail/Vulkan/Texture/Sampler.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	class StaticSamplerManager
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 */
		explicit StaticSamplerManager(RenderDevice* renderDevice);

		/**
		 * \brief 
		 * \param staticSampler 
		 * \return 
		 */
		Sampler* GetSampler(Renderer::StaticSampler staticSampler) const;

	private:

		RenderDevice* mRenderDevice;
		EnumMap<Renderer::StaticSampler, std::shared_ptr<Sampler>> mSamplers;
	};
}

#endif // COCKTAIL_VULKAN_TEXTURE_STATICSAMPLERMANAGER_HPP
