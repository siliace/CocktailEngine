#ifndef COCKTAILENGINE_VULKAN_PIPELINE_GRAPHICPIPELINECREATEINFO_HPP
#define COCKTAILENGINE_VULKAN_PIPELINE_GRAPHICPIPELINECREATEINFO_HPP

#include <CocktailEngine/Vulkan/Pipeline/Layout/PipelineLayout.hpp>
#include <CocktailEngine/Vulkan/Pipeline/State/GraphicState.hpp>

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
		GraphicState State;

		/**
		 * \brief
		 */
		SharedPtr<PipelineLayout> Layout;

		/**
		 * \brief 
		 */
		SharedPtr<RenderPass> InRenderPass;

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

#endif // COCKTAILENGINE_VULKAN_PIPELINE_GRAPHICPIPELINECREATEINFO_HPP
