#ifndef COCKTAIL_VULKAN_PIPELINE_STATE_GRAPHICSTATEMANAGER_HPP
#define COCKTAIL_VULKAN_PIPELINE_STATE_GRAPHICSTATEMANAGER_HPP

#include <Cocktail/Vulkan/Pipeline/State/GraphicState.hpp>
#include <Cocktail/Vulkan/Pipeline/State/StateManager.hpp>

namespace Ck::Vulkan
{
	class RenderPass;

	struct IndexBufferBindingInfo
	{
		const Buffer* Buffer = nullptr;
		std::size_t Offset = 0;
		Renderer::IndexType IndexType = Renderer::IndexType::Integer;
	};

	struct IndexBufferBinding
	{
		VkBuffer BufferHandle = VK_NULL_HANDLE;
		std::size_t Offset = 0;
		VkIndexType IndexType = VK_INDEX_TYPE_UINT32;
	};

	struct VertexBufferBindingRange
	{
		unsigned int BindingCount = 0;
		VkBuffer BufferHandles[MaxInputBindings] = {};
		std::size_t BufferOffsets[MaxInputBindings] = {};
	};

	struct VertexBufferBindingInfo
	{
		const Buffer* Buffer = nullptr;
		std::size_t Offset = 0;
	};

	/**
	 * \brief 
	 */
	class GraphicStateManager : public StateManager
	{
	public:

		/**
		 * \brief 
		 */
		enum class GraphicDirtyFlagBits
		{
			/**
			 * \brief 
			 */
			VertexBuffers = Bit(0),

			/**
			 * \brief 
			 */
			IndexBuffer = Bit(1),
		};

		/**
		 * \brief 
		 * \param renderDevice
		 * \param descriptorSetAllocator
		 * \param dynamicState 
		 */
		GraphicStateManager(std::shared_ptr<RenderDevice> renderDevice, DescriptorSetAllocator* descriptorSetAllocator, Renderer::CommandListDynamicState dynamicState);

		void SetShaderProgram(const ShaderProgram* shaderProgram) override;
		void SetEntryPoint(Renderer::ShaderType shaderType, const char* name) override;
		void EnableVertexBinding(unsigned int binding, bool enable);
		void SetVertexInputBinding(unsigned int binding, unsigned int stride, bool instanced, unsigned int divisor);
		void SetVertexInputAttributes(unsigned int binding, unsigned int attributeCount, const Renderer::VertexInputAttribute* attribute);
		void SetPrimitiveTopology(Renderer::PrimitiveTopology primitiveTopology);
		void SetViewport(unsigned int index, const Renderer::Viewport& viewport);
		void SetScissor(unsigned int index, const Renderer::Scissor& scissor);
		void EnableRasterizerDiscard(bool enable);
		void SetPolygonMode(Renderer::PolygonMode polygonMode);
		void SetCullMode(Renderer::CullMode cullMode);
		void SetFrontFace(Renderer::FrontFace frontFace);
		void EnableDepthBias(bool enable);
		void SetDepthBias(float constantFactor, float clamp, float slopeFactor);
		void SetLineWidth(float lineWidth);
		void EnableSampleShading(bool enable);
		void SetMinSampleShading(float minSampleShading);
		void EnableAlphaToCoverage(bool enable);
		void EnableAlphaToOne(bool enable);
		void EnableDepthTest(bool enable);
		void EnableDepthWrite(bool enable);
		void SetDepthCompareOp(Renderer::CompareOp compareOp);
		void EnableLogicOp(bool enable);
		void SetLogicOp(Renderer::LogicOp logicOp);
		void SetBlendingConstants(const LinearColor& blendingConstants);
		void EnableBlending(unsigned int attachmentIndex, bool enable);
		void SetBlendingEquation(unsigned int attachmentIndex, Renderer::BlendOp colorBlendOp, Renderer::BlendOp alphaBlendOp);
		void SetBlendingFunction(unsigned int attachmentIndex, Renderer::BlendFactor sourceColor, Renderer::BlendFactor destinationColor, Renderer::BlendFactor sourceAlpha, Renderer::BlendFactor destinationAlpha);
		void SetRenderPass(const std::shared_ptr<RenderPass>& renderPass);

		void BindIndexBuffer(const Buffer* indexBuffer, std::size_t offset, Renderer::IndexType indexType);
		void BindVertexBuffer(unsigned int binding, const Buffer* vertexBuffer, std::size_t offset);

		void ResetBindings() override;

		std::shared_ptr<Pipeline> CompilePipeline() override;
		IndexBufferBinding CompileIndexBuffer();
		VertexBufferBindingRange CompileVertexBuffers(unsigned int binding);

		bool IsDirty(GraphicDirtyFlagBits flag) const;
		bool IsVertexBufferDirty(unsigned int binding) const;

	private:

		Renderer::CommandListDynamicState mDynamicState;
		GraphicState mState;
		const ShaderProgram* mShaderProgram;
		std::shared_ptr<RenderPass> mRenderPass;
		Flags<GraphicDirtyFlagBits> mGraphicDirtyFlags;
		IndexBufferBindingInfo mIndexBufferBinding;
		VertexBufferBindingInfo mVertexBufferBindings[MaxInputBindings];
		unsigned int mVertexBufferDirtyFlags;
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_STATE_GRAPHICSTATEMANAGER_HPP
