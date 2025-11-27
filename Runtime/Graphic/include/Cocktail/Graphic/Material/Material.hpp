#ifndef COCKTAIL_GRAPHIC_MATERIAL_MATERIAL_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_MATERIAL_HPP

#include <memory>

#include <Cocktail/Core/Color.hpp>
#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Graphic/Export.hpp>

namespace Ck
{
	class TextureResource;

	class COCKTAIL_GRAPHIC_API Material
	{
	public:

		enum class ShadingMode
		{
			Flat,
			Phong,
			BlinnPhong,
			BRDF,
			Unlit,
		};

		enum class TextureType
		{
			Ambient = Bit(0),
			BaseColor = Bit(1),
			MetallicRoughness = Bit(2),
			Normal = Bit(3),
			Displacement = Bit(4),
			Alpha = Bit(5),
			Reflection = Bit(6),
			Emission = Bit(7),
		};

		struct Parameters
		{
			LinearColor Base;
			LinearColor Emission;
			float Roughness = 0.f;
		    float Metallic = 0.f;
		};

		enum class AlphaMode
		{
			Opaque,
			Blend,
			Mask
		};

		/**
		 * \brief 
		 * \param name 
		 * \param shadingMode
		 * \param doubleSided 
		 */
		Material(AsciiString name, ShadingMode shadingMode, bool doubleSided = false);

		/**
		 * \brief
		 * \param color
		 */
		void SetBaseColor(const LinearColor& color);

		/**
		 * \brief
		 * \param color
		 */
		void SetEmissiveColor(const LinearColor& color);

        /**
         * \brief
         * \param metallic
         */
	    void SetMetallic(float metallic);

        /**
         * \brief
         * \param roughness
         */
	    void SetRoughness(float roughness);

		/**
		 * \brief 
		 * \param textureType 
		 * \param textureResource 
		 */
		void SetTexture(TextureType textureType, std::shared_ptr<TextureResource> textureResource);

		/**
		 * \brief 
		 * \param textureType 
		 * \return 
		 */
		std::shared_ptr<TextureResource> GetTexture(TextureType textureType) const;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsEmissive() const;

		/**
		 * \brief 
		 * \return 
		 */
		const AsciiString& GetName() const;

		/**
		 * \brief 
		 * \return 
		 */
		ShadingMode GetShadingMode() const;

		/**
		 * \brief 
		 * \return 
		 */
		const Parameters& GetParameters() const;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsDoubleSided() const;

		AlphaMode GetAlphaMode() const;
		void SetAlphaMode(AlphaMode alphaMode);

		float GetAlphaCutoff() const;
		void SetAlphaCutoff(float alphaCutoff);

	private:

		AsciiString mName;
		ShadingMode mShadingMode;
		Parameters mParameters;
		EnumMap<TextureType, std::shared_ptr<TextureResource>> mTextures;
		bool mDoubleSided;
		AlphaMode mAlphaMode;
		float mAlphaCutoff;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_MATERIAL_HPP
