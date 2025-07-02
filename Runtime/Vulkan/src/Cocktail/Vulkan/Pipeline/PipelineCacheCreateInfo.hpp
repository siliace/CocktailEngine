#ifndef COCKTAIL_VULKAN_PIPELINE_PIPELINECACHECREATEINFO_HPP
#define COCKTAIL_VULKAN_PIPELINE_PIPELINECACHECREATEINFO_HPP

#include <Cocktail/Core/Utility/ByteArray.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	struct PipelineCacheCreateInfo
	{
		std::filesystem::path SavePath;

		/**
		 * \brief 
		 */
		ByteArray InitialData;

        /**
         * \brief Specifies the name of the PipelineCache to create
         * If the extension Debug is not supported by the RenderDevice creating the PipelineCache, this parameter is ignored
         * \see RenderDeviceObject::GetName
         */
        const char* Name = nullptr;
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_PIPELINECACHECREATEINFO_HPP
