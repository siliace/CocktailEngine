#ifndef COCKTAILENGINE_VULKAN_PIPELINE_PIPELINECACHESAVER_HPP
#define COCKTAILENGINE_VULKAN_PIPELINE_PIPELINECACHESAVER_HPP

#include <CocktailEngine/Core/System/FileSystem/Local/LocalFileSystem.hpp>

#include <CocktailEngine/Vulkan/Volk.hpp>
#include <CocktailEngine/Vulkan/VersionDescriptor.hpp>

namespace Ck::Vulkan
{
	class PipelineManager;

	/**
	 * \brief 
	 */
	class PipelineCacheSaver
	{
	public:

		/**
		 * \brief 
		 * \param applicationName 
		 * \param applicationVersion 
		 * \return 
		 */
		static Path ComputePipelineCachePath(AsciiStringView applicationName, const VersionDescriptor& applicationVersion);

		/**
		 * \brief 
		 * \param pipelineManager 
		 * \param pipelineCachePath 
		 */
		PipelineCacheSaver(PipelineManager* pipelineManager, Path pipelineCachePath);

		/**
		 * \brief 
		 */
		void DumpPipelineCache() const;

	private:

		/**
		 * \brief 
		 */
		struct PipelineCacheHeader
		{
			unsigned int Length = 0;
			VkPipelineCacheHeaderVersion Version = VK_PIPELINE_CACHE_HEADER_VERSION_MAX_ENUM;
			unsigned int VendorId = 0;
			unsigned int DeviceId = 0;
			Uint8 Uuid[VK_UUID_SIZE];
		};

		PipelineManager* mPipelineManager;
		Path mPipelineCachePath;
	};
}

#endif // COCKTAILENGINE_VULKAN_PIPELINE_PIPELINECACHESAVER_HPP
