#ifndef COCKTAIL_D3D12_COMMAND_COMMANDLIST_HPP
#define COCKTAIL_D3D12_COMMAND_COMMANDLIST_HPP

#include <memory>

#include <d3d12.h>
#include <wrl/client.h>

#include <Cocktail/Renderer/Command/CommandList.hpp>
#include <Cocktail/Renderer/Command/CommandListCreateInfo.hpp>

namespace Ck::D3D12
{
    class RenderDevice;

    class CommandList : public Renderer::CommandList, public std::enable_shared_from_this<CommandList>
    {
    public:

        CommandList(RenderDevice* renderDevice, const Renderer::CommandListCreateInfo& createInfo);
        void SetObjectName(const char* name) const override;
        Renderer::RenderDevice* GetRenderDevice() const override;
        void Begin(Renderer::CommandList* primary) override;
        void End() override;
        void Barrier(unsigned int barrierCount, const Renderer::GpuBarrier* barriers) override;
        void GenerateMipMaps(const Renderer::Texture* texture, Renderer::ResourceState resourceState, const Renderer::TextureSubResource& subResource) override;
        void ExecuteCommandLists(unsigned int commandListCount, Renderer::CommandList** commandLists) override;
        void ClearColorTexture(const Renderer::Texture* texture, Renderer::ResourceState state, LinearColor colorClearValue, const Renderer::TextureSubResource& subResource) override;
        void ClearDepthStencilTexture(const Renderer::Texture* texture, Renderer::ResourceState state, float depthClearValue, unsigned int stencilCleanValue, const Renderer::TextureSubResource& subResource) override;
        void ClearAttachments(unsigned int firstColorAttachment, unsigned int colorAttachmentCount, LinearColor colorClearValue, float depthClearValue, unsigned int stencilCleanValue) override;
        void UploadBuffer(const Renderer::Buffer* buffer, unsigned int uploadCount, const Renderer::BufferUploadInfo* uploads) override;
        void UploadBuffer(const Renderer::Buffer* buffer, std::size_t offset, std::size_t length, const void* data) override;
        void UploadTexture(const Renderer::Texture* texture, Renderer::ResourceState resourceState, unsigned int uploadCount, const Renderer::TextureUploadInfo* uploads) override;
        void UploadTextureLevel(const Renderer::Texture* texture, Renderer::ResourceState resourceState, unsigned int arrayLayer, unsigned int level, const void* pixels) override;
        void BeginRenderPass(const Renderer::RenderPassBeginInfo& begin) override;
        void EndRenderPass() override;
        void BindShaderProgram(const Renderer::ShaderProgram* shaderProgram) override;
        void BindVertexBuffer(unsigned int binding, const Renderer::Buffer* vertexBuffer, std::size_t offset, unsigned int stride, bool instanced, unsigned int divisor) override;
        void BindIndexBuffer(const Renderer::Buffer* indexBuffer, std::size_t offset, Renderer::IndexType indexType) override;
        void BindSampler(Renderer::UniformSlot* slot, unsigned int arrayIndex, const Renderer::Sampler* sampler) override;
        void BindTextureSampler(Renderer::UniformSlot* slot, unsigned int arrayIndex, const Renderer::TextureView* textureView, const Renderer::Sampler* sampler) override;
        void BindTexture(Renderer::UniformSlot* slot, unsigned int arrayIndex, const Renderer::TextureView* textureView) override;
        void BindBuffer(Renderer::UniformSlot* slot, unsigned int arrayIndex, const Renderer::Buffer* uniformBuffer, std::size_t offset, std::size_t range) override;
        void UpdatePipelineConstant(Renderer::ShaderType shaderType, unsigned int offset, unsigned int length, const void* data) override;
        void EnableVertexBinding(unsigned int binding, bool enable) override;
        void SetVertexInputAttributes(unsigned int binding, unsigned int attributeCount, const Renderer::VertexInputAttribute* attribute) override;
        void SetPrimitiveTopology(Renderer::PrimitiveTopology primitiveTopology) override;
        void SetViewport(const Renderer::Viewport& viewport) override;
        void SetScissor(const Renderer::Scissor& scissor) override;
        void EnableRasterizerDiscard(bool enable) override;
        void SetPolygonMode(Renderer::PolygonMode polygonMode) override;
        void SetCullMode(Renderer::CullMode cullMode) override;
        void SetFrontFace(Renderer::FrontFace frontFace) override;
        void EnableDepthBias(bool enable) override;
        void SetDepthBias(float constantFactor, float clamp, float slopeFactor) override;
        void SetLineWidth(float lineWidth) override;
        void SetShadingRate(Extent2D<int> fragmentSize, Renderer::ShadingRateCombiner pipelineCombineOp, Renderer::ShadingRateCombiner primitiveCombineOp) override;
        void EnableSampleShading(bool enable) override;
        void SetMinSampleShading(float minSampleShading) override;
        void EnableAlphaToCoverage(bool enable) override;
        void EnableAlphaToOne(bool enable) override;
        void EnableDepthTest(bool enable) override;
        void EnableDepthWrite(bool enable) override;
        void SetDepthCompareOp(Renderer::CompareOp compareOp) override;
        void EnableLogicOp(bool enable) override;
        void SetLogicOp(Renderer::LogicOp logicOp) override;
        void SetBlendingConstants(const LinearColor& BlendingConstants) override;
        void EnableBlending(unsigned int attachmentIndex, bool enable) override;
        void SetBlendingEquation(unsigned int attachmentIndex, Renderer::BlendOp colorBlendingOp, Renderer::BlendOp alphaBlendingOp) override;
        void SetBlendingFunction(unsigned int attachmentIndex, Renderer::BlendFactor sourceColor, Renderer::BlendFactor destinationColor, Renderer::BlendFactor sourceAlpha, Renderer::BlendFactor destinationAlpha) override;
        void BeginDebugLabel(const AnsiChar* labelName, LinearColor color) override;
        void InsertDebugLabel(const AnsiChar* labelName, LinearColor color) override;
        void EndDebugLabel() override;
        void Draw(unsigned int vertexCount, unsigned int instanceCount, unsigned int firstVertex, unsigned int firstInstance) override;
        void DrawIndexed(unsigned int indexCount, unsigned int instanceCount, unsigned int firstIndex, int indexOffset, unsigned int firstInstance) override;
        void DrawIndirect(const Renderer::Buffer* buffer, std::size_t offset, unsigned int drawCount, unsigned int stride) override;
        void DrawIndexedIndirect(const Renderer::Buffer* buffer, std::size_t offset, unsigned int drawCount, unsigned int stride) override;
        void Dispatch(unsigned int groupCountX, unsigned int groupCountY, unsigned int groupCountZ) override;
        void DispatchIndirect(const Renderer::Buffer* buffer, std::size_t offset) override;
        Renderer::CommandListState GetState() const override;
        Renderer::CommandListUsageBits GetUsage() const override;
        bool IsSecondary() const override;

    private:

        RenderDevice* mRenderDevice;
        Microsoft::WRL::ComPtr<ID3D12CommandList> mHandle;
    };
}

#endif // COCKTAIL_D3D12_COMMAND_COMMANDLIST_HPP
