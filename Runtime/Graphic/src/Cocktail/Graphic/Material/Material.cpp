#include <Cocktail/Graphic/Material/Material.hpp>

namespace Ck
{
	Material::Material(std::string name, ShadingMode shadingMode, bool doubleSided) :
		mName(std::move(name)),
		mShadingMode(shadingMode),
		mDoubleSided(doubleSided),
		mOpaque(true)
	{
		/// Nothing
	}

	const std::string& Material::GetName() const
	{
		return mName;
	}

	Material::ShadingMode Material::GetShadingMode() const
	{
		return mShadingMode;
	}

	void Material::SetBaseColor(const LinearColor& color)
	{
		mUniformColors.Base = color;
	}

	void Material::SetSpecularColor(const LinearColor& color)
	{
		mUniformColors.Specular = color;
	}

	void Material::SetEmissiveColor(const LinearColor& color)
	{
		mUniformColors.Emission = color;
	}

	void Material::SetTexture(TextureType textureType, std::shared_ptr<TextureResource> textureResource)
	{
		mTextures[textureType] = std::move(textureResource);
	}

	std::shared_ptr<TextureResource> Material::GetTexture(TextureType textureType) const
	{
		return mTextures[textureType];
	}

	bool Material::IsEmissive() const
	{
		return mUniformColors.Base != LinearColor{ 0.f, 0.f, 0.f, 1.f } || mTextures[TextureType::Emission] != nullptr;
	}

	const Material::UniformColors& Material::GetUniformColors() const
	{
		return mUniformColors;
	}

	bool Material::IsDoubleSided() const
	{
		return mDoubleSided;
	}

	bool Material::IsOpaque() const
	{
		return mOpaque;
	}

	void Material::SetOpaque(bool opaque)
	{
		mOpaque = opaque;
	}
}
