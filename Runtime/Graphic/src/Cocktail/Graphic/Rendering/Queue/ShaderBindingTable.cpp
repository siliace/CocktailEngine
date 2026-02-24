#include <Cocktail/Graphic/Rendering/Queue/ShaderBindingTable.hpp>

#include <Cocktail/Renderer/Shader/UniformSlot.hpp>

namespace Ck
{
    ShaderBindingTable::ShaderBindingTable()
    {
        mEntries.reserve(32);
    }

    void ShaderBindingTable::BindBuffer(ShaderBindingDomain domain, BindingSlot slot, const Renderer::Buffer* buffer, unsigned int arrayIndex, std::size_t offset, std::size_t range)
    {
        FindOrCreateEntry<BufferEntry>(domain, slot)->SetBuffer(arrayIndex, buffer, offset, range);
    }

    void ShaderBindingTable::BindSampler(ShaderBindingDomain domain, BindingSlot slot, const Renderer::Sampler* sampler, unsigned int arrayIndex)
    {
        FindOrCreateEntry<SamplerEntry>(domain, slot)->SetSampler(arrayIndex, sampler);
    }

    void ShaderBindingTable::BindTexture(ShaderBindingDomain domain, BindingSlot slot, const Renderer::TextureView* textureView, unsigned int arrayIndex)
    {
        FindOrCreateEntry<TextureEntry>(domain, slot)->SetTexture(arrayIndex, textureView);
    }

    void ShaderBindingTable::BindTextureSampler(ShaderBindingDomain domain, BindingSlot slot, const Renderer::TextureView* textureView, const Renderer::Sampler* sampler, unsigned int arrayIndex)
    {
        FindOrCreateEntry<TextureSamplerEntry>(domain, slot)->SetTextureAndSampler(arrayIndex, textureView, sampler);
    }

    const std::unordered_map<ShaderBindingTable::Entry::Key, UniquePtr<ShaderBindingTable::Entry>>& ShaderBindingTable::GetEntries() const
    {
        return mEntries;
    }
}
