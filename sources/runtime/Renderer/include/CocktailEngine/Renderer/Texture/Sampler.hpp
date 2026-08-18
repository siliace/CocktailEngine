#ifndef COCKTAILENGINE_RENDERER_TEXTURE_SAMPLER_HPP
#define COCKTAILENGINE_RENDERER_TEXTURE_SAMPLER_HPP

#include <CocktailEngine/Renderer/RenderDeviceObject.hpp>
#include <CocktailEngine/Renderer/Texture/SamplerCreateInfo.hpp>

namespace Ck::Renderer
{
	/**
     * \brief Interface representing an opaque handle to a GPU sampler object
	 */
	class Sampler : public RenderDeviceObject
	{
	public:

		/**
		 * \brief Get the magnification filter applied by this sampler to textures lookups
		 * \return The magnification filter
		 * \see SamplerCreateInfo::MagFilter
		 */
		virtual Filter GetMagFilter() const = 0;

		/**
		 * \brief Get the minification filter applied by this sampler to textures lookups
		 * \return The minification filter
		 * \see SamplerCreateInfo::MinFilter
		 */
		virtual Filter GetMinFilter() const = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual SamplerMipmapMode GetMipmapMode() const = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual bool IsAnisotropyEnabled() const = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual float GetAnisotropy() const = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual float GetMinLod() const = 0;
		
		/**
		 * \brief
		 * \return
		 */
		virtual float GetMaxLod() const = 0;
	};
}

#endif // COCKTAILENGINE_RENDERER_TEXTURE_SAMPLER_HPP
