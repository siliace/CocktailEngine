#include <cstring>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Buffer/Buffer.hpp>
#include <Cocktail/Vulkan/Framebuffer/RenderPass.hpp>
#include <Cocktail/Vulkan/Pipeline/PipelineManager.hpp>
#include <Cocktail/Vulkan/Pipeline/State/GraphicStateManager.hpp>
#include <Cocktail/Vulkan/Shader/ShaderProgram.hpp>

namespace Ck::Vulkan
{
	bool GraphicStateManager::IsStripTopology(Renderer::PrimitiveTopology primitiveTopology)
	{
		switch (primitiveTopology)
		{
		case Renderer::PrimitiveTopology::LineStrip:
		case Renderer::PrimitiveTopology::TriangleStrip:
		case Renderer::PrimitiveTopology::TriangleFan:
			return true;
		}

		return false;
	}

	GraphicStateManager::GraphicStateManager(RenderDevice* renderDevice, DescriptorSetAllocator* descriptorSetAllocator) :
		StateManager(renderDevice, descriptorSetAllocator),
		mShaderProgram(nullptr),
		mRenderPass(nullptr),
		mVertexBufferDirtyFlags(0)
	{
		/// Nothing
	}

	void GraphicStateManager::SetShaderProgram(const ShaderProgram* shaderProgram)
	{
		assert(shaderProgram->GetType() == Renderer::ShaderProgramType::Graphic);

		if (CheckedAssign(mShaderProgram, shaderProgram))
		{
			StateManager::SetShaderProgram(mShaderProgram);

			mDirtyFlags |= DirtyFlagBits::Pipeline;
			for (Renderer::ShaderType shaderType : Enum<Renderer::ShaderType>::Values)
				mState.ShaderStages[shaderType].Shader = mShaderStages[shaderType].get();
		}
	}

	void GraphicStateManager::SetEntryPoint(Renderer::ShaderType shaderType, const char* name)
	{
		if (std::strcmp(mState.ShaderStages[shaderType].EntryPoint, name))
		{
			mDirtyFlags |= DirtyFlagBits::Pipeline;
			mState.ShaderStages[shaderType].EntryPoint = name;
		}
	}

	void GraphicStateManager::EnableVertexBinding(unsigned int binding, bool enable)
	{
		if (CheckedAssign(mState.VertexInput.Bindings[binding].Enable, enable))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::SetVertexInputBinding(unsigned int binding, unsigned int stride, bool instanced, unsigned int divisor)
	{
		if (CheckedAssign(mState.VertexInput.Bindings[binding].Stride, stride))
			mDirtyFlags |= DirtyFlagBits::Pipeline;

		if (CheckedAssign(mState.VertexInput.Bindings[binding].Instanced, instanced))
			mDirtyFlags |= DirtyFlagBits::Pipeline;

		if (CheckedAssign(mState.VertexInput.Bindings[binding].Divisor, divisor))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::SetVertexInputAttributes(unsigned int binding, unsigned int attributeCount, const Renderer::VertexInputAttribute* attribute)
	{
	    if (CheckedAssign(mState.VertexInput.Bindings[binding].AttributeCount, attributeCount))
	        mDirtyFlags |= DirtyFlagBits::Pipeline;

        for (unsigned int i = 0; i < attributeCount; i++)
        {
        	if (CheckedAssign(mState.VertexInput.Bindings[binding].Attributes[i].Location, attribute[i].Location))
        		mDirtyFlags |= DirtyFlagBits::Pipeline;

        	if (CheckedAssign(mState.VertexInput.Bindings[binding].Attributes[i].Format, attribute[i].Format))
        		mDirtyFlags |= DirtyFlagBits::Pipeline;

        	if (CheckedAssign(mState.VertexInput.Bindings[binding].Attributes[i].Offset, attribute[i].Offset))
        		mDirtyFlags |= DirtyFlagBits::Pipeline;
        }
	}

	void GraphicStateManager::SetPrimitiveTopology(Renderer::PrimitiveTopology primitiveTopology)
	{
		if (CheckedAssign(mState.InputAssembly.PrimitiveTopology, primitiveTopology))
			mDirtyFlags |= DirtyFlagBits::Pipeline;

		if (CheckedAssign(mState.InputAssembly.PrimitiveRestartEnable, IsStripTopology(primitiveTopology)))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::EnableRasterizerDiscard(bool enable)
	{
		if (CheckedAssign(mState.Rasterization.RasterizerDiscardEnable, enable))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::SetPolygonMode(Renderer::PolygonMode polygonMode)
	{
		if (CheckedAssign(mState.Rasterization.PolygonMode, polygonMode))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::SetCullMode(Renderer::CullMode cullMode)
	{
		if (CheckedAssign(mState.Rasterization.CullMode, cullMode))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::SetFrontFace(Renderer::FrontFace frontFace)
	{
		if (CheckedAssign(mState.Rasterization.FrontFace, frontFace))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::EnableDepthBias(bool enable)
	{
		if (CheckedAssign(mState.Rasterization.DepthBiasEnable, enable))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::SetDepthBias(float constantFactor, float clamp, float slopeFactor)
	{
		if (CheckedAssign(mState.Rasterization.DepthBiasConstantFactor, constantFactor))
			mDirtyFlags |= DirtyFlagBits::Pipeline;

		if (CheckedAssign(mState.Rasterization.DepthBiasClamp, clamp))
			mDirtyFlags |= DirtyFlagBits::Pipeline;

		if (CheckedAssign(mState.Rasterization.DepthBiasSlopeFactor, slopeFactor))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::SetLineWidth(float lineWidth)
	{
		if (CheckedAssign(mState.Rasterization.LineWidth, lineWidth))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::EnableSampleShading(bool enable)
	{
		if (CheckedAssign(mState.Multisample.SampleShadingEnable, enable))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::SetMinSampleShading(float minSampleShading)
	{
		if (CheckedAssign(mState.Multisample.MinSampleShading, minSampleShading))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::EnableAlphaToCoverage(bool enable)
	{
		if (CheckedAssign(mState.Multisample.AlphaToCoverageEnable, enable))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
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

	void GraphicStateManager::SetBlendingConstants(const LinearColor& blendingConstants)
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

	void GraphicStateManager::SetRenderPass(const std::shared_ptr<RenderPass>& renderPass)
	{
		assert(renderPass);

		if (CheckedAssign(mRenderPass, renderPass))
			mDirtyFlags |= DirtyFlagBits::Pipeline;

		if (CheckedAssign(mState.Multisample.Samples, mRenderPass->GetSamples()))
			mDirtyFlags |= DirtyFlagBits::Pipeline;

		if (CheckedAssign(mState.ColorBlend.ColorBlendAttachmentCount, mRenderPass->GetColorAttachmentCount()))
			mDirtyFlags |= DirtyFlagBits::Pipeline;
	}

	void GraphicStateManager::BindIndexBuffer(const Buffer* indexBuffer, std::size_t offset, Renderer::IndexType indexType)
	{
		bool dirty = false;
		dirty |= CheckedAssign(mIndexBufferBinding.Buffer, indexBuffer);
		dirty |= CheckedAssign(mIndexBufferBinding.Offset, offset);
		dirty |= CheckedAssign(mIndexBufferBinding.IndexType, indexType);

		if (dirty)
			mGraphicDirtyFlags |= GraphicDirtyFlagBits::IndexBuffer;
	}

	void GraphicStateManager::BindVertexBuffer(unsigned int binding, const Buffer* vertexBuffer, std::size_t offset)
	{
		bool dirty = false;
		dirty |= CheckedAssign(mVertexBufferBindings[binding].Buffer, vertexBuffer);
		dirty |= CheckedAssign(mVertexBufferBindings[binding].Offset, offset);

		if (dirty)
		{
			mGraphicDirtyFlags |= GraphicDirtyFlagBits::VertexBuffers;
			mVertexBufferDirtyFlags |= Bit(binding);
		}	
	}

	void GraphicStateManager::ResetBindings()
	{
		StateManager::ResetBindings();

		mGraphicDirtyFlags = Flags<GraphicDirtyFlagBits>();

		mVertexBufferDirtyFlags = 0;
		for (unsigned int i = 0; i < MaxInputBindings; i++)
			mVertexBufferBindings[i] = {};

		mIndexBufferBinding = {};
	}

	std::shared_ptr<Pipeline> GraphicStateManager::CompilePipeline()
	{
		assert(mShaderProgram);
		assert(mRenderPass);

		GraphicPipelineCreateInfo createInfo;
		createInfo.PipelineLayout = mShaderProgram->GetPipelineLayout();
		createInfo.GraphicState = mState;
		createInfo.RenderPass = mRenderPass;
		createInfo.Subpass = 0;

		mDirtyFlags &= ~DirtyFlagBits::Pipeline;

		return mRenderDevice->Invoke([&](PipelineManager* pipelineManager) {
			return pipelineManager->CreateGraphicPipeline(createInfo);
		});
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
