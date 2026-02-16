#ifndef COCKTAIL_D3D12_TEXTURE_TEXTURE_HPP
#define COCKTAIL_D3D12_TEXTURE_TEXTURE_HPP

#include <memory>

#include <d3d12.h>
#include <wrl/client.h>

#include <Cocktail/Renderer/Texture/Texture.hpp>
#include <Cocktail/Renderer/Texture/TextureCreateInfo.hpp>

namespace Ck::D3D12
{
    class RenderDevice;

    class Texture : public Renderer::Texture, public std::enable_shared_from_this<Texture>
    {
    public:

        Texture(RenderDevice* renderDevice, const Renderer::TextureCreateInfo& createInfo);
        void SetObjectName(const char* name) const override;
        Renderer::RenderDevice* GetRenderDevice() const override;
        bool IsExclusive() const override;
        Renderer::MemoryType GetMemoryType() const override;
        Renderer::ResourceType GetResourceType() const override;
        Renderer::TextureType GetType() const override;
        PixelFormat GetFormat() const override;
        Extent3D<unsigned int> GetSize() const override;
        unsigned GetMipMapCount() const override;
        unsigned GetArrayLayerCount() const override;

    private:

        RenderDevice* mRenderDevice;
        Microsoft::WRL::ComPtr<ID3D12Resource> mHandle;
    };
}

#endif // COCKTAIL_D3D12_TEXTURE_TEXTURE_HPP
