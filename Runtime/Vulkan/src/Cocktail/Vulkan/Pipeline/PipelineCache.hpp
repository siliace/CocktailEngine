#ifndef COCKTAIL_VULKAN_PIPELINE_PIPELINECACHE_HPP
#define COCKTAIL_VULKAN_PIPELINE_PIPELINECACHE_HPP

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
		PipelineCache(const Ref<RenderDevice>& renderDevice, const PipelineCacheCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~PipelineCache();
		
		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<ComputePipeline> CreateComputePipeline(const ComputePipelineCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<GraphicPipeline> CreateGraphicPipeline(const GraphicPipelineCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param name 
		 */
		void SetObjectName(const char* name) const;

		/**
		 * \brief 
		 * \return 
		 */
		Ref<RenderDevice> GetRenderDevice() const;

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

		Ref<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkPipelineCache mHandle;
		std::unordered_map<PipelineStateHash, Ref<Pipeline>> mCache;
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_PIPELINECACHE_HPP
