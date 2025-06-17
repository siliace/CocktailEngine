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
	 * \brief Interface representing an opaque handle to a GPU command list object
	 * A CommandList is an object used to record commands which can be subsequently submitted to a RenderContext for execution.
	 * There are two types of CommandList: primary and secondary.
	 * A primary CommandList can execute secondary CommandList and be submitted directly to a RenderContext.
	 * A secondary CommandList, can be executed by primary CommandList, and should not be directly submitted to a RenderContext.
	 */
	class CommandList : public RenderDeviceObject
	{
	public:

		/**
		 * \brief Start recording command into the CommandList.
		 * The current state of the CommandList must be Initial.
		 * If the CommandList has been created with CommandListCreateInfo::Secondary set to true
		 * \p primary must be the provided with the CommandList that will execute this one.
		 * \param primary The primary CommandList
		 */
		virtual void Begin(CommandList* primary) = 0;

		/**
		 * \brief Finish recording a command buffer
		 * The current state of the CommandList must be Recording or in RecordingRenderPass.
		 * If the current state is RecordingRenderPass, the render pass will be automatically ended as well.
		 */
		virtual void End() = 0;

		/**
		 * \brief Insert a memory dependency between the last operation in the CommandList and the next one
		 * The current state of the CommandList must be Recording or in RecordingRenderPass.
		 * \param barrierCount The number of barrier
		 * \param barriers Barriers to insert in the CommandList
		 */
		virtual void Barrier(unsigned int barrierCount, const GpuBarrier* barriers) = 0;

		/**
		 * \brief Generate the chain of mipmaps for a texture
		 * The current state of the CommandList must be Recording.
		 * \param texture The texture to generate its mipmaps
		 * \param resourceState The current state of the Texture
		 * \param subResource The subresource to generate
		 */
		virtual void GenerateMipMaps(const Texture* texture, ResourceState resourceState, const TextureSubResource& subResource) = 0;

		/**
		 * \brief Execute secondary CommandList
		 * Each CommandList in \p commandLists should have been created as secondary
		 * \param commandListCount The number of CommandList to execute
		 * \param commandLists The CommandLists to execute
		 */
		virtual void ExecuteCommandLists(unsigned int commandListCount, CommandList** commandLists) = 0;

		/**
		 * \brief Upload data into a Buffer's memory
		 * \param buffer The Buffer to update
		 * \param uploadCount The number of upload to process
		 * \param uploads Uploads to process
		 */
		virtual void UploadBuffer(const Buffer* buffer, unsigned int uploadCount, const BufferUploadInfo* uploads) = 0;

		/**
		 * \brief Upload data into a Buffer's memory
		 * \param buffer The Buffer to update
		 * \param offset The offset where upload the \p data in the \p buffer
		 * \param length The length of \p data to upload
		 * \param data Data to upload
		 */
		virtual void UploadBuffer(const Buffer* buffer, std::size_t offset, std::size_t length, const void* data) = 0;

		/**
		 * \brief Upload data into a Texture's memory
		 * \param texture The Texture to update
		 * \param resourceState The current state of \p texture
		 * \param uploadCount The number of upload to process
		 * \param uploads Uploads to process
		 */
		virtual void UploadTexture(const Texture* texture, ResourceState resourceState, unsigned int uploadCount, const TextureUploadInfo* uploads) = 0;

		/**
		 * \brief Upload data into a Texture's memory
		 * \param texture The Texture to update
		 * \param resourceState The current state of \p texture
		 * \param arrayLayer The layer of the level to upload
		 * \param level The level to upload
		 * \param pixels Pixels to upload in the level of \p texture
		 */
		virtual void UploadTextureLevel(const Texture* texture, ResourceState resourceState, unsigned int arrayLayer, unsigned int level, const void* pixels) = 0;

		/**
		 * \brief Start a render pass
		 * The current state of the CommandList must be Recording.
		 * \param begin The parameters to begin the render pass
		 */
		virtual void BeginRenderPass(const RenderPassBeginInfo& begin) = 0;

		/**
		 * \brief End the render pass
		 * The current state of the CommandList must be RecordingRenderPass.
		 */
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

		/**
		 * \brief Issue a draw call in the CommandList
		 * The current state of the CommandList must be RecordingRenderPass.
		 * \param vertexCount The number of vertices to draw
		 * \param instanceCount The number of instance to draw
		 * \param firstVertex The first vertex to draw
		 * \param firstInstance The first instance to draw
		 */
		virtual void Draw(unsigned int vertexCount, unsigned int instanceCount, unsigned int firstVertex = 0, unsigned int firstInstance = 0) = 0;

		/**
		 * \brief Issue an indexed draw call in the CommandList
		 * The current state of the CommandList must be RecordingRenderPass.
		 * \param indexCount The number of index to draw
		 * \param instanceCount The number of instance to draw
		 * \param firstIndex The first index to draw
		 * \param indexOffset A value added to the index before accessing the vertex buffer
		 * \param firstInstance The first instance to draw
		 */
		virtual void DrawIndexed(unsigned int indexCount, unsigned int instanceCount, unsigned int firstIndex = 0, int indexOffset = 0, unsigned int firstInstance = 0) = 0;

		/**
		 * \brief Issue a dispatch call in the CommandList
		 * The current state of the CommandList must be Recording.
		 * When the command is executed, a global workgroup consisting of groupCountX * groupCountY * groupCountZ local workgroups is assembled.
		 * \param groupCountX The number of local workgroups to dispatch in the X dimension
		 * \param groupCountY The number of local workgroups to dispatch in the Y dimension
		 * \param groupCountZ The number of local workgroups to dispatch in the Z dimension
		 */
		virtual void Dispatch(unsigned int groupCountX, unsigned int groupCountY = 0, unsigned int groupCountZ = 0) = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual CommandListState GetState() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual CommandListUsageBits GetUsage() const = 0;

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
