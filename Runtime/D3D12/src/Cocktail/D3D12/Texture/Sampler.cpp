#include <Cocktail/D3D12/RenderDevice.hpp>
#include <Cocktail/D3D12/Texture/Sampler.hpp>

namespace Ck::D3D12
{
    Sampler::Sampler(RenderDevice* renderDevice, const Renderer::SamplerCreateInfo& createInfo) :
        mRenderDevice(renderDevice)
    {
    }

    void Sampler::SetObjectName(const char* name) const
    {
    }

    Renderer::RenderDevice* Sampler::GetRenderDevice() const
    {
        return mRenderDevice;
    }

    Renderer::Filter Sampler::GetMagFilter() const
    {
        return {};
    }

    Renderer::Filter Sampler::GetMinFilter() const
    {
        return {};
    }

    Renderer::SamplerMipmapMode Sampler::GetMipmapMode() const
    {
        return {};
    }

    bool Sampler::IsAnisotropyEnabled() const
    {
        return {};
    }

    float Sampler::GetAnisotropy() const
    {
        return {};
    }

    float Sampler::GetMinLod() const
    {
        return {};
    }

    float Sampler::GetMaxLod() const
    {
        return {};
    }
}
