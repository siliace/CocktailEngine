#ifndef COCKTAIL_RENDERER_COMMAND_COMMANDLIST_HPP
#define COCKTAIL_RENDERER_COMMAND_COMMANDLIST_HPP

#include <Cocktail/Renderer/Renderer.hpp>
#include <Cocktail/Renderer/Buffer/IndexType.hpp>
#include <Cocktail/Renderer/Command/BufferUploadInfo.hpp>
#include <Cocktail/Renderer/Command/CommandListDynamicState.hpp>
#include <Cocktail/Renderer/Command/CommandListState.hpp>
#include <Cocktail/Renderer/Command/CommandListUsage.hpp>
#include <Cocktail/Renderer/Command/GpuBarrier.hpp>
#include <Cocktail/Renderer/Command/RenderPassBeginInfo.hpp>
#include <Cocktail/Renderer/Command/TextureUploadInfo.hpp>
#include <Cocktail/Renderer/Shader/ShaderType.hpp>
#include <Cocktail/Renderer/Texture/TextureSubResource.hpp>

namespace Ck::Renderer
{
    class Buffer;
    class Sampler;
    class ShaderProgram;
    class Texture;
    class TextureView;
    class UniformSlot;

	/**
     * \brief 
     */
    class CommandList : public Implements<CommandList, RenderDeviceObject>
    {
    public:

	    /**
         * \brief Start recording command into the CommandList
         */
        virtual void Begin(CommandList* primary) = 0;

	    /**
         * \brief Finish recording a command buffer
         */
        virtual void End() = 0;

	    /**
         * \brief 
         * \param barrierCount 
         * \param barriers 
         */
        virtual void Barrier(unsigned int barrierCount, const GpuBarrier* barriers) = 0;

	    /**
         * \brief Generate the chain of mipmaps for a texture
         * This function should be call on Graphic CommandList and outside of a RenderPass
         * \param texture 
         * \param resourceState 
         * \param subResource 
         */
        virtual void GenerateMipMaps(const Texture* texture, ResourceState resourceState, const TextureSubResource& subResource) = 0;

	    /**
         * \brief 
         * \param commandListCount 
         * \param commandLists 
         */
        virtual void ExecuteCommandLists(unsigned int commandListCount, CommandList** commandLists) = 0;

        virtual void UploadBuffer(const Buffer* buffer, unsigned int uploadCount, const BufferUploadInfo* uploads) = 0;
        virtual void UploadTexture(const Texture* texture, ResourceState resourceState, unsigned int uploadCount, const TextureUploadInfo* uploads) = 0;

        virtual void Dispatch(unsigned int groupCountX, unsigned int groupCountY = 0, unsigned int groupCountZ = 0) = 0;

        virtual void BeginRenderPass(const RenderPassBeginInfo& begin) = 0;
    	virtual void EndRenderPass() = 0;

        virtual void BindShaderProgram(const ShaderProgram* shaderProgram) = 0;
        virtual void BindVertexBuffer(unsigned int binding, const Buffer* vertexBuffer, std::size_t offset, unsigned int stride, bool instanced = false, unsigned int divisor = 0) = 0;
        virtual void BindIndexBuffer(const Buffer* indexBuffer, std::size_t offset, IndexType indexType) = 0;
        virtual void BindSampler(UniformSlot* slot, unsigned int arrayIndex, const Sampler* sampler) = 0;
        virtual void BindTextureSampler(UniformSlot* slot, unsigned int arrayIndex, const TextureView* textureView, const Sampler* sampler) = 0;
        virtual void BindTexture(UniformSlot* slot, unsigned int arrayIndex, const TextureView* textureView) = 0;
        virtual void BindBuffer(UniformSlot* slot, unsigned int arrayIndex, const Buffer* uniformBuffer, std::size_t offset, std::size_t range) = 0;

    	virtual void UpdatePipelineConstant(ShaderType shaderType, unsigned int offset, unsigned int length, const void* data) = 0;

        virtual void EnableVertexBinding(unsigned int binding, bool enable) = 0;
        virtual void SetVertexInputAttributes(unsigned int binding, unsigned int attributeCount, const VertexInputAttribute* attribute) = 0;

        virtual void SetPrimitiveTopology(PrimitiveTopology primitiveTopology) = 0;

        virtual void SetViewport(const Viewport& viewport) = 0;
        virtual void SetScissor(const Scissor& scissor) = 0;

        virtual void EnableRasterizerDiscard(bool enable) = 0;
        virtual void SetPolygonMode(PolygonMode polygonMode) = 0;
        virtual void SetCullMode(CullMode cullMode) = 0;
        virtual void SetFrontFace(FrontFace frontFace) = 0;
        virtual void EnableDepthBias(bool enable) = 0;
        virtual void SetDepthBias(float constantFactor, float clamp, float slopeFactor) = 0;
        virtual void SetLineWidth(float lineWidth) = 0;

        virtual void EnableSampleShading(bool enable) = 0;
        virtual void SetMinSampleShading(float minSampleShading) = 0;
        virtual void EnableAlphaToCoverage(bool enable) = 0;
        virtual void EnableAlphaToOne(bool enable) = 0;

        virtual void EnableDepthTest(bool enable) = 0;
        virtual void EnableDepthWrite(bool enable) = 0;
        virtual void SetDepthCompareOp(CompareOp compareOp) = 0;

        virtual void EnableLogicOp(bool enable) = 0;
        virtual void SetLogicOp(LogicOp logicOp) = 0;
        virtual void SetBlendingConstants(const LinearColor& BlendingConstants) = 0;
        virtual void EnableBlending(unsigned int attachmentIndex, bool enable) = 0;
        virtual void SetBlendingEquation(unsigned int attachmentIndex, BlendOp colorBlendingOp, BlendOp alphaBlendingOp) = 0;
        virtual void SetBlendingFunction(unsigned int attachmentIndex, BlendFactor sourceColor, BlendFactor destinationColor, BlendFactor sourceAlpha, BlendFactor destinationAlpha) = 0;

        virtual void Draw(unsigned int vertexCount, unsigned int instanceCount, unsigned int firstVertex = 0, unsigned int firstInstance = 0) = 0;
        virtual void DrawIndexed(unsigned int indexCount, unsigned int instanceCount, unsigned int firstIndex = 0, int indexOffset = 0, unsigned int firstInstance = 0) = 0;

        /**
         * \brief 
         */
        virtual void Reset(bool releaseResources = false) = 0;

        /**
         * \brief
         * \return
         */
        virtual CommandListState GetState() const = 0;

	    /**
         * \brief 
         * \return 
         */
        virtual CommandListUsage GetUsage() const = 0;

	    /**
         * \brief 
         * \return 
         */
        virtual CommandListDynamicState GetDynamicState() const = 0;

	    /**
         * \brief 
         * \return 
         */
        virtual bool IsSecondary() const = 0;
    };
}

#endif // COCKTAIL_RENDERER_COMMAND_COMMANDLIST_HPP
