#ifndef COCKTAIL_VULKAN_PIPELINE_GRAPHICPIPELINECREATEINFO_HPP
#define COCKTAIL_VULKAN_PIPELINE_GRAPHICPIPELINECREATEINFO_HPP

#include <Cocktail/Vulkan/Pipeline/Layout/PipelineLayout.hpp>
#include <Cocktail/Vulkan/Pipeline/State/GraphicState.hpp>

namespace Ck::Vulkan
{
	class RenderPass;

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
		std::shared_ptr<PipelineLayout> PipelineLayout;

		/**
		 * \brief 
		 */
		std::shared_ptr<RenderPass> RenderPass;

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
