#include <Cocktail/D3D12/RenderDevice.hpp>
#include <Cocktail/D3D12/Command/CommandList.hpp>

namespace Ck::D3D12
{
    CommandList::CommandList(RenderDevice* renderDevice, const Renderer::CommandListCreateInfo& createInfo) :
        mRenderDevice(renderDevice)
    {
        /// Nothing
    }

    void CommandList::SetObjectName(const char* name) const
    {
    }

    Renderer::RenderDevice* CommandList::GetRenderDevice() const
    {
        return mRenderDevice;
    }

    void CommandList::Begin(Renderer::CommandList* primary)
    {
    }

    void CommandList::End()
    {
    }

    void CommandList::Barrier(unsigned int barrierCount, const Renderer::GpuBarrier* barriers)
    {
    }

    void CommandList::GenerateMipMaps(const Renderer::Texture* texture, Renderer::ResourceState resourceState, const Renderer::TextureSubResource& subResource)
    {
    }

    void CommandList::ExecuteCommandLists(unsigned int commandListCount, Renderer::CommandList** commandLists)
    {
    }

    void CommandList::ClearColorTexture(const Renderer::Texture* texture, Renderer::ResourceState state, LinearColor colorClearValue, const Renderer::TextureSubResource& subResource)
    {
    }

    void CommandList::ClearDepthStencilTexture(const Renderer::Texture* texture, Renderer::ResourceState state, float depthClearValue, unsigned int stencilCleanValue, const Renderer::TextureSubResource& subResource)
    {
    }

    void CommandList::ClearAttachments(unsigned int firstColorAttachment, unsigned int colorAttachmentCount, LinearColor colorClearValue, float depthClearValue, unsigned int stencilCleanValue)
    {
    }

    void CommandList::UploadBuffer(const Renderer::Buffer* buffer, unsigned int uploadCount, const Renderer::BufferUploadInfo* uploads)
    {
    }

    void CommandList::UploadBuffer(const Renderer::Buffer* buffer, std::size_t offset, std::size_t length, const void* data)
    {
    }

    void CommandList::UploadTexture(const Renderer::Texture* texture, Renderer::ResourceState resourceState, unsigned int uploadCount, const Renderer::TextureUploadInfo* uploads)
    {
    }

    void CommandList::UploadTextureLevel(const Renderer::Texture* texture, Renderer::ResourceState resourceState, unsigned int arrayLayer, unsigned int level, const void* pixels)
    {
    }

    void CommandList::BeginRenderPass(const Renderer::RenderPassBeginInfo& begin)
    {
    }

    void CommandList::EndRenderPass()
    {
    }

    void CommandList::BindShaderProgram(const Renderer::ShaderProgram* shaderProgram)
    {
    }

    void CommandList::BindVertexBuffer(unsigned int binding, const Renderer::Buffer* vertexBuffer, std::size_t offset, unsigned int stride, bool instanced, unsigned int divisor)
    {
    }

    void CommandList::BindIndexBuffer(const Renderer::Buffer* indexBuffer, std::size_t offset, Renderer::IndexType indexType)
    {
    }

    void CommandList::BindSampler(Renderer::UniformSlot* slot, unsigned int arrayIndex, const Renderer::Sampler* sampler)
    {
    }

    void CommandList::BindTextureSampler(Renderer::UniformSlot* slot, unsigned int arrayIndex, const Renderer::TextureView* textureView, const Renderer::Sampler* sampler)
    {
    }

    void CommandList::BindTexture(Renderer::UniformSlot* slot, unsigned int arrayIndex, const Renderer::TextureView* textureView)
    {
    }

    void CommandList::BindBuffer(Renderer::UniformSlot* slot, unsigned int arrayIndex, const Renderer::Buffer* uniformBuffer, std::size_t offset, std::size_t range)
    {
    }

    void CommandList::UpdatePipelineConstant(Renderer::ShaderType shaderType, unsigned int offset, unsigned int length, const void* data)
    {
    }

    void CommandList::EnableVertexBinding(unsigned int binding, bool enable)
    {
    }

    void CommandList::SetVertexInputAttributes(unsigned int binding, unsigned int attributeCount, const Renderer::VertexInputAttribute* attribute)
    {
    }

    void CommandList::SetPrimitiveTopology(Renderer::PrimitiveTopology primitiveTopology)
    {
    }

    void CommandList::SetViewport(const Renderer::Viewport& viewport)
    {
    }

    void CommandList::SetScissor(const Renderer::Scissor& scissor)
    {
    }

    void CommandList::EnableRasterizerDiscard(bool enable)
    {
    }

    void CommandList::SetPolygonMode(Renderer::PolygonMode polygonMode)
    {
    }

    void CommandList::SetCullMode(Renderer::CullMode cullMode)
    {
    }

    void CommandList::SetFrontFace(Renderer::FrontFace frontFace)
    {
    }

    void CommandList::EnableDepthBias(bool enable)
    {
    }

    void CommandList::SetDepthBias(float constantFactor, float clamp, float slopeFactor)
    {
    }

    void CommandList::SetLineWidth(float lineWidth)
    {
    }

    void CommandList::SetShadingRate(Extent2D<int> fragmentSize, Renderer::ShadingRateCombiner pipelineCombineOp, Renderer::ShadingRateCombiner primitiveCombineOp)
    {
    }

    void CommandList::EnableSampleShading(bool enable)
    {
    }

    void CommandList::SetMinSampleShading(float minSampleShading)
    {
    }

    void CommandList::EnableAlphaToCoverage(bool enable)
    {
    }

    void CommandList::EnableAlphaToOne(bool enable)
    {
    }

    void CommandList::EnableDepthTest(bool enable)
    {
    }

    void CommandList::EnableDepthWrite(bool enable)
    {
    }

    void CommandList::SetDepthCompareOp(Renderer::CompareOp compareOp)
    {
    }

    void CommandList::EnableLogicOp(bool enable)
    {
    }

    void CommandList::SetLogicOp(Renderer::LogicOp logicOp)
    {
    }

    void CommandList::SetBlendingConstants(const LinearColor& BlendingConstants)
    {
    }

    void CommandList::EnableBlending(unsigned int attachmentIndex, bool enable)
    {
    }

    void CommandList::SetBlendingEquation(unsigned int attachmentIndex, Renderer::BlendOp colorBlendingOp, Renderer::BlendOp alphaBlendingOp)
    {
    }

    void CommandList::SetBlendingFunction(unsigned int attachmentIndex, Renderer::BlendFactor sourceColor, Renderer::BlendFactor destinationColor, Renderer::BlendFactor sourceAlpha, Renderer::BlendFactor destinationAlpha)
    {
    }

    void CommandList::BeginDebugLabel(const AnsiChar* labelName, LinearColor color)
    {
    }

    void CommandList::InsertDebugLabel(const AnsiChar* labelName, LinearColor color)
    {
    }

    void CommandList::EndDebugLabel()
    {
    }

    void CommandList::Draw(unsigned int vertexCount, unsigned int instanceCount, unsigned int firstVertex, unsigned int firstInstance)
    {
    }

    void CommandList::DrawIndexed(unsigned int indexCount, unsigned int instanceCount, unsigned int firstIndex, int indexOffset, unsigned int firstInstance)
    {
    }

    void CommandList::DrawIndirect(const Renderer::Buffer* buffer, std::size_t offset, unsigned int drawCount, unsigned int stride)
    {
    }

    void CommandList::DrawIndexedIndirect(const Renderer::Buffer* buffer, std::size_t offset, unsigned int drawCount, unsigned int stride)
    {
    }

    void CommandList::Dispatch(unsigned int groupCountX, unsigned int groupCountY, unsigned int groupCountZ)
    {
    }

    void CommandList::DispatchIndirect(const Renderer::Buffer* buffer, std::size_t offset)
    {
    }

    Renderer::CommandListState CommandList::GetState() const
    {
    }

    Renderer::CommandListUsageBits CommandList::GetUsage() const
    {
    }

    bool CommandList::IsSecondary() const
    {
    }
}
