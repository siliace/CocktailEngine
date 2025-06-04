#ifndef COCKTAIL_VULKAN_PIPELINE_PIPELINECACHE_HPP
#define COCKTAIL_VULKAN_PIPELINE_PIPELINECACHE_HPP

#include <Cocktail/Core/Utility/ByteArray.hpp>

#include <Cocktail/Vulkan/Pipeline/ComputePipeline.hpp>
#include <Cocktail/Vulkan/Pipeline/GraphicPipeline.hpp>
#include <Cocktail/Vulkan/Pipeline/GraphicPipelineCreateInfo.hpp>
#include <Cocktail/Vulkan/Pipeline/PipelineCacheCreateInfo.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	using PipelineStateHash = std::size_t;

	/**
	 * \brief 
	 */
	class PipelineCache
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		PipelineCache(std::shared_ptr<RenderDevice> renderDevice, const PipelineCacheCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~PipelineCache();
		
		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<ComputePipeline> CreateComputePipeline(const ComputePipelineCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<GraphicPipeline> CreateGraphicPipeline(const GraphicPipelineCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param name 
		 */
		void SetObjectName(const char* name) const;

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<RenderDevice> GetRenderDevice() const;

		/**
		 * \brief 
		 * \return 
		 */
		ByteArray GetCacheData() const;

		/**
		 * \brief 
		 * \return 
		 */
		VkPipelineCache GetHandle() const;

	private:

		std::shared_ptr<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkPipelineCache mHandle;
		std::unordered_map<PipelineStateHash, std::shared_ptr<Pipeline>> mCache;
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_PIPELINECACHE_HPP
