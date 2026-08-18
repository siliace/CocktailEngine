#ifndef COCKTAILENGINE_VULKAN_PIPELINE_COMPUTEPIPELINECREATEINFO_HPP
#define COCKTAILENGINE_VULKAN_PIPELINE_COMPUTEPIPELINECREATEINFO_HPP

#include <CocktailEngine/Vulkan/Pipeline/State/ComputeState.hpp>

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
		ComputeState State;

		/**
		 * \brief
		 */
		SharedPtr<PipelineLayout> Layout;

		/**
		 * \brief Specifies the name of the Pipeline to create
		 * If the extension Debug is not supported by the RenderDevice creating the GraphicPipeline, this parameter is ignored
		 * \see RenderDeviceObject::GetName
		 */
		const char* Name = nullptr;
	};
}
#endif // COCKTAILENGINE_VULKAN_PIPELINE_COMPUTEPIPELINECREATEINFO_HPP
