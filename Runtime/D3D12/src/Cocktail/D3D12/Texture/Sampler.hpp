#ifndef COCKTAIL_D3D12_TEXTURE_SAMPLER_HPP
#define COCKTAIL_D3D12_TEXTURE_SAMPLER_HPP

#include <memory>

#include <d3d12.h>
#include <wrl/client.h>

#include <Cocktail/Renderer/Texture/Sampler.hpp>
#include <Cocktail/Renderer/Texture/SamplerCreateInfo.hpp>

namespace Ck::D3D12
{
    class RenderDevice;

    class Sampler : public Renderer::Sampler, public std::enable_shared_from_this<Sampler>
    {
    public:

        Sampler(RenderDevice* renderDevice, const Renderer::SamplerCreateInfo& createInfo);
        void SetObjectName(const char* name) const override;
        Renderer::RenderDevice* GetRenderDevice() const override;
        Renderer::Filter GetMagFilter() const override;
        Renderer::Filter GetMinFilter() const override;
        Renderer::SamplerMipmapMode GetMipmapMode() const override;
        bool IsAnisotropyEnabled() const override;
        float GetAnisotropy() const override;
        float GetMinLod() const override;
        float GetMaxLod() const override;

    private:

        RenderDevice* mRenderDevice;
    };
}

#endif // COCKTAIL_D3D12_TEXTURE_SAMPLER_HPP
