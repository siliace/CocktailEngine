#ifndef COCKTAILENGINE_VULKAN_PIPELINE_PIPELINECACHECREATEINFO_HPP
#define COCKTAILENGINE_VULKAN_PIPELINE_PIPELINECACHECREATEINFO_HPP

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	struct PipelineCacheCreateInfo
	{
		/**
		 * \brief 
		 */
		std::size_t InitialDataSize = 0;

		/**
		 * \brief 
		 */
		const void* InitialData = nullptr;

        /**
         * \brief Specifies the name of the PipelineCache to create
         * If the extension Debug is not supported by the RenderDevice creating the PipelineCache, this parameter is ignored
         * \see RenderDeviceObject::GetName
         */
        const char* Name = nullptr;
	};
}

#endif // COCKTAILENGINE_VULKAN_PIPELINE_PIPELINECACHECREATEINFO_HPP
