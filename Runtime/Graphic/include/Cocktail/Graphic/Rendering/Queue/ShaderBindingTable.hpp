#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_SHADERBINDINGTABLE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_SHADERBINDINGTABLE_HPP

#include <Cocktail/Core/Memory/Allocator/SizedLinearAllocator.hpp>
#include <Cocktail/Core/Utility/CompositeKey.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialProgramInterface.hpp>

#include <Cocktail/Renderer/Command/CommandList.hpp>
#include <Cocktail/Renderer/Shader/UniformSlot.hpp>

namespace Ck
{
    class COCKTAIL_GRAPHIC_API ShaderBindingTable
    {
    public:

        class Entry
        {
        public:

            using Key = CompositeKey<ShaderBindingDomain, BindingSlot>;

            virtual ~Entry() = default;

            virtual void Bind(Renderer::CommandList& commandList, const Renderer::UniformSlot* slot) = 0;

        protected:

            Entry() :
                mDirty(true)
            {
                /// Nothing
            }

            bool mDirty;
        };

        ShaderBindingTable();

        ShaderBindingTable(const ShaderBindingTable& other) = delete;
        ShaderBindingTable(ShaderBindingTable&& other) = default;
        ShaderBindingTable& operator=(const ShaderBindingTable& other) = delete;
        ShaderBindingTable& operator=(ShaderBindingTable&& other) = default;

        void BindBuffer(ShaderBindingDomain domain, BindingSlot slot, const Renderer::Buffer* buffer, unsigned int arrayIndex, std::size_t offset, std::size_t range);
        void BindSampler(ShaderBindingDomain domain, BindingSlot slot, const Renderer::Sampler* sampler, unsigned int arrayIndex = 0);
        void BindTexture(ShaderBindingDomain domain, BindingSlot slot, const Renderer::TextureView* textureView, unsigned int arrayIndex = 0);
        void BindTextureSampler(ShaderBindingDomain domain, BindingSlot slot, const Renderer::TextureView* textureView, const Renderer::Sampler* sampler,
                                unsigned int arrayIndex = 0);

        const std::unordered_map<Entry::Key, UniquePtr<Entry>>& GetEntries() const;

    private:

        template <typename T>
        T* FindOrCreateEntry(ShaderBindingDomain domain, BindingSlot slot)
        {
            T* entry;
            Entry::Key key{ domain, slot };
            if (auto it = mEntries.find(key); it != mEntries.end())
            {
                entry = static_cast<T*>(it->second.Get());
            }
            else
            {
                UniquePtr<T> newEntry = MakeUnique<T>();
                entry = newEntry.Get();

                mEntries[key] = std::move(newEntry);
            }

            return entry;
        }

        class BufferEntry : public Entry
        {
        public:

            void Bind(Renderer::CommandList& commandList, const Renderer::UniformSlot* slot) override
            {
                if (mDirty)
                {
                    for (uint32_t i = 0; i < slot->GetArrayLength(); ++i)
                    {
                        const BufferInfo& bufferInfo = mBuffers[i];
                        if (bufferInfo.Buffer != nullptr)
                            commandList.BindBuffer(slot, i, bufferInfo.Buffer, bufferInfo.Offset, bufferInfo.Range);
                    }

                    mDirty = false;
                }
            }

            void SetBuffer(unsigned int arrayIndex, const Renderer::Buffer* buffer, std::size_t offset, std::size_t range)
            {
                mDirty |= mBuffers.EnsureSize(arrayIndex + 1);
                mDirty |= CheckedAssign(mBuffers[arrayIndex].Buffer, buffer);
                mDirty |= CheckedAssign(mBuffers[arrayIndex].Offset, offset);
                mDirty |= CheckedAssign(mBuffers[arrayIndex].Range, range);
            }

        private:

            struct BufferInfo
            {
                const Renderer::Buffer* Buffer = nullptr;
                std::size_t Offset = 0;
                std::size_t Range = 0;
            };

            Array<BufferInfo, LinearAllocator<4>> mBuffers;
        };

        class SamplerEntry : public Entry
        {
        public:

            void Bind(Renderer::CommandList& commandList, const Renderer::UniformSlot* slot) override
            {
                if (mDirty)
                {
                    for (uint32_t i = 0; i < slot->GetArrayLength(); ++i)
                        commandList.BindSampler(slot, i, mSamplers[i]);

                    mDirty = false;
                }
            }

            void SetSampler(unsigned int arrayIndex, const Renderer::Sampler* sampler)
            {
                mDirty |= mSamplers.EnsureSize(arrayIndex + 1, nullptr);
                mDirty |= CheckedAssign(mSamplers[arrayIndex], sampler);
            }

        private:

            Array<const Renderer::Sampler*, LinearAllocator<4>> mSamplers;
        };

        class TextureEntry : public Entry
        {
        public:

            void Bind(Renderer::CommandList& commandList, const Renderer::UniformSlot* slot) override
            {
                if (mDirty)
                {
                    for (uint32_t i = 0; i < slot->GetArrayLength(); ++i)
                        commandList.BindTexture(slot, i, mTextures[i]);

                    mDirty = false;
                }
            }

            void SetTexture(unsigned int arrayIndex, const Renderer::TextureView* textureView)
            {
                mDirty |= mTextures.EnsureSize(arrayIndex + 1, nullptr);
                mDirty |= CheckedAssign(mTextures[arrayIndex], textureView);
            }

        private:

            Array<const Renderer::TextureView*, LinearAllocator<4>> mTextures;
        };

        class TextureSamplerEntry : public Entry
        {
        public:

            void Bind(Renderer::CommandList& commandList, const Renderer::UniformSlot* slot) override
            {
                if (mDirty)
                {
                    for (uint32_t i = 0; i < slot->GetArrayLength(); ++i)
                        commandList.BindTextureSampler(slot, i, mTextures[i], mSamplers[i]);

                    mDirty = false;
                }
            }

            void SetTextureAndSampler(unsigned int arrayIndex, const Renderer::TextureView* textureView, const Renderer::Sampler* sampler)
            {
                mDirty |= mTextures.EnsureSize(arrayIndex + 1, nullptr);
                mDirty |= CheckedAssign(mTextures[arrayIndex], textureView);

                mDirty |= mSamplers.EnsureSize(arrayIndex + 1, nullptr);
                mDirty |= CheckedAssign(mSamplers[arrayIndex], sampler);
            }

        private:

            Array<const Renderer::TextureView*, LinearAllocator<4>> mTextures;
            Array<const Renderer::Sampler*, LinearAllocator<4>> mSamplers;
        };

        std::unordered_map<Entry::Key, UniquePtr<Entry>> mEntries;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_SHADERBINDINGTABLE_HPP
