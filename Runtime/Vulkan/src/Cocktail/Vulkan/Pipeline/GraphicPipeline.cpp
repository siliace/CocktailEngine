#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Framebuffer/RenderPass.hpp>
#include <Cocktail/Vulkan/Pipeline/GraphicPipeline.hpp>
#include <Cocktail/Vulkan/Pipeline/PipelineCache.hpp>
#include <Cocktail/Vulkan/Shader/Shader.hpp>

namespace Ck::Vulkan
{
	GraphicPipeline::GraphicPipeline(RenderDevice* renderDevice, const PipelineCache* pipelineCache, const GraphicPipelineCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		Pipeline(renderDevice),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE)
	{
		mLayout = createInfo.PipelineLayout;

		unsigned int shaderStageCount = 0;
		VkPipelineShaderStageCreateInfo vkPipelineShaderStages[Enum<Renderer::ShaderType>::ValueCount];
		{
			for (Renderer::ShaderType type : Enum<Renderer::ShaderType>::Values)
			{
				const ShaderStageState& shaderStageState = createInfo.GraphicState.ShaderStages[type];

				const Shader* shaderStage = shaderStageState.Shader;
				if (!shaderStage)
					continue;

				VkPipelineShaderStageCreateInfo vkPipelineShaderStage{ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr };
				vkPipelineShaderStage.flags = 0;
				vkPipelineShaderStage.stage = ToVkType(type);
				vkPipelineShaderStage.module = shaderStage->GetHandle();
				vkPipelineShaderStage.pName = shaderStageState.EntryPoint;

				vkPipelineShaderStages[shaderStageCount++] = vkPipelineShaderStage;
			}
		}

		unsigned int bindingCount = 0;
		VkVertexInputBindingDescription vkInputBindings[MaxInputBindings];

		unsigned int bindingDivisorCount = 0;
		VkVertexInputBindingDivisorDescriptionEXT vkInputBindingDivisors[MaxInputBindings];

		unsigned int attributeCount = 0;
		VkVertexInputAttributeDescription vkVertexAttributes[MaxVertexAttributes];

		VkPipelineVertexInputDivisorStateCreateInfoEXT vkPipelineVertexInputDivisorState{ VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO_EXT, nullptr };
		VkPipelineVertexInputStateCreateInfo vkPipelineVertexInputState{ VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO, nullptr };
		{
			const VertexInputState& vertexInputState = createInfo.GraphicState.VertexInput;
			
			for (unsigned int i = 0; i < MaxInputBindings; i++)
			{
				const VertexInputBinding& binding = vertexInputState.Bindings[i];
				if (!binding.Enable)
					continue;

				vkInputBindings[bindingCount].binding = bindingCount;
				vkInputBindings[bindingCount].inputRate = binding.Instanced ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;
				vkInputBindings[bindingCount].stride = binding.Stride;

				if (binding.Instanced && binding.Divisor > 1)
				{
					vkInputBindingDivisors[bindingDivisorCount].binding = bindingCount;
					vkInputBindingDivisors[bindingDivisorCount].divisor = binding.Divisor;

					++bindingDivisorCount;
				}

				for (unsigned int j = 0; j < binding.AttributeCount; j++)
				{
					const Renderer::VertexInputAttribute& attribute = binding.Attributes[j];

					vkVertexAttributes[attributeCount].location = attribute.Location;
					vkVertexAttributes[attributeCount].binding = bindingCount;
					vkVertexAttributes[attributeCount].format = ToVkType(attribute.Format);
					vkVertexAttributes[attributeCount].offset = attribute.Offset;

					++attributeCount;
				}

				++bindingCount;
			}

			if (bindingDivisorCount)
			{
				vkPipelineVertexInputDivisorState.vertexBindingDivisorCount = bindingDivisorCount;
				vkPipelineVertexInputDivisorState.pVertexBindingDivisors = vkInputBindingDivisors;

				Chain(vkPipelineVertexInputState, vkPipelineVertexInputDivisorState);
			}

			vkPipelineVertexInputState.flags = 0;
			vkPipelineVertexInputState.vertexBindingDescriptionCount = bindingCount;
			vkPipelineVertexInputState.pVertexBindingDescriptions = vkInputBindings;
			vkPipelineVertexInputState.vertexAttributeDescriptionCount = attributeCount;
			vkPipelineVertexInputState.pVertexAttributeDescriptions = vkVertexAttributes;
		}

		VkPipelineInputAssemblyStateCreateInfo vkPipelineInputAssemblyState{ VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, nullptr };
		{
			const InputAssemblyState& inputAssemblyState = createInfo.GraphicState.InputAssembly;

			vkPipelineInputAssemblyState.flags = 0;
			vkPipelineInputAssemblyState.topology = ToVkType(inputAssemblyState.PrimitiveTopology);
			vkPipelineInputAssemblyState.primitiveRestartEnable = inputAssemblyState.PrimitiveRestartEnable;
		}

		VkViewport vkViewports[MaxViewports];
		VkRect2D vkScissors[MaxViewports];
		VkPipelineViewportStateCreateInfo vkPipelineViewportState{ VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, nullptr };
		{
			const ViewportState& viewportState = createInfo.GraphicState.Viewport;

			unsigned int viewportCount = viewportState.ViewportCount;
			for (unsigned int i = 0; i < viewportCount; i++)
				vkViewports[i] = ToVkType(viewportState.Viewports[i]);

			unsigned int scissorCount = viewportState.ScissorCount;
			for (unsigned int i = 0; i < viewportCount; i++)
				vkScissors[i] = ToVkType(viewportState.Scissors[i]);

			vkPipelineViewportState.flags = 0;
			vkPipelineViewportState.viewportCount = viewportCount;
			vkPipelineViewportState.pViewports = vkViewports;
			vkPipelineViewportState.scissorCount = scissorCount;
			vkPipelineViewportState.pScissors = vkScissors;
		}

		VkPipelineRasterizationStateCreateInfo vkPipelineRasterizationState{ VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, nullptr };
		{
			const RasterizationState& rasterizationState = createInfo.GraphicState.Rasterization;

			vkPipelineRasterizationState.flags = 0;
			vkPipelineRasterizationState.depthBiasClamp = false;
			vkPipelineRasterizationState.rasterizerDiscardEnable = rasterizationState.RasterizerDiscardEnable;
			vkPipelineRasterizationState.polygonMode = ToVkType(rasterizationState.PolygonMode);
			vkPipelineRasterizationState.cullMode = ToVkType(rasterizationState.CullMode);
			vkPipelineRasterizationState.frontFace = ToVkType(rasterizationState.FrontFace);
			vkPipelineRasterizationState.depthBiasEnable = rasterizationState.DepthBiasEnable;
			vkPipelineRasterizationState.depthBiasConstantFactor = rasterizationState.DepthBiasConstantFactor;
			vkPipelineRasterizationState.depthBiasClamp = rasterizationState.DepthBiasClamp;
			vkPipelineRasterizationState.depthBiasSlopeFactor = rasterizationState.DepthBiasSlopeFactor;
			vkPipelineRasterizationState.lineWidth = rasterizationState.LineWidth;
		}

		VkPipelineMultisampleStateCreateInfo vkPipelineMultisampleState{ VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO, nullptr };
		{
			const MultisampleState& multisampleState = createInfo.GraphicState.Multisample;

			vkPipelineMultisampleState.flags = 0;
			vkPipelineMultisampleState.rasterizationSamples = ToVkType(multisampleState.Samples);
			vkPipelineMultisampleState.sampleShadingEnable = multisampleState.SampleShadingEnable;
			vkPipelineMultisampleState.minSampleShading = multisampleState.MinSampleShading;
			vkPipelineMultisampleState.pSampleMask = nullptr;
			vkPipelineMultisampleState.alphaToCoverageEnable = multisampleState.AlphaToCoverageEnable;
			vkPipelineMultisampleState.alphaToOneEnable = multisampleState.AlphaToOneEnable;
		}

		VkPipelineDepthStencilStateCreateInfo vkPipelineDepthStencilState{ VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO, nullptr };
		{
			const DepthStencilState& depthStencilState = createInfo.GraphicState.DepthStencil;

			vkPipelineDepthStencilState.flags = 0;
			vkPipelineDepthStencilState.depthTestEnable = depthStencilState.DepthTestEnable;
			vkPipelineDepthStencilState.depthWriteEnable = depthStencilState.DepthWriteEnable;
			vkPipelineDepthStencilState.depthCompareOp = ToVkType(depthStencilState.DepthCompareOp);
			vkPipelineDepthStencilState.depthBoundsTestEnable = depthStencilState.DepthBoundsTestEnable;
			vkPipelineDepthStencilState.minDepthBounds = depthStencilState.MinDepthBounds;
			vkPipelineDepthStencilState.maxDepthBounds = depthStencilState.MaxDepthBounds;
		}

		VkPipelineColorBlendAttachmentState vkPipelineColorBlendAttachmentState[MaxColorAttachments];
		VkPipelineColorBlendStateCreateInfo vkPipelineColorBlendState{ VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO, nullptr };
		{
			const ColorBlendState& colorBlendState = createInfo.GraphicState.ColorBlend;

			unsigned int colorAttachmentCount = colorBlendState.ColorBlendAttachmentCount;
			for (unsigned int i = 0; i < colorAttachmentCount; i++)
			{
				const ColorBlendAttachmentState& colorBlendAttachmentState = colorBlendState.ColorBlendAttachments[i];

				vkPipelineColorBlendAttachmentState[i].blendEnable = colorBlendAttachmentState.BlendEnable;
				vkPipelineColorBlendAttachmentState[i].srcColorBlendFactor = ToVkType(colorBlendAttachmentState.SourceColorBlendFactor);
				vkPipelineColorBlendAttachmentState[i].dstColorBlendFactor = ToVkType(colorBlendAttachmentState.DestinationColorBlendFactor);
				vkPipelineColorBlendAttachmentState[i].colorBlendOp = ToVkType(colorBlendAttachmentState.ColorBlendOp);
				vkPipelineColorBlendAttachmentState[i].srcAlphaBlendFactor = ToVkType(colorBlendAttachmentState.SourceAlphaBlendFactor);
				vkPipelineColorBlendAttachmentState[i].dstAlphaBlendFactor = ToVkType(colorBlendAttachmentState.DestinationAlphaBlendFactor);
				vkPipelineColorBlendAttachmentState[i].alphaBlendOp = ToVkType(colorBlendAttachmentState.AlphaBlendOp);
				vkPipelineColorBlendAttachmentState[i].colorWriteMask = 0;
				if (colorBlendAttachmentState.ColorWriteMask[0])
					vkPipelineColorBlendAttachmentState[i].colorWriteMask |= VK_COLOR_COMPONENT_R_BIT;

				if (colorBlendAttachmentState.ColorWriteMask[1])
					vkPipelineColorBlendAttachmentState[i].colorWriteMask |= VK_COLOR_COMPONENT_G_BIT;

				if (colorBlendAttachmentState.ColorWriteMask[2])
					vkPipelineColorBlendAttachmentState[i].colorWriteMask |= VK_COLOR_COMPONENT_B_BIT;

				if (colorBlendAttachmentState.ColorWriteMask[3])
					vkPipelineColorBlendAttachmentState[i].colorWriteMask |= VK_COLOR_COMPONENT_A_BIT;
			}

			vkPipelineColorBlendState.flags = 0;
			vkPipelineColorBlendState.logicOpEnable = colorBlendState.LogicOpEnable;
			vkPipelineColorBlendState.logicOp = ToVkType(colorBlendState.LogicOp);
			vkPipelineColorBlendState.attachmentCount = colorAttachmentCount;
			vkPipelineColorBlendState.pAttachments = vkPipelineColorBlendAttachmentState;
			vkPipelineColorBlendState.blendConstants[0] = colorBlendState.BlendConstants.R;
			vkPipelineColorBlendState.blendConstants[1] = colorBlendState.BlendConstants.G;
			vkPipelineColorBlendState.blendConstants[2] = colorBlendState.BlendConstants.B;
			vkPipelineColorBlendState.blendConstants[3] = colorBlendState.BlendConstants.A;
		}

		VkDynamicState vkDynamicStates[Enum<Renderer::CommandListDynamicStateBits>::ValueCount];
		VkPipelineDynamicStateCreateInfo vkPipelineDynamicState{ VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO, nullptr };
		{
			unsigned dynamicStateCount = 0;
			for (Renderer::CommandListDynamicStateBits dynamicState : Enum<Renderer::CommandListDynamicStateBits>::Values)
			{
				if (dynamicState & createInfo.DynamicState)
					vkDynamicStates[dynamicStateCount++] = ToVkType(dynamicState);
			}

			vkPipelineDynamicState.flags = 0;
			vkPipelineDynamicState.dynamicStateCount = dynamicStateCount;
			vkPipelineDynamicState.pDynamicStates = vkDynamicStates;
		}

		VkGraphicsPipelineCreateInfo vkCreateInfo{ VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, nullptr };
		{
			vkCreateInfo.flags = 0;
			vkCreateInfo.stageCount = shaderStageCount;
			vkCreateInfo.pStages = vkPipelineShaderStages;
			vkCreateInfo.pVertexInputState = &vkPipelineVertexInputState;
			vkCreateInfo.pInputAssemblyState = &vkPipelineInputAssemblyState;
			vkCreateInfo.pViewportState = &vkPipelineViewportState;
			vkCreateInfo.pRasterizationState = &vkPipelineRasterizationState;
			vkCreateInfo.pDepthStencilState = &vkPipelineDepthStencilState;
			vkCreateInfo.pMultisampleState = &vkPipelineMultisampleState;
			vkCreateInfo.pColorBlendState = &vkPipelineColorBlendState;
			vkCreateInfo.pDynamicState = &vkPipelineDynamicState;
			vkCreateInfo.layout = mLayout->GetHandle();
			vkCreateInfo.renderPass = createInfo.RenderPass->GetHandle(Renderer::RenderPassMode::Initial);
			vkCreateInfo.subpass = createInfo.Subpass;
		}

		VkPipelineCache pipelineCacheHandle = pipelineCache ? pipelineCache->GetHandle() : VK_NULL_HANDLE;
		COCKTAIL_VK_CHECK(vkCreateGraphicsPipelines(mRenderDevice->GetHandle(), pipelineCacheHandle, 1, &vkCreateInfo, mAllocationCallbacks, &mHandle));
	}

	GraphicPipeline::~GraphicPipeline()
	{
		vkDestroyPipeline(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	std::shared_ptr<PipelineLayout> GraphicPipeline::GetLayout() const
	{
		return mLayout;
	}

	VkPipeline GraphicPipeline::GetHandle() const
	{
		return mHandle;
	}
}
