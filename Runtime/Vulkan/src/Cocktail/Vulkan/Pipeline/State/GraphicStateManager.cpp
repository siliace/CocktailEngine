#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Pipeline/GraphicPipeline.hpp>
#include <Cocktail/Vulkan/Pipeline/State/GraphicStateManager.hpp>

namespace Ck::Vulkan
{
	namespace
	{
		bool IsStripTopology(Renderer::PrimitiveTopology primitiveTopology)
		{
			if (primitiveTopology == Renderer::PrimitiveTopology::LineStrip)
				return true;

			if (primitiveTopology == Renderer::PrimitiveTopology::TriangleStrip)
				return true;

			if (primitiveTopology == Renderer::PrimitiveTopology::TriangleFan)
				return true;

			return false;
		}
	}

	GraphicStateManager::GraphicStateManager(Ref<RenderDevice> renderDevice, Ref<DescriptorSetAllocator> descriptorSetAllocator, Renderer::CommandListDynamicState dynamicState) :
		Super(std::move(renderDevice), std::move(descriptorSetAllocator)),
		mDynamicState(dynamicState),
		mShaderProgram(nullptr),
		mRenderPass(nullptr),
		mVertexBufferDirtyFlags(0)
	{
		if (!(mDynamicState & Renderer::CommandListDynamicStateBits::Viewport))
			mState.Viewport.ViewportCount = 1;

		if (!(mDynamicState & Renderer::CommandListDynamicStateBits::Scissor))
			mState.Viewport.ScissorCount = 1;
	}

	void GraphicStateManager::SetShaderProgram(const ShaderProgram* shaderProgram)
	{
		assert(shaderProgram->GetType() == Renderer::ShaderProgramType::Graphic);

		if (CheckedAssign(mShaderProgram, shaderProgram))
		{
			mDirtyFlags |= DirtyFlagBits::Pipeline;
			for (Renderer::ShaderType shaderType : Enum<Renderer::ShaderType>::Values)
				mState.ShaderStages[shaderType].Shader = Shader::Cast(shaderProgram->GetStage(shaderType)).Get();
		}
	}

	void GraphicStateManager::SetEntryPoint(Renderer::ShaderType shaderType, const char* name)
	{
		mDirtyFlags |= DirtyFlagBits::Pipeline;
		mState.ShaderStages[shaderType].EntryPoint = name;
	}

	void GraphicStateManager::EnableVertexBinding(unsigned int binding, bool enable)
	{
		mDirtyFlags |= DirtyFlagBits::Pipeline;
		mState.VertexInput.Bindings[binding].Enable = enable;
	}

	void GraphicStateManager::SetVertexInputBinding(unsigned int binding, unsigned int stride, bool instanced, unsigned int divisor)
	{
		mDirtyFlags |= DirtyFlagBits::Pipeline;
		mState.VertexInput.Bindings[binding].Stride = stride;
		mState.VertexInput.Bindings[binding].Instanced = instanced;
		mState.VertexInput.Bindings[binding].Divisor = divisor;
	}

	void GraphicStateManager::SetVertexInputAttributes(unsigned int binding, unsigned int attributeCount, const Renderer::VertexInputAttribute* attribute)
	{
		mDirtyFlags |= DirtyFlagBits::Pipeline;
		mState.VertexInput.Bindings[binding].AttributeCount = attributeCount;
		for (unsigned int i = 0; i < attributeCount; i++)
			mState.VertexInput.Bindings[binding].Attributes[i] = attribute[i];
	}

	void GraphicStateManager::SetPrimitiveTopology(Renderer::PrimitiveTopology primitiveTopology)
	{
		mDirtyFlags |= DirtyFlagBits::Pipeline;
		mState.InputAssembly.PrimitiveTopology = primitiveTopology;
		mState.InputAssembly.PrimitiveRestartEnable = IsStripTopology(primitiveTopology);
	}

	void GraphicStateManager::SetViewport(unsigned int index, const Renderer::Viewport& viewport)
	{
		if (!(mDynamicState & Renderer::CommandListDynamicStateBits::Viewport))
		{
			if (CheckedAssign(mState.Viewport.Viewports[index], viewport))
				mDirtyFlags |= DirtyFlagBits::Pipeline;
		}
	}

	void GraphicStateManager::SetScissor(unsigned int index, const Renderer::Scissor& scissor)
	{
		if (!(mDynamicState & Renderer::CommandListDynamicStateBits::Scissor))
		{
			if (CheckedAssign(mState.Viewport.Scissors[index], scissor))
				mDirtyFlags |= DirtyFlagBits::Pipeline;
		}
	}

	void GraphicStateManager::EnableRasterizerDiscard(bool enable)
	{
		mDirtyFlags |= DirtyFlagBits::Pipeline;
		mState.Rasterization.RasterizerDiscardEnable = enable;
	}

	void GraphicStateManager::SetPolygonMode(Renderer::PolygonMode polygonMode)
	{
		mDirtyFlags |= DirtyFlagBits::Pipeline;
		mState.Rasterization.PolygonMode = polygonMode;
	}

	void GraphicStateManager::SetCullMode(Renderer::CullMode cullMode)
	{
		mDirtyFlags |= DirtyFlagBits::Pipeline;
		mState.Rasterization.CullMode = cullMode;
	}

	void GraphicStateManager::SetFrontFace(Renderer::FrontFace frontFace)
	{
		mDirtyFlags |= DirtyFlagBits::Pipeline;
		mState.Rasterization.FrontFace = frontFace;
	}

	void GraphicStateManager::EnableDepthBias(bool enable)
	{
		mDirtyFlags |= DirtyFlagBits::Pipeline;
		mState.Rasterization.DepthBiasEnable = enable;
	}

	void GraphicStateManager::SetDepthBias(float constantFactor, float clamp, float slopeFactor)
	{
		mDirtyFlags |= DirtyFlagBits::Pipeline;
		mState.Rasterization.DepthBiasConstantFactor = constantFactor;
		mState.Rasterization.DepthBiasClamp = clamp;
		mState.Rasterization.DepthBiasSlopeFactor = slopeFactor;
	}

	void GraphicStateManager::SetLineWidth(float lineWidth)
	{
		if (CheckedAssign(mState.Rasterization.LineWidth, lineWidth))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::EnableSampleShading(bool enable)
	{
		mDirtyFlags |= DirtyFlagBits::Pipeline;
		mState.Multisample.SampleShadingEnable = enable;
	}

	void GraphicStateManager::SetMinSampleShading(float minSampleShading)
	{
		mDirtyFlags |= DirtyFlagBits::Pipeline;
		mState.Multisample.MinSampleShading = minSampleShading;
	}

	void GraphicStateManager::EnableAlphaToCoverage(bool enable)
	{
		mDirtyFlags |= DirtyFlagBits::Pipeline;
		mState.Multisample.AlphaToCoverageEnable = enable;
	}

	void GraphicStateManager::EnableAlphaToOne(bool enable)
	{
		if (CheckedAssign(mState.Multisample.AlphaToOneEnable, enable))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::EnableDepthTest(bool enable)
	{
		if (CheckedAssign(mState.DepthStencil.DepthTestEnable, enable))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::EnableDepthWrite(bool enable)
	{
		if (CheckedAssign(mState.DepthStencil.DepthWriteEnable, enable))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::SetDepthCompareOp(Renderer::CompareOp compareOp)
	{
		if (CheckedAssign(mState.DepthStencil.DepthCompareOp, compareOp))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::EnableLogicOp(bool enable)
	{
		if (CheckedAssign(mState.ColorBlend.LogicOpEnable, enable))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::SetLogicOp(Renderer::LogicOp logicOp)
	{
		if (CheckedAssign(mState.ColorBlend.LogicOp, logicOp))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::SetBlendingConstants(const RgbaColor<float>& blendingConstants)
	{
		if (CheckedAssign(mState.ColorBlend.BlendConstants, blendingConstants))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::EnableBlending(unsigned int attachmentIndex, bool enable)
	{
		if (CheckedAssign(mState.ColorBlend.ColorBlendAttachments[attachmentIndex].BlendEnable, enable))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::SetBlendingEquation(unsigned int attachmentIndex, Renderer::BlendOp colorBlendOp, Renderer::BlendOp alphaBlendOp)
	{
		bool dirty = false;
		dirty |= CheckedAssign(mState.ColorBlend.ColorBlendAttachments[attachmentIndex].ColorBlendOp, colorBlendOp);
		dirty |= CheckedAssign(mState.ColorBlend.ColorBlendAttachments[attachmentIndex].AlphaBlendOp, alphaBlendOp);

		if (dirty)
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::SetBlendingFunction(unsigned int attachmentIndex, Renderer::BlendFactor sourceColor, Renderer::BlendFactor destinationColor, Renderer::BlendFactor sourceAlpha, Renderer::BlendFactor destinationAlpha)
	{
		bool dirty = false;
		dirty |= CheckedAssign(mState.ColorBlend.ColorBlendAttachments[attachmentIndex].SourceColorBlendFactor, sourceColor);
		dirty |= CheckedAssign(mState.ColorBlend.ColorBlendAttachments[attachmentIndex].DestinationColorBlendFactor, destinationColor);
		dirty |= CheckedAssign(mState.ColorBlend.ColorBlendAttachments[attachmentIndex].SourceAlphaBlendFactor, sourceAlpha);
		dirty |= CheckedAssign(mState.ColorBlend.ColorBlendAttachments[attachmentIndex].DestinationAlphaBlendFactor, destinationAlpha);

		if (dirty)
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::SetRenderPass(const RenderPass* renderPass)
	{
		mRenderPass = renderPass;
		mState.Multisample.Samples = mRenderPass->GetSamples();

		if (CheckedAssign(mState.ColorBlend.ColorBlendAttachmentCount, mRenderPass->GetColorAttachmentCount()))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::BindIndexBuffer(const Buffer* indexBuffer, std::size_t offset, Renderer::IndexType indexType)
	{
		mGraphicDirtyFlags |= GraphicDirtyFlagBits::IndexBuffer;

		mIndexBufferBinding.Buffer = indexBuffer;
		mIndexBufferBinding.Offset = offset;
		mIndexBufferBinding.IndexType = indexType;
	}

	void GraphicStateManager::BindVertexBuffer(unsigned int binding, const Buffer* vertexBuffer, std::size_t offset)
	{
		mGraphicDirtyFlags |= GraphicDirtyFlagBits::VertexBuffers;
		mVertexBufferDirtyFlags |= Bit(binding);

		mVertexBufferBindings[binding].Buffer = vertexBuffer;
		mVertexBufferBindings[binding].Offset = offset;
	}

	Ref<Pipeline> GraphicStateManager::CompilePipeline()
	{
		assert(mShaderProgram);
		assert(mRenderPass);

		GraphicPipelineCreateInfo createInfo;
		createInfo.DynamicState = mDynamicState;
		createInfo.PipelineLayout = mShaderProgram->GetPipelineLayout();
		createInfo.GraphicState = mState;
		createInfo.RenderPass = mRenderPass;
		createInfo.Subpass = 0;

		mDirtyFlags &= ~DirtyFlagBits::Pipeline;

		return mRenderDevice->CreateGraphicPipeline(createInfo);
	}

	IndexBufferBinding GraphicStateManager::CompileIndexBuffer()
	{
		IndexBufferBinding indexBufferBinding;
		indexBufferBinding.BufferHandle = mIndexBufferBinding.Buffer->GetHandle();
		indexBufferBinding.Offset = mIndexBufferBinding.Offset;
		indexBufferBinding.IndexType = ToVkType(mIndexBufferBinding.IndexType);

		mGraphicDirtyFlags &= ~GraphicDirtyFlagBits::IndexBuffer;

		return indexBufferBinding;
	}

	VertexBufferBindingRange GraphicStateManager::CompileVertexBuffers(unsigned int binding)
	{
		unsigned int bindingCount = 0;
		VertexBufferBindingRange range;

		while (IsVertexBufferDirty(binding))
		{
			range.BufferHandles[bindingCount] = mVertexBufferBindings[binding].Buffer->GetHandle();
			range.BufferOffsets[bindingCount] = mVertexBufferBindings[binding].Offset;

			mVertexBufferDirtyFlags &= ~Bit(binding);

			++binding;
			++bindingCount;
		}

		range.BindingCount = bindingCount;

		if (!mVertexBufferDirtyFlags)
			mGraphicDirtyFlags &= ~GraphicDirtyFlagBits::VertexBuffers;

		return range;
	}

	bool GraphicStateManager::IsDirty(GraphicDirtyFlagBits flag) const
	{
		return mGraphicDirtyFlags & flag;
	}

	bool GraphicStateManager::IsVertexBufferDirty(unsigned int binding) const
	{
		return (mVertexBufferDirtyFlags & Bit(binding)) > 0;
	}
}
