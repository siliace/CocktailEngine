#ifndef COCKTAIL_VULKAN_PIPELINE_COMPUTEPIPELINECREATEINFO_HPP
#define COCKTAIL_VULKAN_PIPELINE_COMPUTEPIPELINECREATEINFO_HPP

#include <Cocktail/Vulkan/Pipeline/State/ComputeState.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	struct ComputePipelineCreateInfo
	{
		/**
		 * \brief 
		 */
		ComputeState ComputeState;

		/**
		 * \brief
		 */
		std::shared_ptr<PipelineLayout> PipelineLayout;

		/**
		 * \brief Specifies the name of the Pipeline to create
		 * If the extension Debug is not supported by the RenderDevice creating the GraphicPipeline, this parameter is ignored
		 * \see RenderDeviceObject::GetName
		 */
		const char* Name = nullptr;
	};
}
#endif // COCKTAIL_VULKAN_PIPELINE_COMPUTEPIPELINECREATEINFO_HPP
