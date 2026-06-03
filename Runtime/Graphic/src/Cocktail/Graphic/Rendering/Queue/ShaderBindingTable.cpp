#include <Cocktail/Graphic/Rendering/Queue/ShaderBindingTable.hpp>

#include <Cocktail/Renderer/Shader/UniformSlot.hpp>

namespace Ck
{
    ShaderBindingTable::ShaderBindingTable()
    {
        mEntries.Reserve(32);
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

    void ShaderBindingTable::Bind(Renderer::CommandList& commandList, const MaterialProgramVariant& programVariant) const
    {
        for (const auto& [key, entry] : mEntries)
        {
            ShaderBindingDomain domain = std::get<0>(key);
            unsigned int index = std::get<1>(key);
        
            const Renderer::UniformSlot* slot = programVariant.GetSlot(domain, index);
            if (slot != nullptr)
                entry->Bind(commandList, slot);
        }
    }

    void ShaderBindingTable::Invalidate() const
    {
        mEntries.ForEach([](const UniquePtr<Entry>& entry) {
            entry->Invalidate();
        });
    }

    void ShaderBindingTable::Entry::Invalidate()
    {
        mDirty = static_cast<unsigned int>(-1);
    }

    void ShaderBindingTable::BufferEntry::Bind(Renderer::CommandList& commandList, const Renderer::UniformSlot* slot)
    {
        if (mDirty)
        {
            for (unsigned int i = 0; i < slot->GetArrayLength(); ++i)
            {
                if (mDirty & Bit(i))
                {
                    const BufferInfo& bufferInfo = mBuffers[i];
                    if (bufferInfo.Buffer != nullptr)
                        commandList.BindBuffer(slot, i, bufferInfo.Buffer, bufferInfo.Offset, bufferInfo.Range);
                }
            }

            mDirty = 0;
        }
    }

    void ShaderBindingTable::BufferEntry::SetBuffer(unsigned int arrayIndex, const Renderer::Buffer* buffer, std::size_t offset, std::size_t range)
    {
        bool changed = false;
        changed |= mBuffers.EnsureSize(arrayIndex + 1);
        changed |= CheckedAssign(mBuffers[arrayIndex].Buffer, buffer);
        changed |= CheckedAssign(mBuffers[arrayIndex].Offset, offset);
        changed |= CheckedAssign(mBuffers[arrayIndex].Range, range);

        if (changed)
            mDirty |= Bit(arrayIndex);
    }

    void ShaderBindingTable::SamplerEntry::Bind(Renderer::CommandList& commandList, const Renderer::UniformSlot* slot)
    {
        if (mDirty)
        {
            for (unsigned int i = 0; i < slot->GetArrayLength(); ++i)
            {
                if (mDirty & Bit(i))
                    commandList.BindSampler(slot, i, mSamplers[i]);
            }

            mDirty = 0;
        }
    }

    void ShaderBindingTable::SamplerEntry::SetSampler(unsigned int arrayIndex, const Renderer::Sampler* sampler)
    {
        bool changed = false;
        changed |= mSamplers.EnsureSize(arrayIndex + 1, nullptr);
        changed |= CheckedAssign(mSamplers[arrayIndex], sampler);

        if (changed)
            mDirty |= Bit(arrayIndex);
    }

    void ShaderBindingTable::TextureEntry::Bind(Renderer::CommandList& commandList, const Renderer::UniformSlot* slot)
    {
        if (mDirty)
        {
            for (unsigned int i = 0; i < slot->GetArrayLength(); ++i)
            {
                if (mDirty & Bit(i))
                    commandList.BindTexture(slot, i, mTextures[i]);
            }

            mDirty = 0;
        }
    }

    void ShaderBindingTable::TextureEntry::SetTexture(unsigned int arrayIndex, const Renderer::TextureView* textureView)
    {
        bool changed = false;
        changed |= mTextures.EnsureSize(arrayIndex + 1, nullptr);
        changed |= CheckedAssign(mTextures[arrayIndex], textureView);

        if (changed)
            mDirty |= Bit(arrayIndex);
    }

    void ShaderBindingTable::TextureSamplerEntry::Bind(Renderer::CommandList& commandList, const Renderer::UniformSlot* slot)
    {
        if (mDirty)
        {
            for (unsigned int i = 0; i < slot->GetArrayLength(); ++i)
            {
                if (mDirty & Bit(i))
                    commandList.BindTextureSampler(slot, i, mTextures[i], mSamplers[i]);
            }

            mDirty = 0;
        }
    }

    void ShaderBindingTable::TextureSamplerEntry::SetTextureAndSampler(unsigned int arrayIndex, const Renderer::TextureView* textureView, const Renderer::Sampler* sampler)
    {
        bool changed = false;
        changed |= mTextures.EnsureSize(arrayIndex + 1, nullptr);
        changed |= CheckedAssign(mTextures[arrayIndex], textureView);

        changed |= mSamplers.EnsureSize(arrayIndex + 1, nullptr);
        changed |= CheckedAssign(mSamplers[arrayIndex], sampler);
                
        if (changed)
            mDirty |= Bit(arrayIndex);
    }
}
