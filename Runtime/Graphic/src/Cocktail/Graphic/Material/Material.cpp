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

	void Material::SetBaseColor(const RgbaColor<float>& color)
	{
		mUniformColors.Base.Red = color.Red / 255.f;
		mUniformColors.Base.Green = color.Green / 255.f;
		mUniformColors.Base.Blue = color.Blue / 255.f;
		mUniformColors.Base.Alpha = color.Alpha / 255.f;
	}

	void Material::SetSpecularColor(const RgbColor<float>& color)
	{
		mUniformColors.Specular.Red = color.Red / 255.f;
		mUniformColors.Specular.Green = color.Green / 255.f;
		mUniformColors.Specular.Blue = color.Blue / 255.f;
	}

	void Material::SetEmissiveColor(const RgbColor<float>& color)
	{
		mUniformColors.Emission.Red = color.Red / 255.f;
		mUniformColors.Emission.Green = color.Green / 255.f;
		mUniformColors.Emission.Blue = color.Blue / 255.f;
	}

	void Material::SetTexture(TextureType textureType, Ref<TextureResource> textureResource)
	{
		mTextures[textureType] = std::move(textureResource);
	}

	Ref<TextureResource> Material::GetTexture(TextureType textureType) const
	{
		return mTextures[textureType];
	}

	bool Material::IsEmissive() const
	{
		return mUniformColors.Base != RgbaColor<float>::Black || mTextures[TextureType::Emission] != nullptr;
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
