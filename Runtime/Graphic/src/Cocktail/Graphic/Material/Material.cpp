#include <Cocktail/Graphic/Material/Material.hpp>

namespace Ck
{
    Material::Material(AsciiString name, ShadingMode shadingMode, bool doubleSided) :
        mName(std::move(name)),
        mShadingMode(shadingMode),
        mDoubleSided(doubleSided),
        mAlphaMode(AlphaMode::Opaque),
        mAlphaCutoff(0.f)
    {
        /// Nothing
    }

    void Material::SetBaseColor(const LinearColor& color)
    {
        mParameters.Base = color;
    }

    void Material::SetEmissiveColor(const LinearColor& color)
    {
        mParameters.Emission = color;
    }

    void Material::SetMetallic(float metallic)
    {
        mParameters.Metallic = metallic;
    }

    void Material::SetRoughness(float roughness)
    {
        mParameters.Roughness = roughness;
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
        return mParameters.Base != LinearColor{ 0.f, 0.f, 0.f, 1.f } || mTextures[TextureType::Emission] != nullptr;
    }

    const AsciiString& Material::GetName() const
    {
        return mName;
    }

    Material::ShadingMode Material::GetShadingMode() const
    {
        return mShadingMode;
    }

    const Material::Parameters& Material::GetParameters() const
    {
        return mParameters;
    }

    bool Material::IsDoubleSided() const
    {
        return mDoubleSided;
    }

    Material::AlphaMode Material::GetAlphaMode() const
    {
        return mAlphaMode;
    }

    void Material::SetAlphaMode(AlphaMode alphaMode)
    {
        mAlphaMode = alphaMode;
    }

    float Material::GetAlphaCutoff() const
    {
        return mAlphaCutoff;
    }

    void Material::SetAlphaCutoff(float alphaCutoff)
    {
        mAlphaCutoff = alphaCutoff;
    }
}
