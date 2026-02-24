#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_RECORDDRAWCONTEXT_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_RECORDDRAWCONTEXT_HPP

#include <Cocktail/Graphic/Geometry/Vertex/VertexLayout.hpp>
#include <Cocktail/Graphic/Material/Shading/MaterialProgramVariant.hpp>
#include <Cocktail/Graphic/Rendering/Queue/ShaderBindingTable.hpp>

#include <Cocktail/Renderer/Buffer/BufferArea.hpp>
#include <Cocktail/Renderer/Context/RenderContext.hpp>

namespace Ck
{
    class COCKTAIL_GRAPHIC_API RecordDrawContext
    {
    public:

        /**
         * \brief
         */
        enum class RenderingModifierBits
        {
            /**
             * \brief
             */
            Wireframe = Bit(0),
        };

        using RenderingModifiers = Flags<RenderingModifierBits>;

        /**
         * \brief
         * \param renderContext
         * \param modifiers
         */
        explicit RecordDrawContext(Renderer::RenderContext& renderContext, RenderingModifiers modifiers = {});

        void BindMaterialProgram(Renderer::CommandList& commandList, const MaterialProgramVariant* materialProgramVariant);

        void BindData(ShaderBindingDomain domain, BindingSlot bindingSlot, Renderer::BufferUsageFlags usage, unsigned int arrayIndex, std::size_t size, const void* data);

        void BindBuffer(ShaderBindingDomain domain, BindingSlot bindingSlot, const Renderer::Buffer* buffer, unsigned int arrayIndex = 0);
        void BindBuffer(ShaderBindingDomain domain, BindingSlot bindingSlot, const Renderer::Buffer* buffer, unsigned int arrayIndex, std::size_t offset, std::size_t range);

        void BindSampler(ShaderBindingDomain domain, BindingSlot bindingSlot, const Renderer::Sampler* sampler, unsigned int arrayIndex = 0);
        void BindTexture(ShaderBindingDomain domain, BindingSlot bindingSlot, const Renderer::TextureView* texture, unsigned int arrayIndex = 0);
        void BindTextureSampler(ShaderBindingDomain domain, BindingSlot bindingSlot, const Renderer::TextureView* texture, const Renderer::Sampler* sampler, unsigned int arrayIndex = 0);

        void BindVertexData(Renderer::CommandList& commandList, unsigned int binding, const VertexLayout* vertexLayout, unsigned int vertexCount, const void* data) const;
        void BindVertexBuffer(Renderer::CommandList& commandList, unsigned int binding, const VertexLayout* vertexLayout, const Renderer::Buffer* buffer, std::size_t offset = 0) const;

        void BindIndexData(Renderer::CommandList& commandList, Renderer::IndexType indexType, unsigned int indexCount, const void* data) const;
        void BindIndexBuffer(Renderer::CommandList& commandList, const Renderer::Buffer* buffer, Renderer::IndexType indexType, std::size_t offset = 0) const;

        void Draw(Renderer::CommandList& commandList, unsigned int vertexCount, unsigned int instanceCount = 1, unsigned int firstVertex = 0, unsigned int firstInstance = 0) const;
        void DrawIndexed(Renderer::CommandList& commandList, unsigned int indexCount, unsigned int instanceCount = 1, unsigned int firstIndex = 0, int indexOffset = 0, unsigned int firstInstance = 0) const;

        void EnableVertexBindings(Renderer::CommandList& commandList, unsigned int firstBinding, unsigned int bindingsCount, bool enable);

        RenderingModifiers GetModifiers() const;

    private:

        /**
         * \brief
         * \param elementCount
         * \return
         */
        static PixelFormat::Layout GetVertexInputAttributeFormatLayout(unsigned int elementCount);

        Renderer::RenderContext* mRenderContext;
        RenderingModifiers mModifiers;
        ShaderBindingTable mBindingTable;
        const MaterialProgramVariant* mCurrentMaterialProgram;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_RECORDDRAWCONTEXT_HPP
