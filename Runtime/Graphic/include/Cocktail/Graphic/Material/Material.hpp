#ifndef COCKTAIL_GRAPHIC_MATERIAL_MATERIAL_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_MATERIAL_HPP

#include <string>

#include <Cocktail/Core/Color.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Graphic/Export.hpp>

namespace Ck
{
	class TextureResource;

	class COCKTAIL_GRAPHIC_API Material : public Extends<Material, Object>
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
			Specular = Bit(2),
			SpecularHighlight = Bit(3),
			Bump = Bit(4),
			Displacement = Bit(5),
			Alpha = Bit(6),
			Reflection = Bit(7),
			Emission = Bit(8),
		};

		struct UniformColors
		{
			alignas(16) LinearColor Base;
			alignas(16) LinearColor Specular;
			alignas(16) LinearColor Emission;
		};

		/**
		 * \brief 
		 * \param name 
		 * \param shadingMode 
		 */
		Material(std::string name, ShadingMode shadingMode, bool doubleSided = false);

		/**
		 * \brief
		 * \param color
		 */
		void SetBaseColor(const LinearColor& color);

		/**
		 * \brief
		 * \param color
		 */
		void SetSpecularColor(const LinearColor& color);

		/**
		 * \brief
		 * \param color
		 */
		void SetEmissiveColor(const LinearColor& color);

		/**
		 * \brief 
		 * \param textureType 
		 * \param textureResource 
		 */
		void SetTexture(TextureType textureType, Ref<TextureResource> textureResource);

		/**
		 * \brief 
		 * \param textureType 
		 * \return 
		 */
		Ref<TextureResource> GetTexture(TextureType textureType) const;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsEmissive() const;

		/**
		 * \brief 
		 * \return 
		 */
		const std::string& GetName() const;

		/**
		 * \brief 
		 * \return 
		 */
		ShadingMode GetShadingMode() const;

		/**
		 * \brief 
		 * \return 
		 */
		const UniformColors& GetUniformColors() const;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsDoubleSided() const;

		bool IsOpaque() const;
		void SetOpaque(bool opaque);

	private:

		std::string mName;
		ShadingMode mShadingMode;
		UniformColors mUniformColors;
		EnumMap<TextureType, Ref<TextureResource>> mTextures;
		bool mDoubleSided;
		bool mOpaque;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_M ATERIAL_HPP
