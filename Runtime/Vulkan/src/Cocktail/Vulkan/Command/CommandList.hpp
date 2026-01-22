#ifndef COCKTAIL_VULKAN_COMMAND_COMMANDLIST_HPP
#define COCKTAIL_VULKAN_COMMAND_COMMANDLIST_HPP

#include <Cocktail/Core/Signal/Observable.hpp>

#include <Cocktail/Renderer/Command/CommandList.hpp>
#include <Cocktail/Renderer/Command/CommandListCreateInfo.hpp>
#include <Cocktail/Renderer/Shader/ShaderProgramType.hpp>

#include <Cocktail/Vulkan/Volk.hpp>
#include <Cocktail/Vulkan/Pipeline/State/StateManager.hpp>

namespace Ck::Vulkan
{
	class CommandListPool;
	class ComputeStateManager;
	class DescriptorSetAllocator;
	class Framebuffer;
	class GraphicStateManager;
	class Pipeline;
	class RenderDevice;

	class CommandList : public Renderer::CommandList, public Observable, public std::enable_shared_from_this<CommandList>
	{
	public:

		CommandList(RenderDevice* renderDevice, std::shared_ptr<CommandListPool> allocator, DescriptorSetAllocator* descriptorSetAllocator, const Renderer::CommandListCreateInfo& createInfo);

		~CommandList() override;

		void SetObjectName(const char* name) const override;

		Renderer::RenderDevice* GetRenderDevice() const override;

		void Begin(Renderer::CommandList* primary) override;
		void End() override;

		void Barrier(unsigned barrierCount, const Renderer::GpuBarrier* barriers) override;

		void GenerateMipMaps(const Renderer::Texture* texture, Renderer::ResourceState resourceState, const Renderer::TextureSubResource& subResource) override;

		void ExecuteCommandLists(unsigned int commandListCount, Renderer::CommandList** commandLists) override;

		void ClearColorTexture(const Renderer::Texture* texture, Renderer::ResourceState state, LinearColor colorClearValue, const Renderer::TextureSubResource& subResource) override;
		void ClearDepthStencilTexture(const Renderer::Texture* texture, Renderer::ResourceState state, float depthClearValue, unsigned int stencilCleanValue, const Renderer::TextureSubResource& subResource);
		void ClearAttachments(unsigned int firstColorAttachment, unsigned int colorAttachmentCount, LinearColor colorClearValue, float depthClearValue, unsigned int stencilCleanValue) override;

		void UploadBuffer(const Renderer::Buffer* buffer, unsigned int uploadCount, const Renderer::BufferUploadInfo* uploads) override;
		void UploadBuffer(const Renderer::Buffer* buffer, std::size_t offset, std::size_t length, const void* data) override;

		void UploadTexture(const Renderer::Texture* texture, Renderer::ResourceState resourceState, unsigned int uploadCount, const Renderer::TextureUploadInfo* uploads) override;
		void UploadTextureLevel(const Renderer::Texture* texture, Renderer::ResourceState resourceState, unsigned int arrayLayer, unsigned int level, const void* pixels) override;

		void BeginRenderPass(const Renderer::RenderPassBeginInfo& begin) override;
		void EndRenderPass() override;

		void BindShaderProgram(const Renderer::ShaderProgram* inShaderProgram) override;
		void BindVertexBuffer(unsigned int binding, const Renderer::Buffer* inVertexBuffer, std::size_t offset, unsigned int stride, bool instanced, unsigned int divisor) override;
		void BindIndexBuffer(const Renderer::Buffer* inIndexBuffer, std::size_t offset, Renderer::IndexType indexType) override;
		void BindSampler(Renderer::UniformSlot* slot, unsigned int arrayIndex, const Renderer::Sampler* sampler) override;
		void BindTextureSampler(Renderer::UniformSlot* slot, unsigned int arrayIndex, const Renderer::TextureView* textureView, const Renderer::Sampler* sampler) override;
		void BindTexture(Renderer::UniformSlot* uniformSlot, unsigned int arrayIndex, const Renderer::TextureView* inTextureView) override;
		void BindBuffer(Renderer::UniformSlot* uniformSlot, unsigned int arrayIndex, const Renderer::Buffer* uniformBuffer, std::size_t offset, std::size_t range) override;

		void UpdatePipelineConstant(Renderer::ShaderType shaderType, unsigned int offset, unsigned int length, const void* data) override;

		void EnableVertexBinding(unsigned int binding, bool enable) override;
		void SetVertexInputAttributes(unsigned int binding, unsigned int attributeCount, const Renderer::VertexInputAttribute* attributes) override;
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
        void SetShadingRate(Extent2D<unsigned int> sampleSize, Renderer::ShadingRateCombiner pipelineCombineOp, Renderer::ShadingRateCombiner primitiveCombineOp) override;
        void EnableSampleShading(bool enable) override;
		void SetMinSampleShading(float minSampleShading) override;
		void EnableAlphaToCoverage(bool enable) override;
		void EnableAlphaToOne(bool enable) override;
		void EnableDepthTest(bool enable) override;
		void EnableDepthWrite(bool enable) override;
		void SetDepthCompareOp(Renderer::CompareOp compareOp) override;
		void EnableLogicOp(bool enable) override;
		void SetLogicOp(Renderer::LogicOp logicOp) override;
		void SetBlendingConstants(const LinearColor& blendingConstants) override;
		void EnableBlending(unsigned attachmentIndex, bool enable) override;
		void SetBlendingEquation(unsigned attachmentIndex, Renderer::BlendOp colorBlendingOp, Renderer::BlendOp alphaBlendingOp) override;
		void SetBlendingFunction(unsigned attachmentIndex, Renderer::BlendFactor sourceColor, Renderer::BlendFactor destinationColor, Renderer::BlendFactor sourceAlpha, Renderer::BlendFactor destinationAlpha) override;

		void BeginDebugLabel(const AnsiChar* labelName, LinearColor color) override;
		void InsertDebugLabel(const AnsiChar* labelName, LinearColor color) override;
		void EndDebugLabel() override;

		void Draw(unsigned int vertexCount, unsigned int instanceCount, unsigned int firstVertex, unsigned int firstInstance) override;
		void DrawIndexed(unsigned int indexCount, unsigned int instanceCount, unsigned int firstIndex, int indexOffset, unsigned int firstInstance) override;
		void DrawIndirect(const Renderer::Buffer* buffer, std::size_t offset, unsigned int drawCount, unsigned int stride) override;
		void DrawIndexedIndirect(const Renderer::Buffer* buffer, std::size_t offset, unsigned int drawCount, unsigned int stride) override;

		void Dispatch(unsigned int groupCountX, unsigned int groupCountY, unsigned int groupCountZ) override;
		void DispatchIndirect(const Renderer::Buffer* buffer, std::size_t offset) override;
		
		void Reset(bool releaseResources);

		Renderer::CommandListState GetState() const override;
		Renderer::CommandListUsageBits GetUsage() const override;
		bool IsSecondary() const override;

		void MarkInitial();
		void MarkPending();
		void MarkCompleted();

		VkCommandBuffer GetHandle() const;

	private:

		ComputeStateManager* GetComputeStateManager() const;
		GraphicStateManager* GetGraphicStateManager() const;
		StateManager* GetStateManager(Renderer::ShaderProgramType programType) const;

		void FlushComputeState();
		void FlushGraphicState();
		void FlushState(Renderer::ShaderProgramType programType);

		RenderDevice* mRenderDevice;
		std::shared_ptr<CommandListPool> mAllocator;
		DescriptorSetAllocator* mDescriptorSetAllocator;
		VkCommandBuffer mHandle;
		bool mOneShot;
		bool mSecondary;
		Renderer::CommandListState mState;
		Renderer::CommandListUsageBits mUsage;
		const Framebuffer* mCurrentFramebuffer;
		Optional<Renderer::RenderPassMode> mCurrentRenderPassMode;
		EnumMap<Renderer::ShaderProgramType, std::shared_ptr<Pipeline>> mCurrentPipelines;
		EnumMap<Renderer::ShaderProgramType, UniquePtr<StateManager>> mStateManagers;
	};
}

#endif // COCKTAIL_VULKAN_COMMAND_COMMANDLIST_HPP
