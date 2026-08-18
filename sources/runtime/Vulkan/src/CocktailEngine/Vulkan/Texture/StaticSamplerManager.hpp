#ifndef COCKTAILENGINE_VULKAN_TEXTURE_STATICSAMPLERMANAGER_HPP
#define COCKTAILENGINE_VULKAN_TEXTURE_STATICSAMPLERMANAGER_HPP

#include <memory>

#include <CocktailEngine/Core/Utility/EnumMap.hpp>

#include <CocktailEngine/Renderer/Texture/StaticSampler.hpp>

#include <CocktailEngine/Vulkan/Texture/Sampler.hpp>

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
		EnumMap<Renderer::StaticSampler, SharedPtr<Sampler>> mSamplers;
	};
}

#endif // COCKTAILENGINE_VULKAN_TEXTURE_STATICSAMPLERMANAGER_HPP
