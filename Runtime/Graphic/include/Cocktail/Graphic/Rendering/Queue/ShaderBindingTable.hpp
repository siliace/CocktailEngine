#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_SHADERBINDINGTABLE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_SHADERBINDINGTABLE_HPP

#include <Cocktail/Core/Memory/Allocator/SizedLinearAllocator.hpp>
#include <Cocktail/Core/Utility/CompositeKey.hpp>

#include <Cocktail/Graphic/Material/Shading/MaterialProgramInterface.hpp>
#include <Cocktail/Graphic/Material/Shading/MaterialProgramVariant.hpp>

#include <Cocktail/Renderer/Command/CommandList.hpp>
#include <Cocktail/Renderer/Shader/UniformSlot.hpp>

namespace Ck
{
    /**
     * \brief Table storing shader resource bindings.
     *
     * A ShaderBindingTable acts as an intermediate layer between engine resources
     * and shader uniform slots. Resources can be bound by domain and binding slot,
     * then applied to a command list for a specific material program variant.
     *
     * Bindings are cached internally and only rebound when marked dirty.
     */
    class COCKTAIL_GRAPHIC_API ShaderBindingTable
    {
    public:

        /**
         * \brief Default constructor
         * 
         * Creates a new instance of an empty shader binding table.
         */
        ShaderBindingTable();

        ShaderBindingTable(const ShaderBindingTable& other) = delete;
        ShaderBindingTable(ShaderBindingTable&& other) = default;
        ShaderBindingTable& operator=(const ShaderBindingTable& other) = delete;
        ShaderBindingTable& operator=(ShaderBindingTable&& other) = default;

        /**
         * \brief Bind a buffer resource to a binding slot
         *
         * \param domain The binding domain where the resource belongs
         * \param slot The binding slot identifier
         * \param buffer The buffer to bind
         * \param arrayIndex The array element index for array bindings
         * \param offset Byte offset within the buffer
         * \param range Size in bytes of the bound buffer range
         */
        void BindBuffer(ShaderBindingDomain domain, BindingSlot slot, const Renderer::Buffer* buffer, unsigned int arrayIndex, std::size_t offset, std::size_t range);

        /**
         * \brief Bind a sampler resource to a binding slot
         *
         * \param domain The binding domain where the resource belongs
         * \param slot The binding slot identifier
         * \param sampler The sampler to bind
         * \param arrayIndex The array element index for array bindings
         */
        void BindSampler(ShaderBindingDomain domain, BindingSlot slot, const Renderer::Sampler* sampler, unsigned int arrayIndex = 0);

        /**
         * \brief Bind a texture resource to a binding slot
         *
         * \param domain The binding domain where the resource belongs
         * \param slot The binding slot identifier
         * \param textureView The texture view to bind
         * \param arrayIndex The array element index for array bindings
         */
        void BindTexture(ShaderBindingDomain domain, BindingSlot slot, const Renderer::TextureView* textureView, unsigned int arrayIndex = 0);

        /**
         * \brief Bind a texture and sampler pair to a binding slot
         *
         * \param domain The binding domain where the resource belongs
         * \param slot The binding slot identifier
         * \param textureView The texture view to bind
         * \param sampler The sampler to bind
         * \param arrayIndex The array element index for array bindings
         */
        void BindTextureSampler(ShaderBindingDomain domain, BindingSlot slot, const Renderer::TextureView* textureView, const Renderer::Sampler* sampler,
                                unsigned int arrayIndex = 0);

        /**
         * \brief Bind current resources in a command list
         *
         * If a binding has no matching uniform slot in \p programVariant,
         * it is ignored.
         *
         * \param commandList The command list where resources will be bound
         * \param programVariant The material program variant that will use the bound resources
         */
        void Bind(Renderer::CommandList& commandList, const MaterialProgramVariant& programVariant) const;

        /**
         * \brief Mark all entries as dirty
         *
         * Forces all resources to be rebound during the next bind operation
         */
        void Invalidate() const;

    private:

        /**
         * \brief Retrieve or create an entry associated with a binding location
         *
         * If no entry already exists for the specified domain and slot,
         * a new one is created.
         *
         * \tparam T Type of entry to create if missing
         *
         * \param domain The binding domain
         * \param slot The binding slot
         *
         * \return The existing or newly created entry
         */
        template <typename T>
        T* FindOrCreateEntry(ShaderBindingDomain domain, BindingSlot slot)
        {
            EntryKey key{ domain, slot };
            UniquePtr<Entry>& entry = mEntries.ComputeIfMissing(key, [](const EntryKey&) -> UniquePtr<Entry> {
                return MakeUnique<T>();
            });

            return static_cast<T*>(entry.Get());
        }

        /**
         * \brief Base class for all resource binding entries
         *
         * An entry stores one resource type and is responsible for binding it
         * to a command list when requested.
         */
        class Entry
        {
        public:

            /**
             * \brief Virtual destructor
             */
            virtual ~Entry() = default;

            /**
             * \brief Bind the resource held by this entry
             *
             * \param commandList The command list where the resource will be bound
             * \param slot The uniform slot receiving the resource
             */
            virtual void Bind(Renderer::CommandList& commandList, const Renderer::UniformSlot* slot) = 0;

            /**
             * \brief Mark the entry as dirty
             *
             * Dirty entries are rebound during the next bind operation.
             * This is typically required when shader state changes.
             */
            void Invalidate();

        protected:

            unsigned int mDirty = 0; /*!< Bitmask indicating which array elements require rebinding */
        };

        /**
             * \brief Resource entry storing buffer bindings
             */
        class BufferEntry : public Entry
        {
        public:

            /**
             * \copydoc Entry::Bind
             */
            void Bind(Renderer::CommandList& commandList, const Renderer::UniformSlot* slot) override;

            /**
             * \brief Set a buffer binding
             *
             * \param arrayIndex Array element index
             * \param buffer Buffer to bind
             * \param offset Byte offset in the buffer
             * \param range Size in bytes of the bound range
             */
            void SetBuffer(unsigned int arrayIndex, const Renderer::Buffer* buffer, std::size_t offset, std::size_t range);

        private:

            /**
             * \brief Information describing a buffer binding
             */
            struct BufferInfo
            {
                const Renderer::Buffer* Buffer = nullptr; /*!< Bound buffer */
                std::size_t Offset = 0; /*!< Byte offset within the buffer */
                std::size_t Range = 0; /*!< Bound range size in bytes */
            };

            Array<BufferInfo, LinearAllocator<4>> mBuffers; /*!< Bound buffer array */
        };

        /**
         * \brief Resource entry storing sampler bindings
         */
        class SamplerEntry : public Entry
        {
        public:

            /**
             * \copydoc Entry::Bind
             */
            void Bind(Renderer::CommandList& commandList, const Renderer::UniformSlot* slot) override;

            /**
             * \brief Set a sampler binding
             *
             * \param arrayIndex Array element index
             * \param sampler Sampler to bind
             */
            void SetSampler(unsigned int arrayIndex, const Renderer::Sampler* sampler);

        private:

            Array<const Renderer::Sampler*, LinearAllocator<4>> mSamplers; /*!< Bound sampler array */
        };

        /**
         * \brief Resource entry storing texture bindings
         */
        class TextureEntry : public Entry
        {
        public:

            /**
             * \copydoc Entry::Bind
             */
            void Bind(Renderer::CommandList& commandList, const Renderer::UniformSlot* slot) override;

            /**
             * \brief Set a texture binding
             *
             * \param arrayIndex Array element index
             * \param textureView Texture view to bind
             */
            void SetTexture(unsigned int arrayIndex, const Renderer::TextureView* textureView);

        private:

            Array<const Renderer::TextureView*, LinearAllocator<4>> mTextures; /*!< Bound texture array */
        };

        /**
         * \brief Resource entry storing combined texture and sampler bindings
         */
        class TextureSamplerEntry : public Entry
        {
        public:

            /**
             * \copydoc Entry::Bind
             */
            void Bind(Renderer::CommandList& commandList, const Renderer::UniformSlot* slot) override;

            /**
             * \brief Set a texture and sampler binding pair
             *
             * \param arrayIndex Array element index
             * \param textureView Texture view to bind
             * \param sampler Sampler to bind
             */
            void SetTextureAndSampler(unsigned int arrayIndex, const Renderer::TextureView* textureView, const Renderer::Sampler* sampler);

        private:

            Array<const Renderer::TextureView*, LinearAllocator<4>> mTextures; /*!< Bound texture array */
            Array<const Renderer::Sampler*, LinearAllocator<4>> mSamplers; /*!< Bound sampler array */
        };

        using EntryKey = CompositeKey<ShaderBindingDomain, BindingSlot>;

        HashMap<EntryKey, UniquePtr<Entry>> mEntries; /*!< Resource binding entries indexed by domain and slot */
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_SHADERBINDINGTABLE_HPP
