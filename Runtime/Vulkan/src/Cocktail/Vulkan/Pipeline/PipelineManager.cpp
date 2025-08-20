#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Pipeline/PipelineManager.hpp>
#include <Cocktail/Vulkan/Shader/Shader.hpp>

namespace Ck::Vulkan
{
	namespace
	{
		std::size_t HashShaderStageState(const ShaderStageState& state)
		{
			std::size_t hash = 0;
			HashCombine(hash, static_cast<void*>(state.Shader->GetHandle()));

			std::string_view entryPoint(state.EntryPoint);
			HashCombine(hash, entryPoint);

			return hash;
		}

		PipelineStateHash HashComputeState(const ComputeState& computeState)
		{
			return HashShaderStageState(computeState.ComputeStageState);
		}

		PipelineStateHash HashGraphicState(const GraphicState& graphicState)
		{
			PipelineStateHash hash = 0;

			{
				for (Renderer::ShaderType shaderType : Enum<Renderer::ShaderType>::Values)
				{
					if (graphicState.ShaderStages[shaderType].Shader == nullptr)
						continue;

					HashMerge(hash, HashShaderStageState(graphicState.ShaderStages[shaderType]));
				}
			}

			// VertexInputState
			{
				for (unsigned int i = 0; i < MaxInputBindings; i++)
				{
					if (!graphicState.VertexInput.Bindings[i].Enable)
						continue;

					HashCombine(hash, graphicState.VertexInput.Bindings[i].AttributeCount);
					for (unsigned int j = 0; j < MaxVertexAttributes; j++)
					{
						HashCombine(hash, graphicState.VertexInput.Bindings[i].Attributes[j].Location);
						HashCombine(hash, ToVkType(graphicState.VertexInput.Bindings[i].Attributes[j].Format));
						HashCombine(hash, graphicState.VertexInput.Bindings[i].Attributes[j].Offset);
					}

					HashCombine(hash, graphicState.VertexInput.Bindings[i].Stride);
					HashCombine(hash, graphicState.VertexInput.Bindings[i].Instanced);
					HashCombine(hash, graphicState.VertexInput.Bindings[i].Divisor);
				}
			}

			// InputAssemblyState
			{
				HashCombine(hash, graphicState.InputAssembly.PrimitiveTopology);
				HashCombine(hash, graphicState.InputAssembly.PrimitiveRestartEnable);
			}

			// ViewportState
			{
				HashCombine(hash, graphicState.Viewport.ViewportCount);
				for (unsigned int i = 0; i < graphicState.Viewport.ViewportCount; i++)
				{
					HashCombine(hash, graphicState.Viewport.Viewports[i].X);
					HashCombine(hash, graphicState.Viewport.Viewports[i].Y);
					HashCombine(hash, graphicState.Viewport.Viewports[i].Width);
					HashCombine(hash, graphicState.Viewport.Viewports[i].Height);
					HashCombine(hash, graphicState.Viewport.Viewports[i].MinDepth);
					HashCombine(hash, graphicState.Viewport.Viewports[i].MaxDepth);
				}

				HashCombine(hash, graphicState.Viewport.ScissorCount);
				for (unsigned int i = 0; i < graphicState.Viewport.ScissorCount; i++)
				{
					HashCombine(hash, graphicState.Viewport.Scissors[i].Position.Width);
					HashCombine(hash, graphicState.Viewport.Scissors[i].Position.Width);
					HashCombine(hash, graphicState.Viewport.Scissors[i].Size.Width);
					HashCombine(hash, graphicState.Viewport.Scissors[i].Size.Height);
				}
			}

			// RasterizationState
			{
				HashCombine(hash, graphicState.Rasterization.RasterizerDiscardEnable);
				HashCombine(hash, graphicState.Rasterization.PolygonMode);
				HashCombine(hash, graphicState.Rasterization.CullMode);
				HashCombine(hash, graphicState.Rasterization.FrontFace);
				HashCombine(hash, graphicState.Rasterization.DepthBiasEnable);
				HashCombine(hash, graphicState.Rasterization.DepthBiasConstantFactor);
				HashCombine(hash, graphicState.Rasterization.DepthBiasClamp);
				HashCombine(hash, graphicState.Rasterization.DepthBiasSlopeFactor);
				HashCombine(hash, graphicState.Rasterization.LineWidth);
			}

			// MultisampleState
			{
				HashCombine(hash, graphicState.Multisample.Samples);
				HashCombine(hash, graphicState.Multisample.SampleShadingEnable);
				HashCombine(hash, graphicState.Multisample.MinSampleShading);
				HashCombine(hash, graphicState.Multisample.AlphaToCoverageEnable);
				HashCombine(hash, graphicState.Multisample.AlphaToOneEnable);
			}

			// DepthStencilState
			{
				HashCombine(hash, graphicState.DepthStencil.DepthTestEnable);
				HashCombine(hash, graphicState.DepthStencil.DepthWriteEnable);
				HashCombine(hash, graphicState.DepthStencil.DepthCompareOp);
				HashCombine(hash, graphicState.DepthStencil.DepthBoundsTestEnable);
				HashCombine(hash, graphicState.DepthStencil.MinDepthBounds);
				HashCombine(hash, graphicState.DepthStencil.MaxDepthBounds);
			}

			// ColorBlendState
			{
				HashCombine(hash, graphicState.ColorBlend.LogicOpEnable);
				HashCombine(hash, graphicState.ColorBlend.LogicOp);
				HashCombine(hash, graphicState.ColorBlend.ColorBlendAttachmentCount);
				for (unsigned int i = 0; i < graphicState.ColorBlend.ColorBlendAttachmentCount; i++)
				{
					HashCombine(hash, graphicState.ColorBlend.ColorBlendAttachments[i].BlendEnable);
					HashCombine(hash, graphicState.ColorBlend.ColorBlendAttachments[i].SourceColorBlendFactor);
					HashCombine(hash, graphicState.ColorBlend.ColorBlendAttachments[i].DestinationColorBlendFactor);
					HashCombine(hash, graphicState.ColorBlend.ColorBlendAttachments[i].ColorBlendOp);
					HashCombine(hash, graphicState.ColorBlend.ColorBlendAttachments[i].SourceAlphaBlendFactor);
					HashCombine(hash, graphicState.ColorBlend.ColorBlendAttachments[i].DestinationAlphaBlendFactor);
					HashCombine(hash, graphicState.ColorBlend.ColorBlendAttachments[i].AlphaBlendOp);
					HashCombine(hash, graphicState.ColorBlend.ColorBlendAttachments[i].ColorWriteMask[0]);
					HashCombine(hash, graphicState.ColorBlend.ColorBlendAttachments[i].ColorWriteMask[1]);
					HashCombine(hash, graphicState.ColorBlend.ColorBlendAttachments[i].ColorWriteMask[2]);
					HashCombine(hash, graphicState.ColorBlend.ColorBlendAttachments[i].ColorWriteMask[3]);
				}

				HashCombine(hash, graphicState.ColorBlend.BlendConstants.R);
				HashCombine(hash, graphicState.ColorBlend.BlendConstants.G);
				HashCombine(hash, graphicState.ColorBlend.BlendConstants.B);
				HashCombine(hash, graphicState.ColorBlend.BlendConstants.A);
			}

			return hash;
		}
	}

	PipelineManager::PipelineManager(RenderDevice* renderDevice) :
		mRenderDevice(renderDevice)
	{
		PipelineCacheCreateInfo cacheCreateInfo;
		mCache = std::make_shared<PipelineCache>(mRenderDevice, cacheCreateInfo, nullptr);
	}

	std::shared_ptr<ComputePipeline> PipelineManager::CreateComputePipeline(const ComputePipelineCreateInfo& createInfo)
	{
		PipelineStateHash stateHash = HashComputeState(createInfo.ComputeState);
		if (auto it = mComputePipelines.find(stateHash); it != mComputePipelines.end())
			return it->second;

		std::shared_ptr<ComputePipeline> pipeline = mRenderDevice->CreateComputePipeline(mCache.get(), createInfo);
		mComputePipelines[stateHash] = pipeline;

		return pipeline;
	}

	std::shared_ptr<GraphicPipeline> PipelineManager::CreateGraphicPipeline(const GraphicPipelineCreateInfo& createInfo)
	{
		PipelineStateHash stateHash = HashGraphicState(createInfo.GraphicState);
		if (auto it = mGraphicPipelines.find(stateHash); it != mGraphicPipelines.end())
			return it->second;

		std::shared_ptr<GraphicPipeline> pipeline = mRenderDevice->CreateGraphicPipeline(mCache.get(), createInfo);
		mGraphicPipelines[stateHash] = pipeline;

		return pipeline;
	}

	std::shared_ptr<PipelineCache> PipelineManager::GetCache() const
	{
		return mCache;
	}

	RenderDevice* PipelineManager::GetRenderDevice() const
	{
		return mRenderDevice;
	}
}
