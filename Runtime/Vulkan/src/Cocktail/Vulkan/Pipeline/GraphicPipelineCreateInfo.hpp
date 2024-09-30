#ifndef COCKTAIL_VULKAN_PIPELINE_GRAPHICPIPELINECREATEINFO_HPP
#define COCKTAIL_VULKAN_PIPELINE_GRAPHICPIPELINECREATEINFO_HPP

#include <Cocktail/Vulkan/Pipeline/Layout/PipelineLayout.hpp>
#include <Cocktail/Vulkan/Pipeline/State/GraphicState.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	struct GraphicPipelineCreateInfo
	{
		/**
		 * \brief 
		 */
		Renderer::CommandListDynamicState DynamicState;

		/**
		 * \brief 
		 */
		GraphicState GraphicState;

		/**
		 * \brief
		 */
		Ref<PipelineLayout> PipelineLayout;

		/**
		 * \brief 
		 */
		const RenderPass* RenderPass;

		/**
		 * \brief 
		 */
		unsigned int Subpass = 0;

		/**
		 * \brief Specifies the name of the Pipeline to create
		 * If the extension Debug is not supported by the RenderDevice creating the GraphicPipeline, this parameter is ignored
		 * \see RenderDeviceObject::GetName
		 */
		const char* Name = nullptr;
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_GRAPHICPIPELINECREATEINFO_HPP
