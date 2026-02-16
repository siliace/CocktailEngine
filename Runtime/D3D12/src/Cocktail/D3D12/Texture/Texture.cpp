#include <Cocktail/D3D12/RenderDevice.hpp>
#include <Cocktail/D3D12/Texture/Texture.hpp>

namespace Ck::D3D12
{
    Texture::Texture(RenderDevice* renderDevice, const Renderer::TextureCreateInfo& createInfo) :
        mRenderDevice(renderDevice)
    {
        /// Nothing
    }

    void Texture::SetObjectName(const char* name) const
    {
    }

    Renderer::RenderDevice* Texture::GetRenderDevice() const
    {
        return mRenderDevice;
    }

    bool Texture::IsExclusive() const
    {
        return false;
    }

    Renderer::MemoryType Texture::GetMemoryType() const
    {
        return {};
    }

    Renderer::ResourceType Texture::GetResourceType() const
    {
        return {};
    }

    Renderer::TextureType Texture::GetType() const
    {
        return {};
    }

    PixelFormat Texture::GetFormat() const
    {
        return {};
    }

    Extent3D<unsigned int> Texture::GetSize() const
    {
        return {};
    }

    unsigned int Texture::GetMipMapCount() const
    {
        return {};
    }

    unsigned int Texture::GetArrayLayerCount() const
    {
        return {};
    }
}
