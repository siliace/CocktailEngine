#ifndef COCKTAIL_RENDERER_TEXTRURE_SAMPLERCREATEINFO_HPP
#define COCKTAIL_RENDERER_TEXTRURE_SAMPLERCREATEINFO_HPP

#include <tuple>

#include <Cocktail/Core/Cocktail.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief Enumeration of filters used by Sampler for texture lookup
	 */
	enum class Filter
	{
		/**
		 * \brief Specifies nearest filtering
		 */
		Nearest,

		/**
		 * \brief Specifies linear filtering
		 */
		Linear,
	};

	/**
	 * \brief Enumeration of mipmap modes used by Sampler for texture lookup
	 */
	enum class SamplerMipmapMode
	{
		/**
		 * \brief Specifies nearest filtering
		 */
		Nearest,

		/**
		 * \brief Specifies linear filtering
		 */
		 Linear,
	};

	/**
	 * \brief 
	 */
	enum class SamplerAddressMode
	{
		/**
		 * \brief 
		 */
		Repeat,

		/**
		 * \brief 
		 */
		MirroredRepeat,

		/**
		 * \brief 
		 */
		ClampToEdge,

		/**
		 * \brief 
		 */
		ClampToBorder,
	};

	/**
	 * \brief Sampler descriptor structure
     * Describe the set of parameters to create a Sampler
     * \see RenderDevice::CreateSampler
	 */
	struct SamplerCreateInfo
	{
		/**
		 * \brief Specifies the magnification filter to apply to textures lookups
		 */
		Filter MagFilter = Filter::Linear;

		/**
		 * \brief Specifies the minification filter to apply to textures lookups
		 */
		Filter MinFilter = Filter::Linear;

		/**
		 * \brief Specifies the mipmaps filter to apply to textures lookups
		 */
		SamplerMipmapMode MipmapMode = SamplerMipmapMode::Linear;

		/**
		 * \brief 
		 */
		SamplerAddressMode AddressModeU = SamplerAddressMode::Repeat;

		/**
		 * \brief 
		 */
		SamplerAddressMode AddressModeV = SamplerAddressMode::Repeat;

		/**
		 * \brief 
		 */
		SamplerAddressMode AddressModeW = SamplerAddressMode::Repeat;
									    
		/**
		 * \brief Specifies whether enable the anisotropic filtering on the sampler
		 */
		bool EnableAnisotropy = false;

		/**
		 * \brief Specifies the anisotropy value clamp used by the sampler
		 * This anisotropy value must be within the [0, 16] interval
		 * If EnableAnisotropy is false, this value is ignored
		 */
		float MaxAnisotropy = 1.f;

		/**
		 * \brief Specifies the minimum of of the computed LOD value
		 */
		float MinLod = 0.f;
		
		/**
		 * \brief Specifies the maximum of of the computed LOD value
		 */
		float MaxLod = 1000.f;

        /**
         * \brief Specifies the name of the Sampler to create
         * If the extension Debug is not supported by the RenderDevice creating the Sampler, this parameter is ignored
         * \see RenderDeviceObject::GetName
         */
        const char* Name = nullptr;
	};
}

#endif // COCKTAIL_RENDERER_TEXTRURE_SAMPLERCREATEINFO_HPP
